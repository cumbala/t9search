TARGET=t9search
TARGET_REC=t9search_rec
COMPILER=gcc
COMPILE=$(COMPILER) -std=c99 -Wall -Wextra -Werror $(TARGET).c -o $(TARGET)

default: all

all:
	$(COMPILER) -std=c99 -Wall -Wextra -Werror $(TARGET).c -o $(TARGET)

rec:
	$(COMPILER) -std=c99 -Wall -Wextra -Werror $(TARGET).c -o $(TARGET_REC)

run:
	$(COMPILE) && ./$(TARGET) < seznam.txt

run-search:
	$(COMPILE) && ./$(TARGET) 264 < seznam.txt
