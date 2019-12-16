
.PHONY: clean

RM = rm -f

rhythm: rhythm.o socklib.o gui.o
	gcc -o rhythm rhythm.o socklib.o gui.o -lpthread -lncursesw

rhythm.o: rhythm.c socklib.h gui.h
	gcc -Wall -g -c rhythm.c

socklib.o: socklib.c socklib.h
	gcc -Wall -g -c socklib.c

gui.o: gui.c gui.h
	gcc -Wall -g -c gui.c

clean:
	$(RM) *.o
