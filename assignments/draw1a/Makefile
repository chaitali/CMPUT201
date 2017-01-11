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

draw1a: draw1a.c
	gcc -Wall -W -std=c99 draw1a.c -o draw1a
clean:
	-rm *.o draw1a
tar:
	tar cvf submit.tar draw1a.c Makefile README
