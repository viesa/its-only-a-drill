ifeq ($(OS),Windows_NT)     # is Windows_NT on XP, 2000, 7, Vista, 10...
detected_OS := Windows
else
detected_OS := $(shell uname)  # same as "uname -s"
endif

ifeq ($(detected_OS), Windows)
APP_C		:= app_client.exe
APP_S		:= app_server.exe
LIB_LOC 	:= -LC:/lib/
INC_LOC 	:= -IC:/include/
else
APP_C		:= app_client.out
APP_S		:= app_server.out
LIB_LOC 	:= -L/usr/lib/x86_64-linux-gnu
INC_LOC 	:=
endif

CC 			:= gcc
ENTRY_C		:= Main_Client.c
ENTRY_S 	:= Main_Server.c

SRCDIR  	:= ./include
SRCDIRS 	:= ./ ./core ./net ./math
OBJDIR		:= ./bin
OBJDIR_C	:= $(OBJDIR)/CLIENT
OBJDIR_S	:= $(OBJDIR)/SERVER

# Files and folders
ALL_SRCS	:= $(wildcard $(SRCDIR)/*.c $(SRCDIR)/core/*.c $(SRCDIR)/net/*.c $(SRCDIR)/math/*.c)
SRCS_C		:= $(filter-out $(SRCDIR)/AppServer.c, $(ALL_SRCS))
SRCS_S		:= $(filter-out $(SRCDIR)/AppClient.c, $(ALL_SRCS))

OBJS_C		:= $(patsubst $(SRCDIR)/%.c,$(OBJDIR_C)/%.o,$(SRCS_C))
OBJS_S		:= $(patsubst $(SRCDIR)/%.c,$(OBJDIR_S)/%.o,$(SRCS_S))

# Flags
CFLAGS  	:= -std=c11 -Wall -pedantic -pthread -g -lm $(INC_LOC)
LDFLAGS 	:= -std=c11

# Libraries
LIBS 		:= $(LIB_LOC) -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_net -lSDL2_ttf

# Targets
help:
	@echo Usage: make \<option\>
	@echo Options:
	@echo -b Builds app
	@echo -r Runs app
	@echo -br Builds and runs app
os:
	@echo $(detected_OS)
bc: $(APP_C)
rc: $(ENTRY_C)
	./$(APP_C)
brc:	
	make bc && make rc

bs: $(APP_S)
rs: $(ENTRY_S)
	./$(APP_S)
brs:	
	make bs && make rs

$(APP_C): buildrepo-c $(OBJS_C)
	$(CC) $(ENTRY_C) $(OBJS_C) $(LIBS) $(CFLAGS) -o $@
$(APP_S): buildrepo-s $(OBJS_S)
	$(CC) $(ENTRY_S) $(OBJS_S) $(LIBS) $(CFLAGS) -o $@

$(OBJDIR_C)/%.o: $(SRCDIR)/%.c
	$(CC) $(LDFLAGS) -c $< -o $@
$(OBJDIR_S)/%.o: $(SRCDIR)/%.c
	$(CC) $(LDFLAGS) -c $< -o $@
	
clean:
	rm $(OBJDIR) -Rf

buildrepo-c:
	@$(call make-repo-c)
buildrepo-s:
	@$(call make-repo-s)

# Create obj directory structure
define make-repo-c
	mkdir -p $(OBJDIR_C)
	for dir in $(SRCDIRS); \
	do \
		mkdir -p $(OBJDIR_C)/$$dir; \
	done
endef

define make-repo-s
	mkdir -p $(OBJDIR_S)
	for dir in $(SRCDIRS); \
	do \
		mkdir -p $(OBJDIR_S)/$$dir; \
	done
endef