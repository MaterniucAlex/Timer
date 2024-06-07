all:
	gcc -o app main.c -Llib -Iinclude -l SDL3 -l X11
