CC = gcc

CFLAGS = -Wall -Wextra -std=c11 -MMD -MP
TARGET = enigma_m3

SOURCES = main.c enigma.c
OBJECTS = $(SOURCES:.c=.o)
DEPENDS = $(OBJECTS:.o=.d)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPENDS)

clean:
	rm -f $(TARGET) $(OBJECTS) $(DEPENDS)
