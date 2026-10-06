NAME     = sequenceur_test
CC       = gcc
CFLAGS   = -Wall -Wextra -std=c11 -g
INCLUDES = -I ../unity -I ../commun -I ../probleme1 -I ../probleme2
LDLIBS   = -lm
LDFLAGS  =

# Détection du shell pour adapter les commandes de nettoyage.
# Tester seulement $(OS) ne suffit pas : sous Windows, mingw32-make utilise
# sh.exe s'il en trouve un dans le PATH (ex. lancé depuis Git Bash), sinon cmd.
# cmd affiche les guillemets de echo alors que sh les retire.
ifeq ($(shell echo "test"),"test")
    RM  = del /q /f
    FIX = $(subst /,\,$1)
else
    RM  = rm -f
    FIX = $1
endif

# gcc ajoute .exe au nom de l'exécutable sous Windows, peu importe le shell.
ifeq ($(OS),Windows_NT)
    EXE = $(NAME).exe
else
    EXE = $(NAME)
endif

# Définition des listes de fichiers
# (SRC = liste des sources, OBJ = liste des objets, DEP = liste des fichiers de dépendance)
# Unity est partagé entre les problèmes (../unity), le Vecteur, la Pile et la
# File du cours sont dans ../commun (exigé par l'énoncé), resultat vient du
# problème 1 et journal du problème 2.
SRC = $(NAME).c sequenceur.c ../probleme2/journal.c ../probleme1/resultat.c \
      ../commun/vecteur_gen.c ../commun/pile_gen.c ../commun/file_gen.c \
      ../unity/unity.c
OBJ = $(SRC:.c=.o)
DEP = $(OBJ:.o=.d)

# Le target .PHONY est utilisé pour indiquer à make que ces termes ne sont pas des
# fichiers sur le disque. Empeche la collision avec un fichier appellé 'run' par exemple
.PHONY: all clean run distclean

# Le target all est le 'top-level' qui assemble tout le binaire
# C'est la cible par défaut, entrer seulement 'make' est équivalent de 'make all'
all: $(NAME)

# Étape d'édition de lien. Assemble tous les .o en un seul exécutable à l'aide du linker
# ($^ :  liste des prérequis, par exemple (sequenceur_test.o sequenceur.o journal.o ... unity.o))
# ($@ : nom du fichier executable cible, par exemple "sequenceur_test")
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
# (FIX convertit les / en \ pour del, qui refuse ../unity/unity.o)
clean:
	$(RM) $(call FIX,$(OBJ) $(DEP))

# Élimine tous les produits de compilation et l'exécutable, laissant le source intact
distclean: clean
	$(RM) $(EXE)

# Inclusion des fichiers de dépendance
-include $(DEP)
