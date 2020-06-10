CC = g++
CFLAGS = -std=c++11 -O2 -Wall -c 
OFLAGS = -Wall -o
LIBS = -lncurses
LIB_PACK = ar rc
OBJ = main.o game.o map.o menu.o snake.o display.o periph.o files.o

.PHONY: tsnake clean help lib

all: tsnake


help:
	@echo 'clean      Delete automatically created files.'
	@echo 'lib        Create ".a" static library.'
	@echo 'help       Show this text.'

tsnake: $(OBJ)
	$(CC) $(OFLAGS) tsnake $(OBJ) $(LIBS)

main.o: main.cpp game/game.h
	$(CC) $(CFLAGS) main.cpp

game.o: game/game.cpp game/game.h
	$(CC) $(CFLAGS) game/game.cpp

map.o: menu/map.cpp menu/map.h
	$(CC) $(CFLAGS) menu/map.cpp

menu.o: menu/menu.cpp menu/menu.h
	$(CC) $(CFLAGS) menu/menu.cpp

snake.o: player/snake.cpp player/snake.h
	$(CC) $(CFLAGS) player/snake.cpp

display.o: term/display.c term/display.h
	$(CC) $(CFLAGS) term/display.c

periph.o: term/periph.c term/periph.h
	$(CC) $(CFLAGS) term/periph.c
	

files.o: term/files.cpp term/files.h
	$(CC) $(CFLAGS) term/files.cpp

clean:
	rm -rf tsnake *.o

lib: $(OBJ)
	 $(LIB_PACK) libTSnake.a $(OBJ)
