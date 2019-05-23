# Files
SRC := $(wildcard *.c)
OBJ := $(patsubst %.c, %.o, $(SRC))
EXEC := $(patsubst %.o, %, $(OBJ))
# Options
CC := gcc
CFLAGS := -I$(HOME)/local/gsl-2.5/include
LDFLAGS := -L$(HOME)/local/gsl-2.5/lib -lgsl -lgslcblas
# Rules
all: $(EXEC)
$(EXEC): %: %.o
	$(CC) $(LDFLAGS) -o $@ $^
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
.PHONY: clean clobber neat
clean: clobber
	$(RM) $(EXEC)
clobber: neat
	$(RM) $(OBJ)
neat:
	$(RM) *~ .*~
