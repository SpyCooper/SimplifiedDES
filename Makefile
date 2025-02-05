CC = g++
CFLAGS = -Wall
TARGET = simple_des

all: $(TARGET)

$(TARGET): simple_des.o
	$(CC) $(CFLAGS) -o $(TARGET) simple_des.o

simple_des.o: simple_des.cc
	$(CC) $(CFLAGS) -c simple_des.cc

clean:
	rm -f $(TARGET) *.o