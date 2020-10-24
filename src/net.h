#ifndef _NET_H_
#define _NET_H_

#if defined _WIN32
#include <winsock2.h>

#elif defined __linux__
 #include <unistd.h>
 #include <sys/types.h>
 #include <sys/socket.h> 
 #include <arpa/inet.h>
 #include <netinet/in.h>
 #include <netdb.h>

 #define SOCKET int
 #define INVALID_SOCKET -1
 #define SOCKET_ERROR -1
 #define closesocket(socket) close((socket))

#endif

int init_wsa();

#endif
