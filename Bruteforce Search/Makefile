CC := gcc

SRCDIR := src
INCDIR := include
BUILDDIR := obj
OUTPUTDIR := bin
TARGET := $(OUTPUTDIR)/app
CFLAGS := -std=c2x -Wall -I $(INCDIR) -Werror -pedantic
MAIN := main.c

all: $(TARGET)

SRC := $(wildcard $(SRCDIR)/*.c)
OBJ := $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SRC))

$(TARGET): $(OBJ) $(BUILDDIR)/main.o | $(OUTPUTDIR)
	$(CC) $(CFLAGS) $^ -o $@

$(BUILDDIR)/main.o: $(MAIN) | $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm $(BUILDDIR)/*.o
	@rm $(TARGET)
