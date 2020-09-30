CC=gcc
INC=-I./include
CFLAGS=-g -Wall $(INC)
OBJS=daemon.o
TARGET=cgleed

all: $(TARGET)

clean:
	rm -f *.o
	rm -f $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS)
