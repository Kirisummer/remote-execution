#include "client.h"

#include <stdio.h>

int main(int argc, char **argv) {
	if (!init_wsa()) {
		printf("WSAStartup failed");
		return 1;
	}

	SOCKET client = init_client(argv[1], atoi(argv[2]));
	if (client == INVALID_SOCKET) {
		printf("Failed to connect");
		return 2;
	}

	if (!send_command(client, 3, argc, argv)) {
		printf("Failed to start");
		return 3;
	}

	while (get_status(client)) {
		puts("Running");
		uint8_t kill_request = ask_kill();
		if (kill_request)
			puts("Sent a kill request");
		send_kill(client, kill_request);
	}
	uint16_t exit_code = get_exit_code(client);
	printf("Terminated with %u\n", (unsigned) exit_code);

	closesocket(client);
}
