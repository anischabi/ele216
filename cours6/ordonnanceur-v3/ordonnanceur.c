#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "ordonnanceur.h"
#include "fileprio.h"
#include "processus.h"

// structure opaque pour l'ordonnanceur de processus
struct ordonnanceur {
    fileprio_t *taches;
};

#define TAILLE_INITIALE (8)

//////////////  Fonctions statiques //////////////////
/**
 * @brief Ajoute un processus à l'ordonnanceur à partir des paramètres extraits
 *          par strtok() du buffer de commande.
 * Attend les paramètres dans l'ordre : nom, durée, priorité
 * 
 * @param ordo Pointeur non-nul vers l'ordonnanceur.
 */
static void ajouter(ordonnanceur_t *ordo);

/**
 * @brief Exécute les processus de l'ordonnanceur pendant une durée déterminée.
 * 
 * @param ordo      Pointeur non-nul vers l'ordonnanceur.
 */
static void executer(ordonnanceur_t *ordo);

ordonnanceur_t *ordonnanceur_creer() {
    ordonnanceur_t *nouveau = malloc(sizeof(ordonnanceur_t));
    if (!nouveau) {return NULL; }

    nouveau->taches = fileprio_creer(TAILLE_INITIALE);
    if (! nouveau->taches) {
        free(nouveau);
        return NULL;
    }
    return nouveau;
}

bool ordonnanceur_interprete(ordonnanceur_t *ordo, char *commande) {
    char *mot;

    // extraction du premier mot
    mot = strtok(commande, " \n");
    if (mot == NULL) { return false; }

    if (strcmp("add", mot) == 0) {
        ajouter(ordo);
    } else if (strcmp("run", mot) == 0) {
        executer(ordo);
    } else if (strcmp("quit", mot) == 0) {
        return true;
    }
    return false;
}

static void ajouter(ordonnanceur_t *ordo) {
    assert(ordo);

    char* nom;
    char* duree;
    char* priorite;

    nom = strtok(NULL, " ");
    if (nom == NULL) { return; }

    duree = strtok(NULL, " ");
    if (duree == NULL) { return; }

    priorite = strtok(NULL, " \n");
    if (priorite == NULL) { return; }

    if (atoi(priorite) > fileprio_niveaux(ordo->taches)) {
        return;
    }
    fileprio_enfiler(ordo->taches, 
                    processus_creer(nom, atoi(duree), atoi(priorite)),
                    atoi(priorite));
}


static void executer(ordonnanceur_t *ordo) {
    assert(ordo);

    char *duree_str;
    duree_str = strtok(NULL, " \n");
    if (duree_str == NULL) { return; }
    int duree = atoi(duree_str);

    while (duree > 0) {
        if(! fileprio_est_vide(ordo->taches)) {
            processus_t *suivant = fileprio_defiler(ordo->taches);
            processus_execute(suivant, 20);
            processus_afficher(suivant);
            if (!processus_est_termine(suivant)) {
                fileprio_enfiler(ordo->taches, suivant, processus_obtenir_priorite(suivant));
            }
            duree -= UNITE_TEMPS;
        } else {
            return;
        }
    }
}
