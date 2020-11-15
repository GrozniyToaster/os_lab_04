CC = gcc
CCFLAGS =  -O2  -Wall   -pedantic
LIBS =  -lrt -pthread

NAME_PARENT = os_lab4_parent
NAME_CHILD = os_lab4_child

all: settings.h
	$(CC) $(CCFLAGS) parent.c -o $(NAME_PARENT) $(LIBS)
	$(CC) $(CCFLAGS) child.c -o $(NAME_CHILD) $(LIBS)
debug: settings.h
	$(CC) $(CCFLAGS) -g3 parent.c -o $(NAME_PARENT) $(LIBS)
	$(CC) $(CCFLAGS) -g3 child.c -o $(NAME_CHILD) $(LIBS)
clean:
	rm -f *.o $(NAME_PARENT) $(NAME_CHILD) 
