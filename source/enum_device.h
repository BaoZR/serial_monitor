#ifndef _ENUM_DEVICE_H_
#define _ENUM_DEVICE_H_

#include <string>
#include <vector>
#include <Windows.h>
#include <SetupAPI.h>
#include <dbt.h>
#include <stdexcept>
#include "device_info.h"
#include "utils.h"

namespace detect_device{
    HDEVNOTIFY RegisterNotify(const GUID& guid, HANDLE hWnd);
    void UnRegisterNotify(HDEVNOTIFY hNotyfy);
    std::string GetDeviceRegistryProperty(HDEVINFO& hDevInfo, PSP_DEVINFO_DATA pdevInfoData, DWORD dwProperty);
    //bool ReadDeviceInfo( GUID* guid, const std::string& dbcc_name, DeviceInfo& info);
    std::vector<DeviceInfo> GetDevicesByGuid(GUID* guid);
}


#endif //_ENUM_DEVICE_H_

