CC = gcc
CFLAGS = -Wall -O2
LIBS = -lSDL2 -lSDL2_image -lSDL2_mixer

SRCS = second_try.c mainMenu.c common.c inventory.c
OBJS = $(SRCS:.c=.o)
HEADERS = mainMenu.h common.h inventory.h

TARGET = sdl_example

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

