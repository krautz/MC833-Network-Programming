CC := gcc
LDFLAGS += -ljson-c libjson-c.a

all: server client

server:
	$(CC) server.c database.c $(LDFLAGS) -o server.out

client:
	$(CC) client.c -o client.out

clean:
	rm -rf $(TARGET) *.out