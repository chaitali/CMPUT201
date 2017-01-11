# name:               Chaitali Patel
# ONE Card number:    1404270
# Unix id:            cpatel1
# lecture section:    B1
# instructor's name:  Jacqueline Smith
# lab section:        H01
# TA's name:          Nicolas Barriga, Shida He
#
# Creates a draw2 file, provides a make clean and tar
#

all: draw2 

draw2: draw2.o memwatch.o draw2.h buffer.o classify.o outsource.o
	gcc -std=c99 -g -Wall -o draw2 draw2.o memwatch.o buffer.o classify.o outsource.o -lm

buffer.o: buffer.c draw2.h memwatch.c
	gcc -std=c99 -g -Wall -c buffer.c -DMEMWATCH -DMW_STDIO -lm

draw2.o: draw2.c memwatch.c draw2.h
	gcc -std=c99 -g -Wall -c draw2.c -DMEMWATCH -DMW_STDIO -lm

classify.o: classify.c memwatch.c draw2.h
	gcc -std=c99 -g -Wall -c classify.c -DMEMWATCH -DMW_STDIO -lm

outsource.o: outsource.c memwatch.c draw2.h
	gcc -std=c99 -g -Wall -c outsource.c -DMEMWATCH -DMW_STDIO -lm

memwatch.o: memwatch.c memwatch.h
	gcc -std=c99 -Wall -c memwatch.c -DMEMWATCH -DMW_STDIO

clean:
	-rm -f *.o draw2
tar:
	tar cvf submit.tar Makefile README draw2.c classify.c buffer.c outsource.c draw2.h
