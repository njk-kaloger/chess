# Nicholas Kaloger 2017
# Define the C compiler
CC      = gcc

CFLAGS  = -Wall -Wextra -Wpedantic -g

# Define the source, header and object files
HDR		= game.h
SRC		= main.c
OBJ     = $(SRC:.c=.o)

# The executable name
TARGET	= chess

$(TARGET): $(OBJ) Makefile
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

.PHONY: clean
clean:
	rm -f $(OBJ)

$(OBJ): $(HDR)