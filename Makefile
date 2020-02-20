ifeq ($(OS),Windows_NT)     # is Windows_NT on XP, 2000, 7, Vista, 10...
    detected_OS := Windows
else
    detected_OS := $(shell uname)  # same as "uname -s"
endif

ifeq ($(detected_OS), Windows)
OUTPUT := main.exe
LIBLOC := -Llib
FLAGS := -Iinclude -Wall -g -pthread
else
OUTPUT := main.out
LIBLOC := -L/usr/lib/x86_64-linux-gnu
FLAGS := -Wall -g -pthread -lm
endif

CC := gcc
OUTFLAG := -o
ENTRY = MainWin.c
SRC := include
SRCS := $(wildcard $(SRC)/*.c)
SRCSCORE := $(wildcard $(SRC)/core/*.c)
LIBS := -lSDL2 -lSDL2_image -lSDL2_mixer


myOS:
	@echo $(detected_OS)

b: $(ENTRY)
	$(CC) $(ENTRY) $(SRCS) $(SRCSCORE) $(SRCSSHADERS) $(SRCSSHAPES) $(OUTFLAG) $(OUTPUT) $(LIBLOC) $(LIBS) $(FLAGS)

r: $(ENTRY)
	./$(OUTPUT)

br: $(ENTRY)
	make b && make r