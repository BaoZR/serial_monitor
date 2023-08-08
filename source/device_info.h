#ifndef _DEVICE_INFO_
#define _DEVICE_INFO_

#include <string>
class DeviceInfo
{
public:
	DeviceInfo();
	DeviceInfo(
		const std::string& dbcc,
		const std::string& friendly_name
	);
	~DeviceInfo();

	std::string GetFriendlyName()
	{
		return friendly_name_;
	}

	std::string GetDbcc() const
	{
		return dbcc_;
	}

	void SetDbcc(const std::string& dbcc)
	{
		dbcc_ = dbcc;
	}

	void SetFriendlyName(const std::string& frindly_name)
	{
		friendly_name_ = frindly_name;
	}

	private:
		std::string dbcc_;
		//std::string guid_;
		std::string friendly_name_;
		//std::string device_description_;
		//std::string port_name_;//COM1 COM2
};


#endif //_DEVICE_INFO_