#include "client.h"

#include <stdio.h>

static int parse_port(char *port);
static int valid_ip(char *ip);
static _Noreturn void usage(char *program_name);

int main(int argc, char **argv) {
	int port;
	if (argc <= 3) {
		fputs("Invalid amount of arguments\n", stderr);
		usage(argv[0]);
	}
	if (!valid_ip(argv[1])) {
		fputs("Invalid IP address\n", stderr);
		usage(argv[0]);
	}
	if (!(port = parse_port(argv[2]))) {
		fputs("Invalid port\n", stderr);
		usage(argv[0]);
	}

	if (!init_wsa()) {
		fputs("WSAStartup failed\n", stderr);
		return 1;
	}

	SOCKET client = init_client(argv[1], port);
	if (client == INVALID_SOCKET) {
		fputs("Failed to connect\n", stderr);
		return 2;
	}

	if (!send_command(client, 3, argc, argv)) {
		fputs("Failed to start\n", stderr);
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

static int parse_port(char *port) {
	long res;
	char *end;
	res = strtol(port, &end, 10);
	if (*end || res <= 0 || res > 65536)
		return 0;
	return res;
}

static int valid_ip(char *ip) {
	char *cur = ip, *end;
	long res;

#define check() \
	res = strtol(cur, &end, 10); \
	if (0 > res || res > 256) \
		return 0;

	for (int i = 0; i < 3; ++i) {
		check();
		if (*end != '.')
			return 0;
		cur = end + 1;
	}
	if (!*cur)
		return 0;
	check();
	return 1;

#undef check
}

static _Noreturn void usage(char *program_name) {
	fprintf(stderr, "Usage: %s <ip> <port> <command_name> [arg1 [arg2 [...]]]\nDefault port for server is 9090\n", program_name);
	exit(1);
}
