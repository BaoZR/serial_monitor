#ifndef _IDEVICE_CHANGE_H_
#define _IDEVICE_CHANGE_H_
#include <windows.h>
#include <string>

struct IDeviceChanged
{
    public:
        virtual void InterfaceArrival(const GUID& guid) = 0;
        virtual void InterfaceRemoved(const std::string& lowerdbcc) = 0;
        virtual  ~IDeviceChanged() {};
};

#endif



