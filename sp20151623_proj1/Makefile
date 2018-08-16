CC = gcc
OBJS = 20151623.o
TARGET = 20151623.out

.SUFFIXES : .c .o

all : $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -Wall -o $@ $(OBJS)

clean :
	rm -f $(OBJS) $(TARGET)

20151623.out : 20151623.c 20151623.h
