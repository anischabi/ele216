#include <stdio.h>
#include <stdlib.h>

#include "probleme4.h"

// Réutilisation directe de afficher_tableau du problème 1, comme le permet
// l'énoncé ("Il est permis de réutiliser certaines fonctions réalisées dans
// les numéros précédents"). Pas de copie du fichier : on inclut le .h
// directement depuis le dossier voisin.
#include "../probleme1/probleme1.h"

#define TAILLE 8

int main(void) {
    int *tableau = creer_tableau(TAILLE);
    if (tableau == NULL) {
        fprintf(stderr, "Erreur : allocation échouée.\n");
        return 1;
    }

    afficher_tableau(tableau, TAILLE);

    // Le tableau vit sur le tas (alloué par malloc dans creer_tableau) :
    // c'est à l'appelant (ici, main) de le libérer explicitement. L'oublier
    // ne cause pas de crash visible, mais une fuite mémoire.
    free(tableau);

    return 0;
}
