#include "process.h"

static size_t args_len(int argc, char *args);
static void nulls_to_spaces(int argc, char *command);

HANDLE_T launch(int argc, char *args) {
    STARTUPINFO start_info;
    HANDLE_T proc_info = malloc(sizeof(HANDLE_T));

    ZeroMemory(&start_info, sizeof(start_info));
    start_info.cb = sizeof(start_info);
    ZeroMemory(proc_info, sizeof(HANDLE_T));

	size_t len = args_len(argc, args);
	char *command = memcpy(malloc(len), args, len);
	nulls_to_spaces(argc, command);

    // Start the child process. 
    if (!CreateProcess( 
		NULL,			// No module name (use command line)
        command,        // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &start_info,    // Pointer to STARTUPINFO structure
        proc_info)      // Pointer to PROCESS_INFORMATION structure
    ) {
		free(proc_info);
        return NULL;
    }
	return proc_info;
}

int get_status(HANDLE_T handle, uint16_t *ret) {
	WaitForSingleObject(handle->hProcess, 1);
	DWORD _ret; // GetExitCodeProcess requires LPDWORD = DWORD*
	GetExitCodeProcess(handle->hProcess, &_ret);
	*ret = _ret;
	if (*ret == STILL_ACTIVE)
		return 1;
	return 0;
}

void kill_app(HANDLE_T handle) {
	TerminateProcess(handle->hProcess, SIGTERM);
}

void cleanup_handle(HANDLE_T handle) {
	CloseHandle(handle->hProcess);
	CloseHandle(handle->hThread);
	free(handle);
}

static size_t args_len(int argc, char *args) {
	char *cur = args;
	int args_remain = argc;
	while (args_remain > 0) {
		if (*(cur++) == '\0')
			--args_remain;
	}
	return cur - args + 1;
}

static void nulls_to_spaces(int argc, char *args) {
	int args_remain = argc;
	for (char *cur = args; args_remain > 1; ++cur) {
		if (*cur == '\0') {
			*cur = ' ';
			--args_remain;
		}
	}
}
