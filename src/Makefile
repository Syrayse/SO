IDIR=../includes

CC=gcc
CFLAGS = -Wall -g -Ofast -I$(IDIR)

DEPS=$(IDIR)/$(wildcard *.h)
SOURCES=$(wildcard *.c)
SOURCES_OBJ=$(patsubst %.c,%.o,$(SOURCES))

%.o : %.c $(DEPS)
		$(CC) $(CFLAGS) -c -o $@ $<

server: $(SOURCES_OBJ)
	$(CC) $(CFLAGS) $(SOURCES_OBJ) -o server

print-% : ; @echo $* = $($*)

clean:
	rm -rf *.o
	rm -rf server
