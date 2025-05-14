CC := gcc

SRCDIR := src
INCDIR := include
BUILDDIR := obj
OUTPUTDIR := bin
TARGET := $(OUTPUTDIR)/app
CFLAGS := -Wall -Werror
MAIN := main.c

all: $(TARGET)

$(TARGET): $(BUILDDIR)/main.o
	$(CC) $(CFLAGS) $^ -o $(TARGET)

$(BUILDDIR)/main.o: $(MAIN)
	$(CC) -c $(MAIN) -o $(BUILDDIR)/main.o

clean:
	@rm $(BUILDDIR)/*.o
	@rm $(TARGET)
