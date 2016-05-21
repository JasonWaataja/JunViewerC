CC ?= gcc
PKGCONFIG = $(shell which pkg-config)
CFLAGS = $(shell $(PKGCONFIG) --cflags gtk+-3.0)
LIBS = $(shell $(PKGCONFIG) --libs gtk+-3.0)
EXEC=jvc

SRC = main.c

OBJS = $(SRC:.c=.o)

all: $(EXEC)

%.o: %.c
	$(CC) -c -o $(@F) $(CFLAGS) $<

$(EXEC): $(OBJS)
	$(CC) -o $(@F) $(LIBS) $(OBJS)

clean:
	rm -f $(OBJS)
	rm -f $(EXEC)
