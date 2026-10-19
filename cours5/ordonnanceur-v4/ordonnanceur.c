/**
 * @file ordonnanceur.c
 * Implémentation d'un ordonnanceur de processus utilisant une file de priorité
 */

#include "ordonnanceur.h"

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "fileprio.h"
#include "processus.h"

#define TAILLE_INITIALE (8)

/**
 * Définition de la structure ordonnanceur
 * 
 * Cette structure est cachée dans le fichier .c pour encapsulation
 */
struct ordonnanceur {
    fileprio_t *taches;     /* File de priorité contenant les processus */
};

/**
 * Crée un nouvel ordonnanceur de processus.
 * @return Pointeur vers l'ordonnanceur créé ou NULL en cas d'échec d'allocation mémoire.
 */
ordonnanceur_t *ordonnanceur_creer() {
    ordonnanceur_t *nouveau = malloc(sizeof(ordonnanceur_t));
    if (nouveau == NULL) { 
        return NULL; 
    }

    nouveau->taches = fileprio_creer(TAILLE_INITIALE);
    if (nouveau->taches == NULL) { 
        free(nouveau);
        return NULL; 
    }
    return nouveau;
}

/**
 * Interprète et exécute une commande textuelle pour l'ordonnanceur.
 * Les commandes supportées sont :
 * - "add nom duree priorite" : ajoute un processus
 * - "run duree" : exécute les processus pendant la durée spécifiée
 * - "quit" : termine l'ordonnanceur
 * 
 * @param ordo Pointeur non-nul vers l'ordonnanceur.
 * @param buffer Chaîne de caractères contenant la commande à interpréter.
 *               Le buffer est modifié par l'utilisation de strtok().
 * @return true si la commande "quit" a été exécutée, false sinon.
 */
bool ordonnanceur_interprete(ordonnanceur_t* ordo, char *buffer) {
    char *mot;
    
    // extraction du premier mot
    mot = strtok(buffer, " \n");
    if (mot == NULL) { return false; }

    if (strcmp("add", mot) == 0) {
        ordonnanceur_ajouter(ordo);

    } else if (strcmp("run", mot ) == 0) {
        ordonnanceur_executer(ordo);

    } else if (strcmp("quit", mot) == 0) {
        return true;
    }
    return false;
}

/**
 * Ajoute un processus à l'ordonnanceur à partir des paramètres extraits 
 * par strtok() du buffer de commande.
 * Attend les paramètres dans l'ordre : nom, durée, priorité.
 * Si un paramètre est manquant ou si la priorité dépasse le nombre
 * de niveaux supportés, la fonction n'a pas d'effet.
 * 
 * @param ordo Pointeur non-nul vers l'ordonnanceur.
 */
void ordonnanceur_ajouter(ordonnanceur_t* ordo) {
    char *nom;
    char *duree;
    char *priorite;

    nom = strtok(NULL, " ");
    if (nom == NULL) { return; }

    duree = strtok(NULL, " ");
    if (duree == NULL) { return; }

    priorite = strtok(NULL, " \n");
    if (priorite == NULL) { return; }
    if (atoi(priorite) > fileprio_niveaux(ordo->taches)) { return; }

    fileprio_enfiler(ordo->taches, processus_creer(nom, atoi(duree), atoi(priorite)), atoi(priorite));
}

/**
 * Exécute les processus de l'ordonnanceur pendant une durée spécifiée.
 * La durée est extraite du buffer de commande par strtok().
 * Les processus sont exécutés par unités de temps (UNITE_TEMPS) selon
 * leur priorité. Un processus exécuté qui n'est pas terminé est remis
 * en file avec sa priorité d'origine. L'exécution s'arrête quand la
 * durée est épuisée ou qu'il n'y a plus de processus.
 * 
 * @param ordo Pointeur non-nul vers l'ordonnanceur.
 */
void ordonnanceur_executer(ordonnanceur_t* ordo) {
    char *duree_str;
    duree_str = strtok(NULL, " \n");
    if (duree_str == NULL) { return; }
    int duree = atoi(duree_str);

    while (duree > 0) {
        if (! fileprio_est_vide(ordo->taches)) {
            processus_t *prochain = fileprio_defiler(ordo->taches);
            processus_execute(prochain, UNITE_TEMPS);
            processus_afficher(prochain);

            if (! processus_est_termine(prochain)) {
                fileprio_enfiler(ordo->taches, prochain, processus_obtenir_priorite(prochain));
            }
            duree -= UNITE_TEMPS;
        } else {
            return;
        }
    } 
}
