CC = gcc
CFLAGS = -g -Wall
OBJECTS = main.o list.o hash.o hex_dump.o debug.o bitmap.o
TARGET = testlib

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS) $(CFLAGS)

main.o: main.c list.h hash.h bitmap.h 
	$(CC) $(CFLAGS) -c -o main.o main.c 

list.o: list.c list.h
	$(CC) $(CFLAGS) -c -o list.o list.c

hash.o: hash.c hash.h
	$(CC) $(CFLAGS) -c -o hash.o hash.c

bitmap.o: bitmap.c bitmap.h limits.h round.h hex_dump.h
	$(CC) $(CFLAGS) -c -o bitmap.o bitmap.c

hex_dump.o: hex_dump.c hex_dump.h
	$(CC) $(CFLAGS) -c -o hex_dump.o hex_dump.c 

debug.o: debug.c debug.h
	$(CC) $(CFLAGS) -c -o debug.o debug.c 

clean:
	rm -rf *.o
	rm -rf $(TARGET)
