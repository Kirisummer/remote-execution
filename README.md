# GL_task1
Author: Eduard Trykoz

## Client/Server application to launch and track application on remote host.
- server is launched and waits for requests
- client sends a request to launch an application
- server launches an application (or reports a failure)
- server replies with application status every second
- client prints "\<Running>" if application is alive or reports application's exit code otherwise
- client can send requests to terminate an application by pressing Enter

## Work Breakdown Structure
WBS is available [here](/wbs.md)

## Build instructions:
### Windows
Install MinGW with MSYS ([guide](http://mingw.org/wiki/Getting_Started)) and git

### Linux
Install gcc, make and git

### Building
- Clone a repository:
	```sh
	git clone https://github.com/LyraLover/GL_task1.git
	cd GL_task1
	```

- Build client:
	```sh
	make client
	```

- Build server:
	```sh
	make server
	```

- Build both:
	```sh
	make
	```

- Clean build tree:
	```sh
	make clean
	```

## Examples:
Building, launching a server, launching an application and terminating it in different ways (Linux)

![Example 1](/examples/example1.gif)

Launching a server with non-default port and launching applications (Linux)

![Example 2](/examples/example2.gif)
