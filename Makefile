CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
LDFLAGS = -lncurses

SRC = src/main.c src/logic.c src/rendering.c src/util.c
OBJ = $(SRC:.c=.o)

TARGET = tetris
PREFIX ?= /usr/local

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

install: $(TARGET)
	install -d $(DESTDIR)$(PREFIX)/bin
	install -m 755 $(TARGET) $(DESTDIR)$(PREFIX)/bin/$(TARGET)

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)

