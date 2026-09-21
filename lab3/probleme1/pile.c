#include "pile.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// Définition de la structure dans le .c : les champs sont invisibles de
// l'extérieur du module. Comme aucune fonction ne modifie capacite_max et
// tension après pile_creer, ces deux champs sont immuables. Seules
// pile_charger et pile_decharger modifient charge, et elles maintiennent
// l'invariant 0 <= charge <= capacite_max.
struct pile {
    float capacite_max;     // en mAh, fixée à la création
    float charge;           // en mAh, toujours dans [0, capacite_max]
    float tension;          // en V, fixée à la création
};

pile_t *pile_creer(float capacite_mAh, float tension_V) {
    // Strictement positif : une capacité nulle causerait une division par zéro
    // dans pile_pourcentage.
    if (capacite_mAh <= 0.0f || tension_V <= 0.0f) { return NULL; }

    pile_t *p = malloc(sizeof(pile_t));
    if (p == NULL) { return NULL; }

    p->capacite_max = capacite_mAh;
    p->charge = 0.0f;
    p->tension = tension_V;
    return p;
}

void pile_detruire(pile_t *p) {
    free(p);    // free(NULL) est sans effet
}

void pile_charger(pile_t *p, float mAh) {
    assert(p != NULL);
    if (mAh <= 0.0f) { return; }

    p->charge += mAh;
    if (p->charge > p->capacite_max) {
        p->charge = p->capacite_max;
    }
}

void pile_decharger(pile_t *p, float mAh) {
    assert(p != NULL);
    if (mAh <= 0.0f) { return; }

    p->charge -= mAh;
    if (p->charge < 0.0f) {
        p->charge = 0.0f;
    }
}

float pile_capacite_max(const pile_t *p) {
    assert(p != NULL);
    return p->capacite_max;
}

float pile_charge_courante(const pile_t *p) {
    assert(p != NULL);
    return p->charge;
}

float pile_tension(const pile_t *p) {
    assert(p != NULL);
    return p->tension;
}

float pile_pourcentage(const pile_t *p) {
    assert(p != NULL);
    return p->charge / p->capacite_max * 100.0f;
}

void pile_afficher(const pile_t *p) {
    assert(p != NULL);
    printf("Pile { capacite_max = %.1f mAh, charge = %.1f mAh, tension = %.2f V }\n",
           p->capacite_max, p->charge, p->tension);
}
