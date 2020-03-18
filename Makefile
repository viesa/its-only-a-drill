ifeq ($(OS),Windows_NT)     # is Windows_NT on XP, 2000, 7, Vista, 10...
    detected_OS := Windows
else
    detected_OS := $(shell uname)  # same as "uname -s"
endif

ifeq ($(detected_OS), Windows)
OUTPUT := client.exe
OUTPUTSERVER := server.exe
LIBLOC := -Llib
FLAGS := -Iinclude -Wall -g -pthread
else
OUTPUT := client.out
OUTPUTSERVER := server.out
LIBLOC := -L/usr/lib/x86_64-linux-gnu
FLAGS := -Wall -g -pthread -lm
endif

CC := gcc
OUTFLAG := -o
ENTRY_CLIENT = Main_Client.c
ENTRY_SERVER = Main_Server.c
CLIENT := $(wildcard _client/*.c)
SERVER := $(wildcard _server/*.c)
INC := $(wildcard include/*.c)
INC_CORE := $(wildcard include/core/*.c)
INC_NET := $(wildcard include/net/*.c)
INC_MATH := $(wildcard include/math/*.c)
INC_CORE_CLIENT := $(wildcard include/core/_client/*.c)
INC_CORE_SERVER := $(wildcard include/core/_server/*.c)
LIBS := -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_net


myOS:
	@echo $(detected_OS)

b: $(ENTRY_CLIENT)
	$(CC) $(ENTRY_CLIENT) $(INC) $(INC_CORE) $(INC_NET) $(INC_MATH) $(INC_CORE_CLIENT) $(CLIENT) $(OUTFLAG) $(OUTPUT) $(LIBLOC) $(LIBS) $(FLAGS)

r: $(ENTRY_CLIENT)
	./$(OUTPUT)

br: $(ENTRY_CLIENT)
	make b && make r

bs: $(ENTRY_SERVER)
	$(CC) $(ENTRY_SERVER) $(INC) $(INC_CORE) $(INC_NET) $(INC_MATH) $(INC_CORE_SERVER) $(SERVER) $(OUTFLAG) $(OUTPUTSERVER) $(LIBLOC) $(LIBS) $(FLAGS)

rs: $(ENTRY_SERVER)
	./$(OUTPUTSERVER)

brs: $(ENTRY_SERVER)
	make bs && make rs