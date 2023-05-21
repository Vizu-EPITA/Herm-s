# Compiler and flags
CC := gcc
CFLAGS := -Wall -Wextra -Werror -g -std=c99
LIBS := -lz -lwget -lcurl -lpthread `pkg-config --libs gtk+-3.0`

# Directories
SRCDIR := src
INCDIR := include
TOOLSDIR := tools

# Source files
CRAWLERSRC := $(wildcard $(SRCDIR)/crawler/*.c)
INDEXERSRC := $(wildcard $(SRCDIR)/indexer/*.c)
SEARCHERSRC := $(wildcard $(SRCDIR)/searcher/*.c)
UISRC := $(wildcard $(SRCDIR)/user_interface/*.c)
TOOLSSRC := $(wildcard $(TOOLSDIR)/*.c)

# Object files
CRAWLEROBJ := $(CRAWLERSRC:.c=.o)
INDEXEROBJ := $(INDEXERSRC:.c=.o)
SEARCHEROBJ := $(SEARCHERSRC:.c=.o)
UIOBJ := $(UISRC:.c=.o)
TOOLSOBJ := $(TOOLSSRC:.c=.o)

# Targets
EXECUTABLE := main

all: $(EXECUTABLE)

$(EXECUTABLE): $(CRAWLEROBJ) $(INDEXEROBJ) $(SEARCHEROBJ) $(UIOBJ) $(TOOLSOBJ) $(SRCDIR)/main.o
	$(CC) $(CFLAGS) -I$(INCDIR) $^ -o $@ $(LIBS)

# Compile C files
$(CRAWLEROBJ): %.o: %.c
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

$(INDEXEROBJ): %.o: %.c
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

$(SEARCHEROBJ): %.o: %.c
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

$(UIOBJ): %.o: %.c
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

$(TOOLSOBJ): %.o: $(TOOLSDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SRCDIR)/main.o: $(SRCDIR)/main.c
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

clean:
	rm -f $(EXECUTABLE) $(CRAWLEROBJ) $(INDEXEROBJ) $(SEARCHEROBJ) $(UIOBJ) $(TOOLSOBJ) $(SRCDIR)/main.o

