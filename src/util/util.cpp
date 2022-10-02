#include "util/util.hpp"

// Windows util function definitions
#if defined(_WIN64) || defined(_WIN32)
  char* multi2d::windows_error_str(const int)
  {
    int err;
    char* msgbuf = new char[256];   // for a message up to 255 bytes.

    msgbuf [0] = '\0';    // Microsoft doesn't guarantee this on man page.

    err = WSAGetLastError();

    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,   // flags
                  NULL,                // lpsource
                  err,                 // message id
                  MAKELANGID (LANG_NEUTRAL, SUBLANG_DEFAULT),    // languageid
                  msgbuf,              // output buffer
                  256,     // size of msgbuf, bytes
                  NULL);               // va_list of arguments

    return msgbuf;
  }
#endif

void multi2d::write_all(int32_t fd, uint8_t* buffer, uint16_t len)
{
  size_t remaining = len;

  while (remaining > 0) {
    auto bytes_written = send(fd, (char*)buffer, len, 0);

    if (bytes_written == -1) {
      RUNTIME_THROW(status_t::IO_ERROR,
        "Failed writing %i bytes to socket %i, only wrote %i",
        len,
        fd,
        len - remaining);
    }

    remaining -= bytes_written;
  }
}
