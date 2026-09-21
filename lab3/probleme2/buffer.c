#include "buffer.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// Définition de la structure dans le .c : les champs sont invisibles de
// l'extérieur du module.
//
// Invariants :
//  - 0 <= nb_elements <= capacite
//  - 0 <= tete < capacite et 0 <= queue < capacite
//  - queue == (tete + nb_elements) % capacite
//
// tete et queue sont égaux à la fois quand le buffer est vide ET quand il est
// plein. C'est nb_elements qui permet de les distinguer.
struct buffer {
    int capacite;           // immuable, fixée à la création
    int *donnees;           // tableau de 'capacite' entiers sur le heap
    int tete;               // index du prochain élément à défiler
    int queue;              // index du prochain emplacement libre
    int nb_elements;        // nombre d'éléments présents
};

buffer_t *buffer_creer(int capacite) {
    if (capacite <= 0) { return NULL; }

    buffer_t *b = malloc(sizeof(buffer_t));
    if (b == NULL) { return NULL; }

    b->donnees = malloc(capacite * sizeof(int));
    if (b->donnees == NULL) {
        free(b);            // ne pas fuir la structure si le tableau échoue
        return NULL;
    }

    b->capacite = capacite;
    b->tete = 0;
    b->queue = 0;
    b->nb_elements = 0;
    return b;
}

void buffer_detruire(buffer_t *b) {
    if (b == NULL) { return; }
    free(b->donnees);       // d'abord le tableau (b->donnees est encore lisible)...
    free(b);                // ...puis la structure qui le référence
}

int buffer_enfiler(buffer_t *b, int valeur) {
    assert(b != NULL);
    if (b->nb_elements == b->capacite) { return 0; }

    b->donnees[b->queue] = valeur;
    b->queue = (b->queue + 1) % b->capacite;
    b->nb_elements++;
    return 1;
}

int buffer_defiler(buffer_t *b, int *valeur) {
    assert(b != NULL);
    assert(valeur != NULL);
    if (b->nb_elements == 0) { return 0; }

    *valeur = b->donnees[b->tete];
    b->tete = (b->tete + 1) % b->capacite;
    b->nb_elements--;
    return 1;
}

int buffer_capacite(const buffer_t *b) {
    assert(b != NULL);
    return b->capacite;
}

int buffer_taille(const buffer_t *b) {
    assert(b != NULL);
    return b->nb_elements;
}

int buffer_est_vide(const buffer_t *b) {
    assert(b != NULL);
    return b->nb_elements == 0;
}

int buffer_est_plein(const buffer_t *b) {
    assert(b != NULL);
    return b->nb_elements == b->capacite;
}

void buffer_afficher(const buffer_t *b) {
    assert(b != NULL);
    printf("Buffer (%d/%d) : [", b->nb_elements, b->capacite);
    for (int i = 0; i < b->nb_elements; i++) {
        // Ordre FIFO : on part de la tête et on revient au début du tableau
        // (modulo) si nécessaire.
        int index = (b->tete + i) % b->capacite;
        if (i > 0) { printf(", "); }
        printf("%d", b->donnees[index]);
    }
    printf("]\n");
}
