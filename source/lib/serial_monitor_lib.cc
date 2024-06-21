#define SERIAL_MONITOR_LIB_EXPORTS
#include "serial_monitor_lib.h"
#include "simple_notify_window.h"
#include "enum_device.h"
#include "device_info.h"
#include <iostream>
#include <mutex>
#include <string>

typedef std::string DeviceId;

class DeviceChange : public IDeviceChanged
{
public:
    explicit DeviceChange(device_change_progress progress_cb);
    void InterfaceArrival(const GUID &guid);
    void InterfaceRemoved(const std::string &lower_dbcc);

private:
    device_change_progress progress_cb_;
};

static std::unique_ptr<SimpleNotifyWindow> window_ = nullptr;
static std::unique_ptr<DeviceChange> device_change_ = nullptr;
static HDEVNOTIFY h_dev_notify_ = nullptr;
static std::map<DeviceId, serial_monitor_lib::DeviceInfo> actual_devices_;
static bool init_flag_ = false;
static std::mutex section_;

DeviceChange::DeviceChange(device_change_progress progress_cb)
{
    this->progress_cb_ = progress_cb;
};
void DeviceChange::InterfaceRemoved(const std::string &lower_dbcc)
{
    section_.lock();
    if(actual_devices_.count(lower_dbcc) != 0)
    {
        std::string name = actual_devices_[lower_dbcc].GetFriendlyName();
        actual_devices_.erase(lower_dbcc);
        this->progress_cb_(lower_dbcc.c_str(), DEVICE_DELETE, name.c_str());
    }
    section_.unlock();
}
void DeviceChange::InterfaceArrival(const GUID &guid)
{
    std::vector<serial_monitor_lib::DeviceInfo> temp_devices = detect_device::GetDevicesByGuid(&const_cast<GUID&>(guid));
    for(std::vector<serial_monitor_lib::DeviceInfo>::const_iterator iter = temp_devices.begin();iter != temp_devices.end();iter++)
    {
        section_.lock();
        if(actual_devices_.count((*iter).GetDbcc()) == 0)
        {
            actual_devices_[(*iter).GetDbcc()] = *iter;
            this->progress_cb_((*iter).GetDbcc().c_str(), DEVICE_NEW, (*iter).GetFriendlyName().c_str());
        }
        section_.unlock();
    }
}

void SERIAL_MONITOR_API monitor_init(
    IN device_change_progress progress_cb)
{
    if (init_flag_ == true)
    {
        monitor_terminate();
    }
    device_change_.reset(new DeviceChange(progress_cb));
    window_.reset(new SimpleNotifyWindow(*device_change_));
    h_dev_notify_ = detect_device::RegisterNotify(GUID_DEVINTERFACE_COMPORT, (*window_).GetHandle());
    init_flag_ = true;
    (*device_change_).InterfaceArrival(GUID_DEVINTERFACE_COMPORT);//初始化扫描

};

void SERIAL_MONITOR_API monitor_terminate()
{
    if (init_flag_ == true)
    {
        detect_device::UnRegisterNotify(h_dev_notify_);
        window_.reset(nullptr);
        device_change_ = nullptr;
        actual_devices_.clear();
        init_flag_ = false;
    }
};