/**
 * @file ordonnanceur.h
 * Interface pour un ordonnanceur de processus avec file de priorité
 */

#pragma once

#include <stdbool.h>

/**
 * Structure opaque représentant un ordonnanceur de processus
 */
typedef struct ordonnanceur ordonnanceur_t;

/**
 * Unité de temps pour l'exécution des processus
 */
#define UNITE_TEMPS (20)

/**
 * Crée un nouvel ordonnanceur de processus.
 * @return Pointeur vers l'ordonnanceur créé ou NULL en cas d'échec d'allocation mémoire.
 */
ordonnanceur_t *ordonnanceur_creer();

/**
 * Interprète et exécute une commande textuelle pour l'ordonnanceur.
 * Les commandes supportées sont :
 * - "add nom duree priorite" : ajoute un processus
 * - "run duree" : exécute les processus pendant la durée spécifiée
 * - "quit" : termine l'ordonnanceur
 * 
 * @param ordo Pointeur non-nul vers l'ordonnanceur.
 * @param buffer Chaîne de caractères contenant la commande à interpréter.
 * @return true si la commande "quit" a été exécutée, false sinon.
 */
bool ordonnanceur_interprete(ordonnanceur_t* ordo, char *buffer);

/**
 * Ajoute un processus à l'ordonnanceur à partir des paramètres extraits 
 * par strtok() du buffer de commande.
 * Attend les paramètres dans l'ordre : nom, durée, priorité.
 * 
 * @param ordo Pointeur non-nul vers l'ordonnanceur.
 */
void ordonnanceur_ajouter(ordonnanceur_t* ordo);

/**
 * Exécute les processus de l'ordonnanceur pendant une durée spécifiée.
 * La durée est extraite du buffer de commande par strtok().
 * Les processus sont exécutés par unités de temps (UNITE_TEMPS) selon
 * leur priorité jusqu'à épuisement de la durée ou des processus.
 * 
 * @param ordo Pointeur non-nul vers l'ordonnanceur.
 */
void ordonnanceur_executer(ordonnanceur_t* ordo);
