#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "probleme5.h"

#define LIGNES 5
#define COLONNES 8

int main(void) {
    // srand avec l'heure courante : sans ça, rand() produirait toujours la
    // même séquence de valeurs à chaque exécution du programme.
    srand((unsigned int) time(NULL));

    int **matrice = creer_matrice(LIGNES, COLONNES);
    if (matrice == NULL) {
        fprintf(stderr, "Erreur : allocation échouée.\n");
        return 1;
    }

    remplir_matrice_aleatoire(matrice, LIGNES, COLONNES, 20, 80);

    printf("Matrice %dx%d (valeurs aleatoires entre 20 et 80) :\n", LIGNES, COLONNES);
    afficher_matrice_dyn(matrice, LIGNES, COLONNES);

    liberer_matrice(matrice, LIGNES);

    return 0;
}
