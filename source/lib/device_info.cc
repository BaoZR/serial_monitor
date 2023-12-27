#include "device_info.h"

namespace serial_monitor_lib{


DeviceInfo::DeviceInfo()
{
}

DeviceInfo::DeviceInfo(const std::string& dbcc, const std::string& friendly_name)
	:dbcc_(dbcc),
	friendly_name_(friendly_name)
{

}

DeviceInfo::~DeviceInfo()
{
}

}