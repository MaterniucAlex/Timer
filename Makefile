files = src/main.c src/textRenderer/textRenderer.c
libs = -lSDL3 -lSDL3_image -lSDL3_mixer -lX11 -lm

all:
	@clear
	gcc -o Timer $(files) -Llib -Iinclude $(libs)

