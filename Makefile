TARGET=t9search

default: all

all:
	gcc -std=c99 -Wall -Wextra -Werror $(TARGET).c -o $(TARGET)
