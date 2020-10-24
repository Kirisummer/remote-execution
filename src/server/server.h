
#ifndef _SERVER_H_
#define _SERVER_H_

#include "../net.h"

#include <stdint.h>
#include <string.h>
#include <malloc.h>

SOCKET init_server(int port);

SOCKET listen_server(SOCKET server);

uint8_t get_argc(SOCKET client);

char *get_args(SOCKET client, uint8_t amount);

void send_status(SOCKET client, uint8_t status, uint16_t ret);

int get_kill(SOCKET client);

#endif
