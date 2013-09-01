CFLAGS=-Wall -Wextra -g -std=c11 -fms-extensions -D_GNU_SOURCE -lSDL2_ttf -lSDL2_image -lSDL2_gfx `sdl2-config --libs --cflags` 

APP=echo_lake

OBJECTS= echo_lake.o player.o game.o components.o entity.o systems.o

all: $(OBJECTS)
	gcc -o $(APP) $(CFLAGS) $(OBJECTS)

echo_lake.o: echo_lake.c echo_lake.h player.h game.h entity.h components.h systems.h
	gcc -c $(CFLAGS) echo_lake.c

player.o: player.c player.h actor.h
	gcc -c $(CFLAGS) player.c

game.o: game.c game.h systems.h components.h
	gcc -c $(CFLAGS) game.c

entity.o: entity.c entity.h
	gcc -c $(CFLAGS) entity.c

components.o: components.c components.h game.h entity.h
	gcc -c $(CFLAGS) components.c

systems.o: systems.c systems.h components.h entity.h
	gcc -c $(CFLAGS) systems.c

clean:
	rm -rf *.o echo_lake
