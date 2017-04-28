PROGRAMS = libnetclient
CC = gcc

libnetfiles: libnetfiles.o test_client.o
	$(CC) -o libnetclient libnetfiles.o test_client.o
	 
.PHONY: all clean

all: $(PROGRAMS)

clean:
	@rm -f *.o $(PROGRAMS)
