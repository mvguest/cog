all:
	gcc main.c -o	cog

install:
	gcc main.c -o cog
	mv cog /usr/local/bin/ 

uninstall:
	sudo rm -rf /usr/local/bin/cog
