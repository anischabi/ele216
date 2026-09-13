#include <stdio.h>

#include "probleme1.h"

int somme_tableau(int tab[], int n) {
    int somme = 0;
    // On accumule chaque élément dans "somme" au fil de la boucle.
    for (int i = 0; i < n; i++) {
        somme += tab[i];
    }
    return somme;
}

int indice_max(int tab[], int n) {
    // On suppose d'abord que le premier élément est le maximum
    int indice = 0;
    // On compare avec le reste du tableau (inutile de comparer
    // l'élément 0 avec lui-même, donc on part de i = 1).
    for (int i = 1; i < n; i++) {
        // ">" strict : en cas d'égalité, on garde le PREMIER indice trouvé
        // (un ">=" garderait plutôt le dernier).
        if (tab[i] > tab[indice]) {
            indice = i;
        }
    }
    return indice;
}

void afficher_tableau(int tab[], int n) {
    for (int i = 0; i < n; i++) {
        // On imprime un espace AVANT chaque élément, sauf le premier
        // (i == 0). Ça place les espaces ENTRE les nombres, sans jamais en
        // laisser un après le dernier élément.
        if (i > 0) {
            printf(" ");
        }
        printf("%d", tab[i]);
    }
    printf("\n");
}
