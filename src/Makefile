# Compiler and flags
CC := gcc
CFLAGS := -Wall -Wextra -Werror -g -std=c99
LIBS := -lz -lwget -lcurl -lpthread `pkg-config --libs gtk+-3.0`

main: main.c

main.c:
	$(CC) $(CFLAGS) ./*/*.c ../*/*.c $(LIBS)
