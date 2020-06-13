IDIR=../includes

CC=gcc
CFLAGS = -Wall -g -Ofast -I$(IDIR)

DEPS=$(IDIR)/$(wildcard *.h)

SOURCES_OBJ=Common.o Request.o LogManager.o HashTable.o Response.o Language.o

%.o : %.c $(DEPS)
		$(CC) $(CFLAGS) -c -o $@ $<

all: pipes client server
	./pipes

server: argusd.o $(SOURCES_OBJ)
	$(CC) $(CFLAGS) argusd.o $(SOURCES_OBJ) -o argusd

client: argus.o $(SOURCES_OBJ)
	$(CC) $(CFLAGS) argus.o $(SOURCES_OBJ) -o argus

pipes: pipes.o $(SOURCES_OBJ)
	$(CC) $(CFLAGS) pipes.o $(SOURCES_OBJ) -o pipes

clean:
	rm -rf *.o
	rm -rf argusd
	rm -rf cl2srpipe
	rm -rf sr2clpipe
	rm -rf pipes
	rm -rf argus
	rm -rf log
	rm -rf log.idx
	rm -rf command_log
