#include "file_gen.h"
#include "vecteur_gen.h"

#include <assert.h>
#include <stdlib.h>

struct file {
    vecteur_t *vect;
};

file_gen_t *file_gen_creer() {
    file_gen_t *f = malloc(sizeof(file_gen_t));
    if (!f) { return NULL; }

    f->vect = vecteur_creer(4);
    if(!f->vect) {
        // Si vecteur_creer échoue, f n'est pas libéré. 
        //Il faudrait ajouter free(f) ici pour éviter une fuite de mémoire.
        return NULL; 
    }     
    return f;
}

void file_gen_liberer(file_gen_t *f) {
    assert(f);
    vecteur_liberer(f->vect);
    free(f);
}

void file_gen_enfiler(file_gen_t *f, void *element) {
    assert(f);
    assert(element);
    // resultat de vecteur_ajouter est ignoré. Il faudrait vérifier si l'ajout a réussi.
    vecteur_ajouter(f->vect, element);
}

void *file_gen_defiler(file_gen_t *f) {
    assert(f);
    return vecteur_retirer_premier(f->vect);
}

void *file_gen_regarder(file_gen_t *f) {
    assert(f);
    return vecteur_obtenir(f->vect, 0);
}

bool file_gen_est_vide(file_gen_t *f) {
    assert(f);
    return vecteur_est_vide(f->vect);
}

int  file_gen_taille(file_gen_t *f) {
    assert(f);
    return vecteur_taille(f->vect);
}