/**
 * @file processus.c
 * Implémentation d'un modèle simplifié de processus géré par un système d'exploitation.
 */

#include "processus.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdint.h>

/**
 * Définition de la structure processus
 * 
 * Cette structure est cachée dans le fichier .c pour encapsulation
 */
struct processus {
    char *nom;          /* Nom du processus */
    int duree;          /* Durée restante d'exécution */
    uint8_t priorite;   /* Priorite du processus */
    bool est_termine;   /* État du processus (terminé ou non) */
};

/**
 * Crée un nouveau processus avec un nom et une durée d'exécution.
 * @param nom Pointeur vers le nom du processus (la chaîne n'est pas copiée).
 * @param duree Durée d'exécution initiale du processus.
 * @param priorite Priorité du processus.
 * @return Pointeur vers le processus créé ou NULL en cas d'échec d'allocation mémoire.
 */
processus_t *processus_creer(char *nom, int duree, uint8_t priorite) {
    processus_t *p = malloc(sizeof(processus_t));
    if (p == NULL) { return NULL; }

    p->nom = nom;
    p->duree = duree;
    p->priorite = priorite;
    p->est_termine = false;
    return p;
}

/**
 * Libère la mémoire allouée pour un processus.
 * @param p Pointeur vers le processus à détruire.
 */
void processus_detruire(processus_t *p) {
    free(p);
}

/**
 * Obtient le nom du processus.
 * @param p Pointeur non-nul vers le processus.
 * @return Pointeur vers le nom du processus.
 */
char *processus_obtenir_nom(processus_t *p) {
    assert(p != NULL);
    return p->nom;
}

/**
 * Obtient la durée restante du processus.
 * @param p Pointeur non-nul vers le processus.
 * @return La durée restante d'exécution du processus.
 */
int processus_obtenir_duree(processus_t *p) {
    assert(p != NULL);
    return p->duree;
}

/**
 * Obtient la priorité du processus.
 * @param p Pointeur non-nul vers le processus.
 * @return La priorité d'exécution du processus.
 */
uint8_t processus_obtenir_priorite(processus_t *p) {
    assert(p != NULL);
    return p->priorite;
}

/**
 * Vérifie si le processus est terminé.
 * @param p Pointeur non-nul vers le processus.
 * @return true si le processus est terminé, false sinon.
 */
bool processus_est_termine(processus_t *p) {
    assert(p != NULL);
    return p->est_termine;
}

/**
 * Exécute le processus pendant une durée spécifiée.
 * Diminue la durée restante du processus. Si la durée d'exécution
 * est supérieure ou égale à la durée restante, le processus est marqué comme terminé.
 * @param p Pointeur non-nul vers le processus.
 * @param duree Durée d'exécution à appliquer.
 */
void processus_execute(processus_t *p, int duree) {
    assert(p != NULL);
    if (p->duree > duree) {
        p->duree -= duree;
    } else {
        p->duree = 0;
        p->est_termine = true;
    }
}

/**
 * Affiche les informations du processus dans un format lisible.
 * @param p Pointeur non-nul vers le processus.
 */
void processus_afficher(processus_t *p) {
    assert(p != NULL);
    printf("Processus : %s, Temps restant : %d\n", p->nom, p->duree);
}