#include "serial_monitor_lib.h"
#include <iostream>
void print_device(const std::string& id,int status,const std::string& friendly_name)
{
    std::cout << "status:" << std::to_string(status) << std::endl;
    std::cout << "id:" << id << std::endl;
    std::cout << "friendly_name:" << friendly_name <<  std::endl;
}

int main()
{
    monitor_init(print_device);
    //monitor_init(print_device);
    //monitor_init(print_device);

    int k;
    std::cin >> k;
    monitor_terminate();
}
