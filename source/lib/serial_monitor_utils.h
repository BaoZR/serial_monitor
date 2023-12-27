#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>
#include <Windows.h>
#include <strsafe.h>
#include <cctype>

namespace serial_monitor_utils
{
    std::string string_lower(const std::string& s);
    std::string to_string(std::wstring const& wstr);
    std::wstring to_wstring(std::string const& str);
    void print_windows_error(LPWSTR msg);
}

#endif