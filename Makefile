CC=gcc
CFLAGS=-Wall -g -Werror

INCLUDE=/usr/local/include
LIB=/usr/local/lib
LDFLAGS=-lraylib -lm

BIN=run

all: $(BIN)

$(BIN): main.c
	$(CC) $(CFLAGS) -I$(INCLUDE) $^ -o $@ -L$(LIB) $(LDFLAGS)

clean:
	$(RM) $(BIN) $(OBJ)

