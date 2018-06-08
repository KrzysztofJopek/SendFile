CC = gcc
PR =

all: sendfile

sendfile: send.o connect.o connect.h md5calc.o receive.o
	$(CC) $(PR) send.o connect.o md5calc.o receive.o sendfile.c -o sendfile -lssl -lcrypto 

send.o: send.c
	$(CC) $(PR) -c send.c -o send.o

receive.o: receive.c
	$(CC) $(PR) -c receive.c -o receive.o

connect.o: connect.c
	$(CC) $(PR) -c connect.c -o connect.o

md5calc.o: md5calc.c
	$(CC) $(PR) -Wall -c md5calc.c -o md5calc.o 

clean:
	rm *.o sendfile
