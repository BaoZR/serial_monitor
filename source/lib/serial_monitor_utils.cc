#include "serial_monitor_utils.h"




std::string serial_monitor_utils::string_lower(const std::string &s)
{
    std::string str = s;
    for (size_t i = 0; i < str.size(); i++)
    {
        str[i] = std::tolower(str[i]);
    }
    return str;
}

void serial_monitor_utils::print_windows_error(LPWSTR msg)
{
    // Retrieve the system error message for the last-error code

    LPVOID lpMsgBuf = NULL;
    LPVOID lpDisplayBuf = NULL;
    DWORD dw = GetLastError();

    FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
        (LPWSTR)&lpMsgBuf,
        0, NULL);

    // Display the error message and exit the process

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
                                      (lstrlenW((LPCWSTR)lpMsgBuf) + lstrlenW((LPCWSTR)msg) + 40) * sizeof(WCHAR));

    if (lpDisplayBuf == NULL)
    {
        LocalFree(lpMsgBuf);
        puts("can't alloc memory\n");
        return;
    }
    StringCchPrintfW((LPWSTR)lpDisplayBuf,
                     LocalSize(lpDisplayBuf) / sizeof(WCHAR),
                     L"%s failed with error %d: %s",
                     msg, dw, (LPWSTR)lpMsgBuf);
    puts(serial_monitor_utils::to_string((LPWSTR)lpDisplayBuf).c_str());

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
    return;
}

std::wstring serial_monitor_utils::to_wstring(std::string const &str)
{
    std::wstring ustr;
    int const length = MultiByteToWideChar(
        CP_UTF8, 0,
        str.c_str(), static_cast<int>(str.length()),
        nullptr, 0);
    if (length <= 0)
        return ustr;
    ustr.resize(static_cast<size_t>(length));
    int const result = MultiByteToWideChar(
        CP_UTF8, 0,
        str.c_str(), static_cast<int>(str.length()),
        const_cast<wchar_t *>(ustr.data()), length);
    if (result != length)
        ustr.clear();
    return ustr;
}

std::string serial_monitor_utils::to_string(std::wstring const &wstr)
{
    std::string str;
    int const length = WideCharToMultiByte(
        CP_UTF8, 0,
        wstr.c_str(), static_cast<int>(wstr.length()),
        nullptr, 0,
        nullptr, nullptr);
    if (length <= 0)
        return str;
    str.resize(static_cast<size_t>(length));
    int const result = WideCharToMultiByte(
        CP_UTF8, 0,
        wstr.c_str(), static_cast<int>(wstr.length()),
        const_cast<char *>(str.data()), length,
        nullptr, nullptr);
    if (result != length)
        str.clear();
    return str;
}
