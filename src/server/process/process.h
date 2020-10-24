#ifndef _PROCESS_H_
#define _PROCESS_H_

#if defined _WIN32
#include <windows.h>
typedef PROCESS_INFORMATION* HANDLE_T;

#elif defined __linux__
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
typedef pid_t HANDLE_T;

#endif

#include <signal.h>
#include <stdint.h>
#include <stdlib.h>


HANDLE_T launch(int argc, char *args);

int get_status(HANDLE_T handle, uint16_t *ret);

void kill_app(HANDLE_T handle);

void cleanup_handle(HANDLE_T handle);

#endif
