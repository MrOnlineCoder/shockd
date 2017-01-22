/*

    This file contains platform specific includes and definitions

*/

#ifdef _WIN32

#include <winsock.h>
typedef int socklen_t;

#else // Hopefully, it will be Unix

//ALL this stuff is not present in Linux :(
#define INADDR_ANY      0x00000000
#define INADDR_NONE     0xFFFFFFFF
#define SOCKET_ERROR	(-1)
#define closesocket     close
#define INVALID_SOCKET  -1
#define SOCKET int

#include <sys/socket.h>
#include <arpa/inet.h>

#endif
