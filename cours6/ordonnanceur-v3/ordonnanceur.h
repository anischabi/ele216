#pragma once

#include <stdbool.h>

// structure opaque pour l'ordonnanceur de processus
typedef struct ordonnanceur ordonnanceur_t;

#define UNITE_TEMPS (20)

/**
 * @brief Crée un nouvel ordonnanceur de processus.
 * 
 * @return ordonnaceur_t* un pointeur vers l'ordonnanceur ou NULL
 *                          en cas d'échec d'allocation mémoire.
 */
ordonnanceur_t *ordonnanceur_creer();

/**
 * @brief Interprète et exécute une commande textuelle pour l'ordonnanceur.
 * Les commandes supportées sont :
 * - "add nom duree priorite" : ajoute un processus.
 * - "run duree" : exécute les processus pendant la durée spécifiée.
 * - "quit" : termine l'ordonnanceur.
 * 
 * @param ordo      Pointeur non-nul vers l'ordonnanceur.
 * @param commande  Chaîne de caractères contenant la commande à interpréter.
 * @return true     si la commande "quit" a été exécutée
 * @return false    sinon
 */
bool ordonnanceur_interprete(ordonnanceur_t *ordo, char *commande);

