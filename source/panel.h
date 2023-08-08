#ifndef _PANEL_H_
#define _PANEL_H_
#include "simple_notify_window.h"
#include "device_info.h"
#include <memory>
#include <mutex>
#include "device_changed.h"
#include <map>

class Panel : public IDeviceChanged{
typedef std::string DeviceId;
public:    
    explicit Panel();
    ~Panel();
	Panel(const Panel&) = delete;
	Panel& operator=(const Panel&) = delete;

    void start();
    void Found(const DeviceInfo& info);
    void Lost(const DeviceId& id);
private:
    void InterfaceArrival(const GUID& guid);
    void InterfaceRemoved(const std::string& lower_dbcc);

private:
    std::unique_ptr<SimpleNotifyWindow> window_;
	HDEVNOTIFY notify_;
    std::mutex section_;
    std::map<DeviceId,DeviceInfo> actual_devices_;
};



#endif //_PANEL_H_