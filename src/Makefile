main: main.c ../tools/*.c ./crawler/*.c ./indexer/*.c ./searcher/*.c ./user_interface/*.c

main.c:
	gcc -Wall -Wextra -Werror -g main.c ../tools/*.c ./crawler/*.c ./indexer/*.c ./searcher/*.c ./user_interface/*.c -o main -lz -lwget -lcurl -lpthread -std=c99 `pkg-config --cflags --libs gtk+-3.0`
