#include "server.h"

SOCKET init_server(int port) {
	SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server == INVALID_SOCKET) {
		return server;
	}

	struct sockaddr_in address;
	memset(&address, '\0', sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = inet_addr("0.0.0.0");

	if (bind(server, (struct sockaddr *) &address, sizeof(address)) == SOCKET_ERROR) {
		closesocket(server);
		return INVALID_SOCKET;
	}

	if (listen(server, 1) == SOCKET_ERROR) {
		closesocket(server);
		return INVALID_SOCKET;
	}

	return server;
}

SOCKET listen_server(SOCKET server) {
	struct sockaddr_in client_address; 
	int addr_len;
	SOCKET client = accept(server, (struct sockaddr *) &client_address, &addr_len);
	if (client == INVALID_SOCKET) {
		return INVALID_SOCKET;
	}
	return client;
}

uint8_t get_argc(SOCKET client) {
	uint8_t amount;
	recv(client, &amount, 1, 0);
	return amount;
}

char *get_args(SOCKET client, uint8_t amount) {
	size_t buffer_size = 10;
	char *buffer = malloc(buffer_size);
	char *cur = buffer;
	size_t read = 0;
	while (amount > 0) {
		char ch;
		recv(client, &ch, 1, 0);
		++read;
		if (read == buffer_size) {
			char *new_buffer = realloc(buffer, buffer_size *= 2);
			cur = (cur - buffer) + new_buffer;
			buffer = new_buffer;
		}
		*cur++ = ch;
		if (!ch)
			--amount;
	}

	return (read < buffer_size)? realloc(buffer, read): buffer;
}

void send_status(SOCKET client, uint8_t status, uint16_t ret) {
	send(client, &status, 1, 0);
	if (!status)
		send(client, &ret, sizeof(ret), 0);
}

int get_kill(SOCKET client) {
	uint8_t kill;
	recv(client, &kill, 1, 0);
	return kill;
}

