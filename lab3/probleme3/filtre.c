#include "filtre.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// Définition de la structure dans le .c : les champs sont invisibles de
// l'extérieur du module.
//
// Le tableau est utilisé comme un buffer circulaire : index_ecriture est la
// case où sera écrit le prochain échantillon. Quand la fenêtre est pleine,
// cette case contient justement le plus ancien échantillon, qui est donc
// celui qui est remplacé.
//
// Invariants :
//  - 0 <= nb_echantillons <= taille_fenetre
//  - 0 <= index_ecriture < taille_fenetre
//  - Les nb_echantillons échantillons valides sont dans les cases
//    0 à nb_echantillons - 1 (avant que la fenêtre soit pleine), ou dans
//    toutes les cases (une fois pleine).
struct filtre {
    int taille_fenetre;         // immuable, fixée à la création
    float *echantillons;        // tableau de 'taille_fenetre' float sur le heap
    int nb_echantillons;        // reçus depuis la création, plafonné à taille_fenetre
    int index_ecriture;         // prochain emplacement à écrire
};

filtre_t *filtre_creer(int taille_fenetre) {
    if (taille_fenetre <= 0) { return NULL; }

    filtre_t *f = malloc(sizeof(filtre_t));
    if (f == NULL) { return NULL; }

    f->echantillons = malloc(taille_fenetre * sizeof(float));
    if (f->echantillons == NULL) {
        free(f);            // ne pas fuir la structure si le tableau échoue
        return NULL;
    }

    f->taille_fenetre = taille_fenetre;
    f->nb_echantillons = 0;
    f->index_ecriture = 0;
    return f;
}

void filtre_detruire(filtre_t *f) {
    if (f == NULL) { return; }
    free(f->echantillons);  // d'abord le tableau (f->echantillons est encore lisible)...
    free(f);                // ...puis la structure qui le référence
}

void filtre_ajouter(filtre_t *f, float echantillon) {
    assert(f != NULL);

    f->echantillons[f->index_ecriture] = echantillon;
    f->index_ecriture = (f->index_ecriture + 1) % f->taille_fenetre;

    // Plafonné : évite qu'un compteur qui compterait tous les échantillons
    // reçus finisse par déborder après un très grand nombre d'ajouts.
    if (f->nb_echantillons < f->taille_fenetre) {
        f->nb_echantillons++;
    }
}

float filtre_valeur(const filtre_t *f) {
    assert(f != NULL);
    if (f->nb_echantillons == 0) { return 0.0f; }   // évite 0 / 0 (NaN)

    // L'ordre n'a pas d'importance pour une somme : il suffit de parcourir
    // les cases valides. Accumulateur en double pour limiter l'erreur d'arrondi.
    double somme = 0.0;
    for (int i = 0; i < f->nb_echantillons; i++) {
        somme += f->echantillons[i];
    }
    return (float)(somme / f->nb_echantillons);
}

int filtre_taille_fenetre(const filtre_t *f) {
    assert(f != NULL);
    return f->taille_fenetre;
}

int filtre_nb_echantillons(const filtre_t *f) {
    assert(f != NULL);
    return f->nb_echantillons;
}

void filtre_afficher(const filtre_t *f) {
    assert(f != NULL);
    printf("Filtre (%d/%d) : [", f->nb_echantillons, f->taille_fenetre);

    // Le plus ancien est à index_ecriture quand la fenêtre est pleine,
    // sinon à la case 0.
    int debut = (f->nb_echantillons == f->taille_fenetre) ? f->index_ecriture : 0;
    for (int i = 0; i < f->nb_echantillons; i++) {
        int index = (debut + i) % f->taille_fenetre;
        if (i > 0) { printf(", "); }
        printf("%.3f", f->echantillons[index]);
    }
    printf("] moyenne = %.3f\n", filtre_valeur(f));
}
