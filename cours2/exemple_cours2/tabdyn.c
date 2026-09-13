#include "tabdyn.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static bool agrandir(tabdyn_t* tabdyn);

tabdyn_t *tabdyn_creer(int taille_initiale) {
    tabdyn_t *tabdyn = malloc(sizeof(tabdyn_t));
    if (tabdyn == NULL) { return NULL; }

    int* contenu_initial = malloc(taille_initiale * sizeof(int));
    if (contenu_initial == NULL) { 
        free(tabdyn);
        return NULL; 
    }

    tabdyn->capacite = taille_initiale;
    tabdyn->taille = 0;
    tabdyn->contenu = contenu_initial;
    return tabdyn;
}

int tabdyn_ajouter(tabdyn_t* tabdyn, int valeur) {
    assert(tabdyn != NULL);
    if (tabdyn->taille == tabdyn->capacite) {
        if(agrandir(tabdyn) == false) {return 0;}
    }
    tabdyn->contenu[tabdyn->taille] = valeur;
    tabdyn->taille++;
    return tabdyn->taille;
}

/**
 * Agrandis le tableau pour permettre l'insertion. Double la taille du
 * stockage alloué au tableau. Si l'agrandissement est impossible, cette
 * fonction n'a pas d'effet sur le contenu du tableau.
 *
 * @param tabdyn pointeur non-nul vers le tableau dynamique.
 * @return true si l'agrandissement est effectué avec succès, false sinon.
 */
static bool agrandir(tabdyn_t* tabdyn) {
    assert(tabdyn != NULL);
    int *nouveau = realloc(tabdyn->contenu, tabdyn->capacite * sizeof(int) * 2);
    if (nouveau == NULL) { return false; }
    tabdyn->contenu = nouveau;
    tabdyn->capacite *= 2;
    return true;
}

void tabdyn_liberer(tabdyn_t* tabdyn) {
    free(tabdyn->contenu);
    free(tabdyn);
}

void tabdyn_afficher(tabdyn_t* tabdyn) {
    assert(tabdyn != NULL);
    printf("taille : %d, nb_elements : %d\n", tabdyn->capacite, tabdyn->taille);
    printf("{ ");
    for (int i = 0; i < tabdyn->taille; i++) {
        printf("%d, ", tabdyn->contenu[i]);
    }
    printf(" }\n");
}

