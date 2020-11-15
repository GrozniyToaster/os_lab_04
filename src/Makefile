CC = gcc
CCFLAGS =  -O2  -Wall   -pedantic
NAME_PARENT = os_lab4_parent
NAME_CHILD = os_lab4_child

all: settings.h
	$(CC) $(CCFLAGS) parent.c -o $(NAME_PARENT) 
	$(CC) $(CCFLAGS) child.c -o $(NAME_CHILD) 
debug: settings.h
	$(CC) $(CCFLAGS) -g3 parent.c -o $(NAME_PARENT) 
	$(CC) $(CCFLAGS) -g3 child.c -o $(NAME_CHILD) 
clean:
	rm -f *.o $(NAME_PARENT) $(NAME_CHILD) 