#ifndef _PANEL_H_
#define _PANEL_H_
#include "simple_notify_window.h"
#include "device_info.h"
#include <memory>
#include <mutex>
#include "device_changed.h"
#include <map>
#include "display_window.h"

class Panel : public IDeviceChanged{
typedef std::string DeviceId;
public:    
    explicit Panel();
    ~Panel();
	Panel(const Panel&) = delete;
	Panel& operator=(const Panel&) = delete;

    void start();
    void Found(const serial_monitor_lib::DeviceInfo& info);
    void Lost(const DeviceId& id);
private:
    void InterfaceArrival(const GUID& guid);
    void InterfaceRemoved(const std::string& lower_dbcc);

private:
    std::unique_ptr<SimpleNotifyWindow> notify_window_;
    std::unique_ptr<DisplayWindow> show_window_;
	HDEVNOTIFY notify_handle;
    std::mutex section_;
    std::map<DeviceId,serial_monitor_lib::DeviceInfo> actual_devices_;
};



#endif //_PANEL_H_