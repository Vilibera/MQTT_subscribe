#This is src Makefile
BIN:=mqtt_program
LDFLAGS:=-lmosquitto -largp -luci -ljson-c -lcurl
CPPFLAGS:=-I$(CURDIR)
SRC:=$(wildcard *.c)
OBJS:=$(SRC:.c=.o)

.PHONY: all clean

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) $(BIN) $(OBJS)





