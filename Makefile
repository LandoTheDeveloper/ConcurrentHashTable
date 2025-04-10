# Makefile constructed by ChatGPT with prompt
# "please make Makefile, here are all file names: chash.c hash_table.c read_write_lock.c"

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g

# Sources and objects
SRCS = chash.c hash_table.c read_write_lock.c
OBJS = $(SRCS:.c=.o)

# Executable name
TARGET = chash

# Default rule
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

# Clean rule
clean:
	rm -f $(OBJS) $(TARGET)
