ifeq ($(OS),Windows_NT)     # is Windows_NT on XP, 2000, 7, Vista, 10...
detected_OS := Windows
else
detected_OS := $(shell uname)  # same as "uname -s"
endif

ifeq ($(detected_OS), Windows)
APP_C		:= app_client.exe
APP_S		:= app_server.exe
LIB_LOC 	:= -LC:\libarys\Chaos_SDL\lib
INC_LOC 	:= -IC:\libarys\Chaos_SDL\include
else
APP_C		:= app_client.out
APP_S		:= app_server.out
LIB_LOC 	:= -L/usr/lib/x86_64-linux-gnu
INC_LOC 	:=
endif

# CC
CC 			:= gcc
ENTRY_C		:= Main_Client.c
ENTRY_S 	:= Main_Server.c

# Source files
SRCDIR  	:= ./include
ALL_SRCS	:= $(wildcard $(SRCDIR)/*.c $(SRCDIR)/core/*.c $(SRCDIR)/net/*.c $(SRCDIR)/math/*.c)
SRCS_C		:= $(filter-out $(SRCDIR)/AppServer.c, $(ALL_SRCS))
SRCS_S		:= $(filter-out $(SRCDIR)/AppClient.c, $(ALL_SRCS))

# Flags
CFLAGS  	:= -std=c11 -Wall -pthread -g -lm $(INC_LOC)

# Libraries
LIBS 		:= $(LIB_LOC) -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_net -lSDL2_ttf

BUILD_C		:= $(CC) $(ENTRY_C) $(SRCS_C) $(CFLAGS) $(INC_LOC) $(LIBS) $(FLAGS) -o $(APP_C)
BUILD_S		:= $(CC) $(ENTRY_S) $(SRCS_S) $(CFLAGS) $(INC_LOC) $(LIBS) $(FLAGS) -o $(APP_S)

RUN_C 		:= ./$(APP_C)
RUN_S 		:= ./$(APP_S)

# Targets
help:
	@echo Usage: make \<option\>
	@echo Options:
	@echo -b\<c/s\> Builds \<client/server\>
	@echo -r\<c/s\> Runs \<client/server\>
	@echo -br\<c/s\> Builds and runs \<client/server\>
myOS:
	@echo $(detected_OS)

bc: $(ENTRY_C)
	$(BUILD_C)

rc: $(ENTRY_C)
	$(RUN_C)

brc: $(ENTRY_CLIENT)
	make bc && make rc

bs: $(ENTRY_S)
	$(BUILD_S)

rs: $(ENTRY_S)
	$(RUN_S)

brs: $(ENTRY_SERVER)
	make bs && make rs
	
