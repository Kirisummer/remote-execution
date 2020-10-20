# Client
## Libraries

### Windows:
- winsocks2 for networking
- WinAPI function WaitForSingleObject() for kill request from user

### Linux:
- BSD sockets for networking
- syscall wrapper for select() for kill request from user

## WBS
1. Establish connection with server
	1. Initialise socket library
		- WSAStartup() on Windows
		- no actions required on Linux
	2. Create a socket
		1. socket()
		2. check for errors
			- socket() == INVALID_SOCKET and WSAGetLastError() on Windows
			- socket() < 0 and errno on Linux
	3. Connect to a server
		1. connect()
		2. check for errors
			- connect() == INVALID_SOCKET and WSAGetLastError() on Windows
			- connect() < 0 and errno on Linux
2. Send a command to a server
	A command is an array of zero-terminated strings with known amount of elements
	1. Send an amount of elements
		- send()
	2. Send strings one by one
		- send()
3. Receive reports about the state of the application
	1. Receive a report
		- recv() into uint8_t 
	2. If the application has not been terminated, print "Running"
		- puts()
	3. Get application's exit code otherwise and print "Terminated with code N", exit the loop
		- recv() into uint16_t
		- printf()
	4. Ask if application must be terminated with 1-second timeout
		1. Ask with timeout
			- WaitForSingleObject() on Windows
			- select() with timeout on Linux
		2. Send a request to kill the application
			- send()
	5. Print exit code of the application
		- puts()

# Server
## Libraries

### Windows:
- winsocks2 for networking
- WinAPI for process management

### Linux:
- BSD sockets for networking
- syscall wrappers for process management

## WBS
1. Create a server
	1. Initialise socket library
		- WSAStartup() on Windows
		- no actions required on Linux
	2. Create a socket
		1. socket()
		2. check for errors
			- socket() == INVALID_SOCKET and WSAGetLastError() on Windows
			- socket() < 0 and errno on Linux
	3. Bind to a port
		- bind()
	4. Listen for connections
		- listen()
2. Handling requests in a loop
	1. Accept a connection
		- accept()
	2. Read a command
		1. Read the amount of arguments
			- recv() into uint8_t
		2. Read zero-terminated arguments one by one
			- recv()
	3. Launch an application
		- fork() and execvp() on Linux
		- CreateProcess() on Windows
	4. Report the application status
		1. Report if application is still running
			1. Check if application is still running
				- GetExitCodeProcess() == STILL_ACTIVE on Windows
				- waitpid() == 0 on Linux
			2. Report application status
				- send()
	5. Terminate an application if needed
		1. Check if an application must be terminated
			- recv() into uint8_t
		2. Terminate an application if needed
			- TerminateProcess() with SIGTERM on Windows
			- kill() with SIGTERM on Linux
	6. Report an exit code
		- send() uint16_t
