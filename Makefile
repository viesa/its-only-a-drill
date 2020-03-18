ifeq ($(OS),Windows_NT)     # is Windows_NT on XP, 2000, 7, Vista, 10...
    detected_OS := Windows
else
    detected_OS := $(shell uname)  # same as "uname -s"
endif

ifeq ($(detected_OS), Windows)
C_OUT := client.exe
S_OUT := server.exe
LIB_LOC := -Llib
FLAGS_COMPILER := -Iinclude -Wall -g -pthread
else
C_OUT := client.out
S_OUT := server.out
LIB_LOC := -L/usr/lib/x86_64-linux-gnu
FLAGS_COMPILER := -Wall -g -pthread -lm
endif

CC := gcc

ENTRY_CLIENT = Main_Client.c
ENTRY_SERVER = Main_Server.c

FLAGS_OUT := -o
FLAGS_CLIENT = -DCLIENT
FLAGS_SERVER = -DSERVER
FLAGS_DEBUG = -DDEBUG

INC_DIRS = ./include/*.c ./include/core/*.c ./include/net/*.c ./include/math/*.c
INC_DIRS_CLIENT := $(INC_DIRS) ./_client/*.c ./include/core/_client/*.c
INC_DIRS_SERVER := $(INC_DIRS) ./_server/*.c ./include/core/_server/*.c
INC_CLIENT := $(wildcard $(INC_DIRS_CLIENT))
INC_SERVER := $(wildcard $(INC_DIRS_SERVER))

LIBS := -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_net

CLEAN_BUILD_CLIENT := $(CC) $(ENTRY_CLIENT) $(FLAGS_OUT) $(C_OUT) $(FLAGS_CLIENT) $(INC_CLIENT) $(LIBLOC) $(LIBS) $(FLAGS) $(FLAGS_COMPILER)
CLEAN_BUILD_SERVER := $(CC) $(ENTRY_SERVER) $(FLAGS_OUT) $(S_OUT) $(FLAGS_SERVER) $(INC_SERVER) $(LIBLOC) $(LIBS) $(FLAGS) $(FLAGS_COMPILER)

RUN_CLIENT := ./$(C_OUT)
RUN_SERVER := ./$(S_OUT)

myOS:
	@echo $(detected_OS)

bc: $(ENTRY_CLIENT)
	$(CLEAN_BUILD_CLIENT)

rc: $(ENTRY_CLIENT)
	$(RUN_CLIENT)

brc: $(ENTRY_CLIENT)
	make bc && make rc

bs: $(ENTRY_SERVER)
	$(CLEAN_BUILD_SERVER)

rs: $(ENTRY_SERVER)
	$(RUN_SERVER)

brs: $(ENTRY_SERVER)
	make bs && make rs