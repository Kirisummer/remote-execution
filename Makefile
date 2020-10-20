ifeq ($(OS),Windows_NT)
	PLATFORM=windows
	SOCKETLIB=-lws2_32
	EXE=.exe
	CC=gcc # make sets $(CC) to cc by default, while MinGW does not have cc symlink
else
	PLATFORM=linux
	EXE=.out
endif

LIB=-c
CFLAGS=-g

RM=rm -rf
MKDIR=mkdir -p

SRC=src
BIN=bin

all: client server
client: $(BIN)/client$(EXE)
server: $(BIN)/server$(EXE)
clean:
	$(RM) $(BIN)

# Create build directories
$(BIN):
	$(MKDIR) $(BIN)
$(BIN)/client:
	$(MKDIR) $(BIN)/client
$(BIN)/server:
	$(MKDIR) $(BIN)/server

# Client
CLIENT_BIN=$(BIN)/client/client.o $(BIN)/client/main.o

$(BIN)/client$(EXE): $(CLIENT_BIN)
	$(CC) $(CLIENT_BIN) $(CFLAGS) -o$(BIN)/client$(EXE)

$(BIN)/client/client.o: $(BIN)/client $(SRC)/client/client.c
	$(CC) $(SRC)/client/client.c $(CFLAGS) $(LIB) $(SOCKETLIB) -o$(BIN)/client/client.o

$(BIN)/client/main.o: $(BIN)/client $(SRC)/client/main.c
	$(CC) $(SRC)/client/main.c $(CFLAGS) $(LIB) -o$(BIN)/client/main.o

# Server
SERVER_BIN=$(BIN)/server/server.o $(BIN)/server/process.o $(BIN)/server/main.o

$(BIN)/server$(EXE): $(SERVER_BIN)
	$(CC) $(SERVER_BIN) $(CFLAGS) -o$(BIN)/server$(EXE)

$(BIN)/server/server.o: $(BIN)/server $(SRC)/server/server.c
	$(CC) $(SRC)/server/server.c $(CFLAGS) $(LIB) $(SOCKETLIB) -o$(BIN)/server/server.o

$(BIN)/server/main.o: $(BIN)/server $(SRC)/server/main.c
	$(CC) $(SRC)/server/main.c $(CFLAGS) $(LIB) -o$(BIN)/server/main.o

$(BIN)/server/process.o: $(BIN)/server $(SRC)/server/process/$(PLATFORM).c
	$(CC) $(SRC)/server/process/$(PLATFORM).c $(CFLAGS) $(LIB) -o$(BIN)/server/process.o
