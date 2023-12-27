#include "enum_device.h"


HDEVNOTIFY detect_device::RegisterNotify(const GUID& guid, HANDLE hWnd)
{
    DEV_BROADCAST_DEVICEINTERFACE notificationFilter;

    ZeroMemory(&notificationFilter, sizeof(notificationFilter));
    notificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
    notificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
    notificationFilter.dbcc_classguid = guid;

    HDEVNOTIFY hDevNotify = RegisterDeviceNotificationW(hWnd,
        &notificationFilter,
        DEVICE_NOTIFY_WINDOW_HANDLE
    );

    if (hDevNotify == NULL)
    {
        serial_monitor_utils::print_windows_error(L"Can not register device notify");
        //throw std::runtime_error("Can not register device notify");
        return NULL;
    }

    return hDevNotify;
}

void detect_device::UnRegisterNotify(HDEVNOTIFY hNotyfy)
{
    if(hNotyfy == NULL)
    {
        return;
    }
    if (FALSE == UnregisterDeviceNotification(hNotyfy))
    {
        serial_monitor_utils::print_windows_error(L"Can not unregister device notify");
        //throw std::runtime_error("Can not unregister device notify");
        return;
    }
    return;
}



std::string detect_device::GetDeviceRegistryProperty(HDEVINFO& hDevInfo, PSP_DEVINFO_DATA pdevInfoData, DWORD dwProperty)
{
    DWORD dwSize = 0;
    DWORD dwDataType = 0;

    if (NULL == pdevInfoData)
    {
        // Wrong PSP_DEVINFO_DATA parameter");
        serial_monitor_utils::print_windows_error(L"Wrong PSP_DEVINFO_DATA parameter");
        return std::string();
    }

    if (FALSE == ::SetupDiGetDeviceRegistryPropertyW(hDevInfo, pdevInfoData, dwProperty, &dwDataType, NULL, 0, &dwSize))
    {
        DWORD error = ::GetLastError();
        if (ERROR_INVALID_DATA == error)
        {
            return std::string();
        }
        if (ERROR_INSUFFICIENT_BUFFER != error)
        {
            // Can't get Device Registry Property");
            puts("Can't get Device Registry Property");
            return std::string();
        }
    }

    std::vector<wchar_t> sResult(dwSize, 0x00);
    if (sResult.empty())
    {
        // Can't get Device Registry Property is empty");
        puts("Can't get Device Registry Property is empty");
        return std::string();
    }

    if (FALSE == ::SetupDiGetDeviceRegistryPropertyW(hDevInfo, pdevInfoData, dwProperty, &dwDataType, (PBYTE)&sResult.front(), (DWORD)sResult.size(), &dwSize))
    {
        // Can't get Device Registry Property");
        puts("Can't get Device Registry Property");
        return std::string();
    }

    sResult.resize(dwSize);
    if (sResult.empty())
    {
        // Device Registry Property is empty");
        puts("Device Registry Property is empty");
        return std::string();
    }

    return serial_monitor_utils::to_string(&sResult.front());
}

// bool detect_device::ReadDeviceInfo(GUID *guid, const std::string &dbcc_name, DeviceInfo &info)
// {
//     HDEVINFO hDevInfo = ::SetupDiGetClassDevsW(guid, NULL, NULL, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);
//     if (INVALID_HANDLE_VALUE == hDevInfo)
//     {
//         // Can't Find Class Devises");
//         return false;
//     }

//     SP_DEVICE_INTERFACE_DATA devInterfaceData;
//     ZeroMemory(&devInterfaceData, sizeof(SP_DEVICE_INTERFACE_DATA));
//     devInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

//     for (DWORD dwCount = 0; ::SetupDiEnumDeviceInterfaces(hDevInfo, NULL, guid, dwCount, &devInterfaceData); ++dwCount)
//     {
//         TCHAR tcBuffer[0x400] = {0x00};
//         PSP_DEVICE_INTERFACE_DETAIL_DATA_W pDevInfoDetail = (PSP_DEVICE_INTERFACE_DETAIL_DATA_W)tcBuffer;
//         pDevInfoDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

//         SP_DEVINFO_DATA devInfoData;
//         ZeroMemory(&devInfoData, sizeof(SP_DEVINFO_DATA));
//         devInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

//         DWORD dwReqSize = 0;
//         if (TRUE == ::SetupDiGetDeviceInterfaceDetailW(hDevInfo, &devInterfaceData, pDevInfoDetail, sizeof(tcBuffer) / sizeof(TCHAR), &dwReqSize, &devInfoData))
//         {
//             std::string temp = serial_monitor_utils::to_string(pDevInfoDetail->DevicePath);
//             if (serial_monitor_utils::string_lower(temp) == serial_monitor_utils::string_lower(dbcc_name))
//             {
//                 info.SetDbcc(dbcc_name);
//                 info.SetFriendlyName(GetDeviceRegistryProperty(hDevInfo, &devInfoData, SPDRP_FRIENDLYNAME));
//                 return true;
//             }
//         }
//     }

//     return false;
// }

std::vector<serial_monitor_lib::DeviceInfo> detect_device::GetDevicesByGuid(GUID* guid)
{
    std::vector<serial_monitor_lib::DeviceInfo> devices;
    HDEVINFO hDevInfo =  ::SetupDiGetClassDevsW(guid, NULL, NULL, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);
    if (INVALID_HANDLE_VALUE == hDevInfo)
    {
        // Can't Find Class Devises");
        serial_monitor_utils::print_windows_error(L"Can't Find Class Devises");
        return devices;
    }

    SP_DEVICE_INTERFACE_DATA devInterfaceData;
    ZeroMemory(&devInterfaceData, sizeof(SP_DEVICE_INTERFACE_DATA));
    devInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

    for (DWORD dwCount = 0; ::SetupDiEnumDeviceInterfaces(hDevInfo, NULL, guid, dwCount, &devInterfaceData); ++dwCount)
    {
        TCHAR tcBuffer[0x400] = { 0x00 };
        PSP_DEVICE_INTERFACE_DETAIL_DATA_W pDevInfoDetail = (PSP_DEVICE_INTERFACE_DETAIL_DATA_W)tcBuffer;
        pDevInfoDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

        SP_DEVINFO_DATA devInfoData;
        ZeroMemory(&devInfoData, sizeof(SP_DEVINFO_DATA));
        devInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

        DWORD dwReqSize = 0;
        if (TRUE == ::SetupDiGetDeviceInterfaceDetailW(hDevInfo, &devInterfaceData, pDevInfoDetail, sizeof(tcBuffer) / sizeof(TCHAR), &dwReqSize, &devInfoData))
        {
            std::string dbcc_name = serial_monitor_utils::string_lower(serial_monitor_utils::to_string(pDevInfoDetail->DevicePath));
            {
                serial_monitor_lib::DeviceInfo info;
                info.SetDbcc(dbcc_name);
                info.SetFriendlyName(GetDeviceRegistryProperty(hDevInfo, &devInfoData, SPDRP_FRIENDLYNAME));
                devices.push_back(info);
            }
        }
    }

    return devices;
}