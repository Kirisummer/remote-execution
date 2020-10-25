#include "server.h"
#include "process/process.h"

#include <stdio.h>
#include <string.h>

static void handle_request(SOCKET client);
static int parse_port(char *port);
static _Noreturn void usage(char *program_name);

int main(int argc, char **argv) {
	int port;
	if (argc == 1)
		port = 9090;
	else if (argc == 2) {
		port = parse_port(argv[1]);
		if (!port)
			usage(argv[0]);
	}
	else
		usage(argv[0]);

	if (!init_wsa()) {
		fputs("WSAStartup failed\n", stderr);
		return 1;
	}


	SOCKET server = init_server(port);
	if (server == INVALID_SOCKET) {
		fputs("Could not create a server", stderr);
		return 1;
	}
	while (1) {
		SOCKET client = listen_server(server);
		handle_request(client);
		closesocket(client);
	}

	return 0;
}

static void handle_request(SOCKET client) {
	uint8_t argc = get_argc(client);
	char *args = get_args(client, argc);

	HANDLE_T handle = launch(argc, args);
	if (!handle) {
		send_status(client, 0, 0);
		closesocket(client);
		cleanup_handle(handle);
	}
	send_status(client, 1, 0);

	uint8_t status;
	uint16_t ret;
	do {
		status = get_status(handle, &ret);
		send_status(client, status, ret);
		if (status) {
			if (get_kill(client))
				kill_app(handle);
		}
	} while (status);

	free(args);
}

static int parse_port(char *port) {
	long res;
	char *end;
	res = strtol(port, &end, 10);
	if (*end || res <= 0 || res > 65536)
		return 0;
	return res;
}

static _Noreturn void usage(char *program_name) {
	fprintf(stderr, "Usage: %s [port]\nDefault port is 9090\n", program_name);
	exit(1);
}
