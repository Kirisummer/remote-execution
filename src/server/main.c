#include "server.h"
#include "process/process.h"

#include <stdio.h>

static void handle_request(SOCKET client);

int main(int argc, char **argv) {
	if (!init_wsa()) {
		printf("WSAStartup failed");
		return 1;
	}

	int port;
	if (argc >= 2)
		port = atoi(argv[1]);
	else
		port = 9090;

	SOCKET server = init_server(port);
	if (server == INVALID_SOCKET) {
		printf("Could not create a server");
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
