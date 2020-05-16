CC = g++
COMP = -std=c++11 -O2 -Wall -c 
LINK = -Wall -o
ncurs = -lncurses
OBJ = main.o game.o map.o menu.o snake.o display.o periph.o

tsnake: $(OBJ)
	$(CC) $(LINK) tsnake $(OBJ) $(ncurs)

main.o: main.cpp game/game.h
	$(CC) $(COMP) main.cpp $(ncurs)

game.o: game/game.cpp game/game.h
	$(CC) $(COMP) game/game.cpp

map.o: map/map.cpp map/map.h
	$(CC) $(COMP) map/map.cpp $(ncurs)

menu.o: menu/menu.cpp menu/menu.h
	$(CC) $(COMP) menu/menu.cpp $(ncurs)

snake.o: player/snake.cpp player/snake.h
	$(CC) $(COMP) player/snake.cpp

display.o: term/display.c term/display.h
	$(CC) $(COMP) term/display.c $(ncurs)

periph.o: term/periph.c term/periph.h
	$(CC) $(COMP) term/periph.c $(ncurs)
	
clean:
	rm -rf tsnake *.o
