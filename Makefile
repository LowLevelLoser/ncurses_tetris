CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -lncurses

SRC = src/main.c src/logic.c src/rendering.c src/util.c
OBJ = $(SRC:.c=.o)

tetris: $(OBJ)
	$(CC) $(OBJ) -o tetris -lncurses

clean:
	rm -f src/*.o tetris

