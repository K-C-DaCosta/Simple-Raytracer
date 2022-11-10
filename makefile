CC=gcc
CFLAGS= -I./include -I/usr/include/freetype2 -lm -Wall -g3 -O3
LIBS= -lSDL2 -lm -lIL -lfreetype -lpthread
BIN_NAME=simple_raytracer
SRC_DIR = ./src

SRC = $(wildcard $(SRC_DIR)/*.c)

OBJ = $(SRC:.c=.o)
OBJ := $(subst src,obj, $(OBJ))

INCLUDE = $(wildcard ./include/*.h)
INCLUDE := $(INCLUDE) $(wildcard ./include/*.hpp);

all: build

run:
	./bin/$(BIN_NAME)

build: $(OBJ)
	$(CC)  $(OBJ)  $(CFLAGS) $(LIBS)  -o ./bin/$(BIN_NAME)

obj/%.o: src/%.c $(INCLUDE)
	$(CC) $(CFLAGS) $(LIBS) -c $< -o $@

clean:
	find ./obj -type f -iname '*.o'  -exec rm {}  \;

.PHONY: all clean run
