#include "panel.h"
#include <iostream>

Panel::Panel()
:notify_window_(),notify_handle(NULL)
{
}

Panel::~Panel()
{
    detect_device::UnRegisterNotify(notify_handle);
    this->notify_window_.release();
    
}

void Panel::start(){
    this->notify_window_.reset(new SimpleNotifyWindow(*this));
    this->notify_handle = detect_device::RegisterNotify(GUID_DEVINTERFACE_COMPORT,notify_window_->GetHandle());
    this->show_window_.reset(new DisplayWindow());
    this->InterfaceArrival(GUID_DEVINTERFACE_COMPORT);
}

void Panel::InterfaceArrival(const GUID& guid)
{
    std::vector<DeviceInfo> temp_devices = detect_device::GetDevicesByGuid(&const_cast<GUID&>(guid));
    for(std::vector<DeviceInfo>::const_iterator iter = temp_devices.begin();iter != temp_devices.end();iter++)
    {
        Found(*iter);

    }
}

void Panel::InterfaceRemoved(const std::string& lower_dbcc)
{
    Lost(lower_dbcc);

}


void Panel::Found(const DeviceInfo& info)
{
    section_.lock();
    if(actual_devices_.count(info.GetDbcc()) == 0)
    {
        actual_devices_[info.GetDbcc()] = info;
        this->show_window_->AddItem(info);
    }
    section_.unlock();
}

void Panel::Lost(const DeviceId& id)
{
    section_.lock();
    if (actual_devices_.count(id) != 0)
    {
        actual_devices_.erase(id);
       this->show_window_->DeleteItem(id);
    }
    section_.unlock();
}