INCLUDE=../include/

CFLAGS=-Wall -Wextra -Werror -pedantic -g -std=c11 -fms-extensions -D_GNU_SOURCE -lSDL2_ttf -lSDL2_image -lSDL2_gfx -I$(INCLUDE) `sdl2-config --libs --cflags` `pkg-config --libs --cflags jansson`

APP=../echo_lake

OBJECTS=echo_lake.o player.o game.o components.o entity.o systems.o tmx.o world.o

all: $(OBJECTS)
	gcc -o $(APP) $(CFLAGS) $(OBJECTS)

echo_lake.o: echo_lake.c $(INCLUDE)game.h $(INCLUDE)entity.h $(INCLUDE)components.h $(INCLUDE)systems.h $(INCLUDE)world.h $(INCLUDE)tmx.h
	gcc -c $(CFLAGS) echo_lake.c

player.o: player.c $(INCLUDE)components.h
	gcc -c $(CFLAGS) player.c

game.o: game.c $(INCLUDE)game.h $(INCLUDE)systems.h $(INCLUDE)components.h
	gcc -c $(CFLAGS) game.c

entity.o: entity.c $(INCLUDE)entity.h
	gcc -c $(CFLAGS) entity.c

components.o: components.c $(INCLUDE)components.h $(INCLUDE)game.h $(INCLUDE)entity.h
	gcc -c $(CFLAGS) components.c

systems.o: systems.c $(INCLUDE)systems.h $(INCLUDE)components.h $(INCLUDE)entity.h
	gcc -c $(CFLAGS) systems.c

tmx.o: tmx.c $(INCLUDE)tmx.h
	gcc -c $(CFLAGS) tmx.c

world.o: world.c

clean:
	rm -rf *.o echo_lake
