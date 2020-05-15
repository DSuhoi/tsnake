CC = g++
COMP = -Wall -c
LINK = -Wall -o
ncurs = -lncurses
OBJ = main.o game.o map.o menu.o snake.o display.o periph.o

tsnake: $(OBJ)
	$(CC) $(LINK) tsnake $(OBJ) $(ncurs)

main.o: main.cpp game/game.h
	$(CC) $(COMP) main.cpp $(ncurs)

game.o: game/game.cpp game/game.h map/map.h menu/menu.h player/snake.h display/display.h
	$(CC) $(COMP) game/game.cpp

map.o: map/map.cpp map/map.h display/display.h
	$(CC) $(COMP) map/map.cpp $(ncurs)

menu.o: menu/menu.cpp menu/menu.h display/display.h
	$(CC) $(COMP) menu/menu.cpp $(ncurs)

snake.o: player/snake.cpp player/snake.h map/map.h display/display.h 
	$(CC) $(COMP) player/snake.cpp

display.o: display/display.cpp display/display.h
	$(CC) $(COMP) display/display.cpp $(ncurs)

periph.o: periph/periph.cpp	periph/periph.h display/display.h
	$(CC) $(COMP) periph/periph.cpp $(ncurs)
	
clean:
	rm -rf tsnake *.o
