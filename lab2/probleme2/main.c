#include <stdio.h>

#include "probleme2.h"

#define LIGNES 4

int main(void) {
    // Matrice 4x5 initialisée dans le code.
    int matrice[LIGNES][NB_COLONNES] = {
        {1, 2, 3, 4, 5},
        {10, 20, 30, 40, 50},
        {7, 14, 21, 28, 35},
        {100, 2, 300, 4, 5},
    };

    printf("Matrice :\n");
    afficher_matrice(matrice, LIGNES);

    // resultats_lignes a une case par LIGNE (donc taille LIGNES).
    int resultats_lignes[LIGNES];
    sommes_lignes(matrice, LIGNES, resultats_lignes);

    // resultats_colonnes a une case par COLONNE (donc taille NB_COLONNES,
    // pas LIGNES !). C'est un piège classique si on ne fait pas attention.
    int resultats_colonnes[NB_COLONNES];
    sommes_colonnes(matrice, LIGNES, resultats_colonnes);

    printf("\nSommes des lignes   : ");
    for (int i = 0; i < LIGNES; i++) {
        printf("%d ", resultats_lignes[i]);
    }
    printf("\n");

    printf("Sommes des colonnes : ");
    for (int j = 0; j < NB_COLONNES; j++) {
        printf("%d ", resultats_colonnes[j]);
    }
    printf("\n");

    // Démonstration : la version alternative (colonne par colonne) donne
    // exactement le même résultat que sommes_colonnes (voir le tableau
    // comparatif dans probleme2.c).
    int resultats_colonnes_v1[NB_COLONNES];
    sommes_colonnes_par_colonne(matrice, LIGNES, resultats_colonnes_v1);

    printf("Sommes des colonnes (version alternative, pour comparaison) : ");
    for (int j = 0; j < NB_COLONNES; j++) {
        printf("%d ", resultats_colonnes_v1[j]);
    }
    printf("\n");

    return 0;
}
