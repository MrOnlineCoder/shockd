/*

    This file contains platform specific includes and definitions

*/

#ifdef _WIN32

#include <winsock.h>
typedef int socklen_t;

#else // Hopefully, it will be Unix

#include <sys/socket.h>

#endif
