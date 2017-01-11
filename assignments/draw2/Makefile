# name:               Chaitali Patel
# ONE Card number:    1404270
# Unix id:            cpatel1
# lecture section:    B1
# instructor's name:  Jacqueline Smith
# lab section:        H01
# TA's name:          Nicolas Barriga, Shida He
#
# Creates a draw1a file, provides a make clean and tar
#

draw1b: draw1b.c
	gcc -Wall -W -std=c99 draw1b.c -o draw1b -lm
clean:
	-rm *.o draw1b
tar:
	tar cvf submit.tar draw1b.c draw1b.h  Makefile README 
