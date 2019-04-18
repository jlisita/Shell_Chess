CC=gcc
CFLAGS=-I$(HEADDIR) -W -Wall
LDFLAGS=
SRCDIR=Src
HEADDIR=Includes
SRC= $(wildcard $(SRCDIR)/*.c)
OBJ= $(SRC:$(SRCDIR)/%.c=$(SRCDIR)/%.o)
EXEC=prog

all: $(EXEC)

prog: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

$(SRCDIR)/main.o: $(HEADDIR)/game.h

$(SRCDIR)/game.o: $(HEADDIR)/board.h

$(SRCDIR)/board.o: $(HEADDIR)/piece.h

$(SRCDIR)/player.o: $(HEADDIR)/piece.h

$(SRCDIR)/%.o: $(SRCDIR)/%.c $(HEADDIR)/%.h
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf $(SRCDIR)/*.o

mrproper: clean
	rm -rf $(EXEC)

