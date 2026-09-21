NAME     = filtre_test
CC       = gcc
CFLAGS   = -Wall -Wextra -std=c11 -g
INCLUDES = -I ../unity
LDLIBS   = -lm
LDFLAGS  =

# Définition des listes de fichiers
# (SRC = liste des sources, OBJ = liste des objets, DEP = liste des fichiers de dépendance)
# Unity est partagé entre les problèmes du laboratoire (dossier ../unity).
SRC = $(NAME).c filtre.c ../unity/unity.c
OBJ = $(SRC:.c=.o)
DEP = $(OBJ:.o=.d)

# Le target .PHONY est utilisé pour indiquer à make que ces termes ne sont pas des
# fichiers sur le disque. Empeche la collision avec un fichier appellé 'run' par exemple
.PHONY: all clean run distclean

# Le target all est le 'top-level' qui assemble tout le binaire
# C'est la cible par défaut, entrer seulement 'make' est équivalent de 'make all'
all: $(NAME)

# Étape d'édition de lien. Assemble tous les .o en un seul exécutable à l'aide du linker
# ($^ :  liste des prérequis, par exemple (filtre_test.o filtre.o unity.o))
# ($@ : nom du fichier executable cible, par exemple "filtre_test")
$(NAME): $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

# Étape de compilation. Génère un .o pour chaque .c
# -MMD -MP génère un fichier de dépendance .d pour chaque .o,
# pour que l'édition d'un .h entraîne une recompilation de tous les fichiers qui l'utilisent.
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

# Lance les tests
run: all
	./$(NAME)

# Élimine les produits de compilation intermédiaire (.o, .d)
# conserve l'exécutable produit.
clean:
	rm -f $(OBJ) $(DEP)

# Élimine tous les produits de compilation et l'exécutable, laissant le source intact
distclean: clean
	rm -f $(NAME)

# Inclusion des fichiers de dépendance
-include $(DEP)
