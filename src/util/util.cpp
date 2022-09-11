#include "util/util.hpp"

// Windows util function definitions
#if defined(_WIN64) || defined(_WIN32)
  std::string multi2d::windows_error_str(const int)
  {
    wchar_t* s = NULL;
    FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 
               NULL, WSAGetLastError(),
               MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
               (LPWSTR)&s, 0, NULL);

    std::wstring ws(s);

    LocalFree(s);

    return std::string(ws.begin(), ws.end());
  }
#endif