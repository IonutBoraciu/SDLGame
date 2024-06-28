CC = gcc
CFLAGS = -Wall -O2
LIBS = -lSDL2 -lSDL2_image

SRCS = second_try.c mainMenu.c common.c
OBJS = $(SRCS:.c=.o)
HEADERS = mainMenu.h common.h

TARGET = sdl_example

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

