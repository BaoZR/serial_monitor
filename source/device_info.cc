#include "device_info.h"


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
