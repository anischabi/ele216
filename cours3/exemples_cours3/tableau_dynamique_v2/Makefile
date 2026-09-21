NAME    = tabdyn_test
CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -g
LDLIBS  = -lm
LDFLAGS =

# Détection de l'OS pour adapter les commandes de nettoyage
ifeq ($(OS),Windows_NT)
    RM = del /q /f
    EXE = $(NAME).exe
else
    RM = rm -f
    EXE = $(NAME)
endif

# Définition des listes de fichiers 
SRC = $(NAME).c tabdyn.c
OBJ = $(SRC:.c=.o)
DEP = $(OBJ:.o=.d)

# Inclusion automatique des fichiers de dépendances (.d) s'ils existent
-include $(DEP)

.PHONY: all clean run distclean

# Cible par défaut
all: $(NAME)

# Étape d'édition de lien
$(NAME): $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

# Étape de compilation
%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# Lance le programme compilé
run: all
	./$(EXE)

# Élimine les produits de compilation intermédiaires (.o, .d)
clean:
	$(RM) $(OBJ) $(DEP)

# Élimine tous les produits de compilation et l'exécutable
distclean: clean
	$(RM) $(EXE)