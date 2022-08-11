FeuerzWm: Main.o
	gcc Main.o -o FeuerzWM -I/usr/include/X11 -L/usr/lib/ -Wall -Wl,--allow-shlib-undefined -lX11 -lm

Main.o: Main.c
	gcc -c Main.c

clean: 
	rm *.o
	