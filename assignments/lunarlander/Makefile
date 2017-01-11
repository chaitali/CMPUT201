# name:               Chaitali Patel
# ONE Card number:    1404270
# Unix id:            cpatel1
# lecture section:    B1
# instructor's name:  Jacqueline Smith
# lab section:        H01
# TA's name:          Nicolas Barriga, Shida He
#
# Creates a lander file, provides a make clean and tar
#

all: lander

lander: improvements.o use_curses.o lander.o draw_landscape.o move_lander.o
	gcc -std=c99 -g -Wall -o lander lander.o use_curses.o draw_landscape.o move_lander.o improvements.o -lm -lcurses

lander.o: lander.c lander.h
	gcc -std=c99 -g -Wall -c lander.c -lm -lcurses

use_curses.o: use_curses.c use_curses.h
	gcc -std=c99 -g -Wall -c use_curses.c -lm -lcurses

draw_landscape.o: draw_landscape.c lander.h
	gcc -std=c99 -g -Wall -c draw_landscape.c -lm -lcurses

improvements.o: improvements.c lander.h
	gcc -std=c99 -g -Wall -c improvements.c -lm -lcurses

move_lander.o: move_lander.c lander.h
	gcc -std=c99 -g -Wall -c move_lander.c -lm -lcurses

clean:
	-rm -f *.o lander

tar:
	tar cvf submit.tar Makefile README lander.c move_lander.c improvements.c draw_landscape.c use_curses.c lander.h use_curses.h
