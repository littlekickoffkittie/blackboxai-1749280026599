CC = gcc
CFLAGS = -Wall -Wextra -O2
LDFLAGS = -lcrypto -lm
TARGET = chrysalis
SRCS = chrysalis.c crypto.c qrcode.c
OBJS = $(SRCS:.c=.o)
DEPS = crypto.h qrcode.h

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

install: $(TARGET)
	mkdir -p /usr/local/bin
	cp $(TARGET) /usr/local/bin/
	mkdir -p /usr/local/include/chrysalis
	cp *.h /usr/local/include/chrysalis/

uninstall:
	rm -f /usr/local/bin/$(TARGET)
	rm -rf /usr/local/include/chrysalis

.PHONY: all clean install uninstall
