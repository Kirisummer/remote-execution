#include "net.h"

int init_wsa() {
#ifdef _WIN32
	WSADATA wsa_data;
	return !WSAStartup(MAKEWORD(2,2), &wsa_data);
#else
	return 1;
#endif
}
