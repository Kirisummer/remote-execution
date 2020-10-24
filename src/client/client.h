#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "../net.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#if defined _WIN32
#include <windows.h>
#include <conio.h>

#elif defined __linux__
#include <unistd.h>

#endif

SOCKET init_client(char *host, int port);

int send_command(SOCKET client, int start, int argc, char **argv);

int get_status(SOCKET client);

int ask_kill();

void send_kill(SOCKET client, uint8_t kill);

uint16_t get_exit_code(SOCKET client);

#endif 
