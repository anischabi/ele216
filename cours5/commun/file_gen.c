#include "file_gen.h"
#include "vecteur_gen.h"

#include <assert.h>
#include <stdlib.h>

struct file {
    vecteur_t *vect;
};

file_t *file_creer() {
    file_t *f = malloc(sizeof(file_t));
    if (!f) { return NULL; }

    f->vect = vecteur_creer(4);
    if(!f->vect) {return NULL; }
    return f;
}

void file_liberer(file_t *f) {
    assert(f);
    vecteur_liberer(f->vect);
    free(f);
}

void file_enfiler(file_t *f, void *element) {
    assert(f);
    assert(element);
    vecteur_ajouter(f->vect, element);
}

void *file_defiler(file_t *f) {
    assert(f);
    return vecteur_retirer_premier(f->vect);
}

void *file_regarder(file_t *f) {
    assert(f);
    return vecteur_obtenir(f->vect, 0);
}

bool file_est_vide(file_t *f) {
    assert(f);
    return vecteur_est_vide(f->vect);
}

int  file_taille(file_t *f) {
    assert(f);
    return vecteur_taille(f->vect);
}