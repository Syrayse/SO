IDIR=../includes

CC=gcc
CFLAGS = -Wall -g -Ofast -I$(IDIR)

DEPS=$(IDIR)/$(wildcard *.h)

SOURCES_OBJ=Common.o Request.o LogManager.o

%.o : %.c $(DEPS)
		$(CC) $(CFLAGS) -c -o $@ $<

instal: pipes client server

server: Servidor.o $(SOURCES_OBJ)
	$(CC) $(CFLAGS) Servidor.o $(SOURCES_OBJ) -o server

client: Cliente.o $(SOURCES_OBJ)
	$(CC) $(CFLAGS) Cliente.o $(SOURCES_OBJ) -o argus


pipes: pipes.o $(SOURCES_OBJ)
	$(CC) $(CFLAGS) pipes.o $(SOURCES_OBJ) -o pipes

clean:
	rm -rf *.o
	rm -rf server
	rm -rf cl2srpipe
	rm -rf sr2clpipe
	rm -rf pipes
	rm -rf argus
	rm -rf client
