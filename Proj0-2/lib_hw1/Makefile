CC = gcc
CFLAGS = -g
OBJECTS = main.o list.o hash.o hex_dump.o debug.o bitmap.o
TARGET = testlib

all: $(TARGET)

main.o: main.c list.h hash.h bitmap.h 
	$(CC) -c main.c $(CFLAGS)

list.o: list.c list.h
	$(CC) -c list.c $(CFLAGS)

hash.o: hash.c hash.h
	$(CC) -c hash.c $(CFLAGS)

hex_dump.o: hex_dump.c hex_dump.h
	$(CC) -c hex_dump.c $(CFLAGS)

debug.o: debug.c debug.h
	$(CC) -c debug.c $(CFLAGS)

bitmap.o: bitmap.c bitmap.h limits.h round.h hex_dump.h
	$(CC) -c bitmap.c $(CFLAGS)


$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS) $(CFLAGS)

clean:
	rm -rf $(OBJECTS) $(TARGET)
