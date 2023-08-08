#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>
#include <Windows.h>
#include <strsafe.h>

namespace utils
{
    std::string StringLower(const std::string& s);
    std::string to_string(std::wstring const& wstr);
    std::wstring to_wstring(std::string const& str);
    void PrintWinError(LPWSTR msg);
}

#endif