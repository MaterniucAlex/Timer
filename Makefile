all:
	@clear
	gcc -o app main.c -Llib -Iinclude -lSDL3 -lX11 -lm
