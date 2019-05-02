CC = gcc
OBJS = commands.o assemble.o 20151623.o
TARGET = 20151623.out

.SUFFIXES : .c .o

all : $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -Wall -g -o  $@ $(OBJS)

clean :
	rm -f $(OBJS) $(TARGET) intermediate location

commands.o : commands.c 20151623.h
assemble.o : assemble.c 20151623.h
20151623.o : 20151623.c 20151623.h
