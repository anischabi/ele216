#include <stdio.h>

#include "probleme1.h"

int main(void) {
    // Tableau de test de 10 entiers, initialisé directement dans le code
    // (contient volontairement deux "23" pour observer le comportement de
    // indice_max en cas d'égalité : elle retourne le PREMIER indice trouvé).
    int tableau[10] = {4, 12, 7, 23, 5, 1, 19, 8, 23, 3};

    printf("Contenu   : ");
    afficher_tableau(tableau, 10);

    printf("Somme     : %d\n", somme_tableau(tableau, 10));

    // On appelle indice_max une seule fois serait plus efficace, mais on le
    // rappelle ici simplement pour afficher à la fois l'indice et la valeur.
    printf("Indice max: %d (valeur %d)\n", indice_max(tableau, 10),
        tableau[indice_max(tableau, 10)]);

    return 0;
}
