#include "pile_gen.h"
#include "vecteur_gen.h"

#include <assert.h>
#include <stdlib.h>

struct pile {
    vecteur_t *vect;
};

pile_t *pile_creer() {
    pile_t *p = malloc(sizeof(pile_t));
    if (!p) { return NULL; }

    p->vect = vecteur_creer(4);
    if(!p->vect) {return NULL; }
    return p;
}

void pile_liberer(pile_t *p) {
    assert(p);
    vecteur_liberer(p->vect);
    free(p);
}

void pile_empiler(pile_t *p, void *element) {
    assert(p);
    assert(element);
    vecteur_ajouter(p->vect, element);
}

void *pile_depiler(pile_t *p) {
    assert(p);
    return vecteur_retirer_dernier(p->vect);
}

void *pile_regarder(pile_t *p) {
    assert(p);
    return vecteur_obtenir(p->vect, (vecteur_taille(p->vect) - 1));
}

bool pile_est_vide(pile_t *p) {
    assert(p);
    return vecteur_est_vide(p->vect);
}

int  pile_taille(pile_t *p) {
    assert(p);
    return vecteur_taille(p->vect);
}