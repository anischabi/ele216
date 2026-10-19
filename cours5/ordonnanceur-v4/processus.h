/**
 * @file processus.h
 * Interface d'un modèle simplifié de processus géré par un système d'exploitation.
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>

/**
 * Structure opaque représentant un processus
 */
typedef struct processus processus_t;

/**
 * Crée un nouveau processus avec un nom et une durée d'exécution.
 * @param nom Pointeur vers le nom du processus (la chaîne n'est pas copiée).
 * @param duree Durée d'exécution initiale du processus.
 * @param priorite Priorité du processus.
 * @return Pointeur vers le processus créé ou NULL en cas d'échec d'allocation mémoire.
 */
processus_t *processus_creer(char *nom, int duree, uint8_t priorite);

/**
 * Libère la mémoire allouée pour un processus.
 * @param p Pointeur vers le processus à détruire.
 */
void processus_detruire(processus_t *p);

/**
 * Obtient le nom du processus.
 * @param p Pointeur non-nul vers le processus.
 * @return Pointeur vers le nom du processus.
 */
char *processus_obtenir_nom(processus_t *p);

/**
 * Obtient la durée restante du processus.
 * @param p Pointeur non-nul vers le processus.
 * @return La durée restante d'exécution du processus.
 */
int processus_obtenir_duree(processus_t *p);

/**
 * Obtient la priorité du processus.
 * @param p Pointeur non-nul vers le processus.
 * @return La priorité d'exécution du processus.
 */
uint8_t processus_obtenir_priorite(processus_t *p);

/**
 * Vérifie si le processus est terminé.
 * @param p Pointeur non-nul vers le processus.
 * @return true si le processus est terminé, false sinon.
 */
bool processus_est_termine(processus_t *p);

/**
 * Exécute le processus pendant une durée spécifiée.
 * Diminue la durée restante du processus. Si la durée d'exécution
 * est supérieure ou égale à la durée restante, le processus est marqué comme terminé.
 * @param p Pointeur non-nul vers le processus.
 * @param duree Durée d'exécution à appliquer.
 */
void processus_execute(processus_t *p, int duree);

/**
 * Affiche les informations du processus dans un format lisible.
 * @param p Pointeur non-nul vers le processus.
 */
void processus_afficher(processus_t *p);
