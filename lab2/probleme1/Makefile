NAME    = probleme1
CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -g
LDLIBS  = -lm
LDFLAGS =

SRC = main.c probleme1.c
OBJ = $(SRC:.c=.o)
DEP = $(OBJ:.o=.d)

.PHONY: all clean run distclean

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

run: all
	./$(NAME)

clean:
	rm -f $(OBJ) $(DEP)

distclean: clean
	rm -f $(NAME)
