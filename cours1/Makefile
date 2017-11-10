#
##

CC	=	gcc 
CFLAGS	=  	-Wall
CFLAGS  += 	-DDEBUG -D_GNU_SOURCE
CFLAGS  += 	-g

LIBS    =       -lm
CIBLES	=	 enumeration

all: $(CIBLES)

enumeration: main.o
	$(CC) $(CFLAGS) -o $@ main.o  $(LIBS)

main.o: main.c
		$(CC) $(CFLAGS) -c main.c
