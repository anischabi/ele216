#include "compteur.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// Définition de la structure dans le .c : les champs sont invisibles de
// l'extérieur du module. Comme aucune fonction ne modifie seuil après
// compteur_creer, ce champ est immuable. Seules compteur_incrementer et
// compteur_reinitialiser modifient valeur, et elles maintiennent
// l'invariant 0 <= valeur < seuil.
struct compteur {
    int valeur;             // toujours dans [0, seuil - 1]
    int seuil;              // > 0, fixé à la création
    int depassements;       // nombre de tours complétés depuis la création
};

compteur_t *compteur_creer(int seuil) {
    if (seuil <= 0) { return NULL; }

    compteur_t *c = malloc(sizeof(compteur_t));
    if (c == NULL) { return NULL; }

    c->valeur = 0;
    c->seuil = seuil;
    c->depassements = 0;
    return c;
}

void compteur_detruire(compteur_t *c) {
    free(c);    // free(NULL) est sans effet
}

void compteur_incrementer(compteur_t *c) {
    assert(c != NULL);
    c->valeur++;
    if (c->valeur >= c->seuil) {
        c->depassements++;
        c->valeur = 0;
    }
}

void compteur_reinitialiser(compteur_t *c) {
    assert(c != NULL);
    c->valeur = 0;
}

int compteur_valeur(const compteur_t *c) {
    assert(c != NULL);
    return c->valeur;
}

int compteur_seuil(const compteur_t *c) {
    assert(c != NULL);
    return c->seuil;
}

int compteur_depassements(const compteur_t *c) {
    assert(c != NULL);
    return c->depassements;
}

void compteur_afficher(const compteur_t *c) {
    assert(c != NULL);
    printf("Compteur { valeur = %d, seuil = %d, depassements = %d }\n",
           c->valeur, c->seuil, c->depassements);
}
