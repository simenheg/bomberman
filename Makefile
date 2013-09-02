CC      = cc
CFLAGS  = $(shell sdl-config --cflags) $(shell guile-config compile) \
	  -std=c99 -g3
LDFLAGS = $(shell sdl-config --libs) -lSDL_image -lopenal -lvorbisfile \
	  $(shell guile-config link)

OBJECTS = main.o game.o video.o gfx.o script.o audio.o input.o player.o map.o

main     : $(OBJECTS)
main.o   : common.h main.h game.h
game.o   : common.h game.h video.h gfx.h script.h audio.h input.h map.h
video.o  : common.h gfx.h game.h player.h
gfx.o    : common.h gfx.h
script.o : common.h script.h player.h
audio.o  : common.h audio.h
input.o  : common.h input.h
player.o : common.h player.h gfx.h script.h input.h game.h
map.o    : common.h map.h gfx.h script.h

clean :
	rm -f main $(OBJECTS)
