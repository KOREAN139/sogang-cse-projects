CC = arm-none-linux-gnueabi-gcc
CFLAGS = -static
TARGET = 20151623
OBJECTS = process.o modes.o main.o

all : $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

clean :
	rm *.o 20151623
