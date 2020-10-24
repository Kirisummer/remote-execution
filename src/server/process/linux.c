#include "process.h"

static void args_to_argv(int argc, char *args, char **argv);

HANDLE_T launch(int argc, char *args) {
	char **argv = malloc(sizeof(char *) * argc);
	args_to_argv(argc, args, argv);

	HANDLE_T handle = fork();

	if (!handle) {
		// child process
		execvp(argv[0], argv);
		abort();
	}
	else if (handle > 0) {
		// parent
		free(argv);
		return handle;
	}
	else {
		// error
		free(argv);
		return 0;
	}
}

int get_status(HANDLE_T handle, uint16_t *ret) {
	int status;
	int wait_ret = waitpid(handle, &status, WNOHANG);
	if (wait_ret > 0) {
		// process is no longer running
		if (WIFSIGNALED(status))
			// process was killed by signal
			*ret = WTERMSIG(status) + 128; // `echo $?`-compatible behaviour
		else
			// normal termination
			*ret = WEXITSTATUS(status);
		return 0;
	}
	else if (wait_ret < 0) {
		// error
		return 0;
	}
	// process is active
	return 1;
}

void kill_app(HANDLE_T handle) {
	kill(handle, SIGTERM);
}

void cleanup_handle(HANDLE_T handle) {}

static void args_to_argv(int argc, char *args, char **argv) {
	char *cur = args;
	char prev_null = 1;
	int args_remain = argc;
	while (args_remain > 0) {
		if (prev_null)
			argv[argc - args_remain--] = cur;
		prev_null = !*(cur++);
	}
}
