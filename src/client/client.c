#include "client.h"

SOCKET init_client(char *host, int port) {
	SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (client == INVALID_SOCKET) {
		return client;
	}

	struct sockaddr_in server;
	memset(&server, '\0', sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = inet_addr(host);

	if (connect(client, (struct sockaddr *) &server, sizeof(server)) == SOCKET_ERROR) {
		closesocket(client);
		return INVALID_SOCKET;
	}

	return client;
}

int send_command(SOCKET client, int start, int argc, char **argv) {
	uint8_t arg_amount = argc - start;
	send(client, &arg_amount, 1, 0);
	for (int i = start; i < argc; ++i) {
		size_t arg_len = strlen(argv[i]) + 1;
		send(client, argv[i], arg_len, 0);
	}

	int status = get_status(client);
	return status;
}

int get_status(SOCKET client) {
	uint8_t status;
	recv(client, &status, 1, 0);
	return status;
}

int ask_kill(SOCKET client) {
	int res;
#if defined _WIN32
	Sleep(1000);
	res = _kbhit();

#elif defined __linux__
	struct timeval timeout = {0, 0};
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(0, &fds);
	
	sleep(1);
	res = select(1, &fds, NULL, NULL, &timeout);
	
#endif
	if (res > 0)
		return getchar() == '\n';
	return 0;
}

void send_kill(SOCKET client, uint8_t kill) {
	send(client, &kill, 1, 0);
}

uint16_t get_exit_code(SOCKET client) {
	size_t received = 0;
	char buffer[sizeof(uint16_t)] = {0};
	while (received < sizeof(uint16_t)) {
		received += recv(client, buffer, sizeof(uint16_t) - received, 0);
	}
	return *( (uint16_t *) buffer);
}
