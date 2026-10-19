/**
 * @file tabdyn_generique.c
 * Implémentation d'un tableau dynamique générique utilisant des pointeurs void
 */

#include "tabdyn_generique.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

/**
 * Définition de la structure TableauDynamique
 * 
 * Cette structure est cachée dans le fichier .c pour encapsulation
 */
struct TableauDynamique {
    size_t capacite;     /* Capacité actuelle */
    size_t taille;       /* Nombre d'éléments actuels */
    void **contenu;      /* Tableau de pointeurs vers les données */
};

/**
 * Crée un nouveau tableau dynamique générique.
 * @param capacite_initiale Capacité initiale du tableau.
 * @return Pointeur vers le tableau créé ou NULL en cas d'échec.
 */
tabdyn_t* tabdyn_creer(size_t capacite_initiale) {
    tabdyn_t* tableau = malloc(sizeof(tabdyn_t));
    if (!tableau) {
        return NULL;  // Échec d'allocation mémoire
    }
    
    tableau->contenu = malloc(capacite_initiale * sizeof(void*));
    if (!tableau->contenu && capacite_initiale > 0) {
        free(tableau);
        return NULL;  // Échec d'allocation mémoire
    }
    
    tableau->taille = 0;
    tableau->capacite = capacite_initiale;
    
    return tableau;
}

/**
 * Libère les ressources allouées pour un tableau dynamique générique.
 * @param tableau Pointeur vers le tableau à détruire. Si le pointeur est NULL,
 *                la fonction n'a pas d'effet (voir la définition de free()).
 */
void tabdyn_detruire(tabdyn_t* tableau) {
    if (tableau == NULL) {
        return;
    }
    
    free(tableau->contenu);
    free(tableau);
}

/**
 * Fonction auxiliaire pour redimensionner le tableau.
 * @param tableau Pointeur non-nul vers le tableau.
 * @param nouvelle_capacite Nouvelle capacité souhaitée.
 * @return true si le redimensionnement a réussi, false sinon.
 */
static bool redimensionner(tabdyn_t* tableau, size_t nouvelle_capacite) {
    assert(tableau != NULL);

    void **nouveau = realloc(tableau->contenu, nouvelle_capacite * sizeof(void*));
    if (!nouveau && nouvelle_capacite > 0) {
        return false;  // Échec de redimensionnement
    }
    
    tableau->contenu = nouveau;
    tableau->capacite = nouvelle_capacite;
    
    return true;  // Redimensionnement réussi
}

/**
 * Ajoute un élément à la fin du tableau.
 * Si l'ajout entraine un dépassement de capacité, la taille du tableau
 * est doublée. Si l'agrandissement est impossible, le tableau n'est pas
 * modifié et cette fonction retourne false.
 * @param tableau Pointeur non-nul vers le tableau.
 * @param element Élément à ajouter.
 * @return true si l'opération a réussi, false sinon.
 */
bool tabdyn_ajouter(tabdyn_t* tableau, void *element) {
    assert(tableau != NULL);

    if (tableau->taille == tableau->capacite) {
        // Doublement de la capacité quand plein
        size_t nouvelle_capacite = tableau->capacite == 0 ? 8 : tableau->capacite * 2;
        if (!redimensionner(tableau, nouvelle_capacite)) {
            return false;  // Échec de l'ajout
        }
    }
    
    tableau->contenu[tableau->taille] = element;
    tableau->taille++;
    return true;  // Ajout réussi
}

/**
 * Récupère un élément à l'indice spécifié.
 * @param tableau Pointeur non-nul vers le tableau.
 * @param indice Indice valide de l'élément à récupérer.
 * @return Pointeur vers l'élément à l'indice spécifié ou NULL si l'indice est invalide.
 */
void *tabdyn_obtenir(const tabdyn_t* tableau, size_t indice) {
    assert(tableau != NULL);
    if (indice >= tableau->taille) {
        return NULL;
    }
    return tableau->contenu[indice];
}

/**
 * Modifie un élément à l'indice spécifié.
 * @param tableau Pointeur non-nul vers le tableau.
 * @param indice Indice valide de l'élément à modifier.
 * @param element Nouveau pointeur vers l'élément.
 * @return true si l'opération a réussi, false si l'indice est invalide.
 */
bool tabdyn_modifier(tabdyn_t* tableau, size_t indice, void *element) {
    assert(tableau != NULL);
    if (indice >= tableau->taille) {
        return false;
    }
    tableau->contenu[indice] = element;
    return true;
}

/**
 * Obtient le nombre d'éléments dans le tableau.
 * @param tableau Pointeur non-nul vers le tableau.
 * @return Le nombre d'éléments dans le tableau.
 */
size_t tabdyn_taille(const tabdyn_t* tableau) {
    assert(tableau != NULL);
    return tableau->taille;
}

/**
 * Obtient la capacité actuelle du tableau.
 * @param tableau Pointeur non-nul vers le tableau.
 * @return La capacité actuelle du tableau.
 */
size_t tabdyn_capacite(const tabdyn_t* tableau) {
    assert(tableau != NULL);
    return tableau->capacite;
}

/**
 * Supprime et retourne le dernier élément du tableau.
 * @param tableau Pointeur non-nul vers le tableau.
 * @return Pointeur vers le dernier élément ou NULL si le tableau est vide.
 */
void *tabdyn_retirer_dernier(tabdyn_t* tableau) {
    assert(tableau != NULL);
    if (tableau->taille == 0) {
        return NULL;  // Tableau vide
    }
    
    void *element = tableau->contenu[tableau->taille-1];
    tableau->taille--;
    return element;
}

/**
 * Insère un élément à un indice spécifié.
 * @param tableau Pointeur non-nul vers le tableau.
 * @param indice Indice valide où insérer l'élément.
 * @param element Élément à insérer.
 * @return true si l'opération a réussi, false sinon.
 */
bool tabdyn_inserer(tabdyn_t* tableau, size_t indice, void *element) {
    assert(tableau != NULL);
    if (indice > tableau->taille) {
        return false;  // Indice invalide
    }
    
    if (tableau->taille == tableau->capacite) {
        // Doublement de la capacité quand plein
        size_t nouvelle_capacite = tableau->capacite == 0 ? 8 : tableau->capacite * 2;
        if (!redimensionner(tableau, nouvelle_capacite)) {
            return false;  // Échec de l'ajout
        }
    }
    
    // Décalage des éléments pour faire de la place
    if (indice < tableau->taille) {
        memmove(tableau->contenu + indice + 1, tableau->contenu + indice, 
                (tableau->taille - indice) * sizeof(void*));
    }
    
    tableau->contenu[indice] = element;
    tableau->taille++;
    
    return true;  // Insertion réussie
}

/**
 * Supprime un élément à un indice spécifié.
 * @param tableau Pointeur non-nul vers le tableau.
 * @param indice Indice valide de l'élément à supprimer.
 * @return Pointeur vers l'élément supprimé ou NULL si l'indice est invalide.
 */
void *tabdyn_supprimer(tabdyn_t* tableau, size_t indice) {
    assert(tableau != NULL);
    if (indice >= tableau->taille) {
        return NULL;  // Indice invalide
    }
    
    void *element_supprime = tableau->contenu[indice];
    
    // Décalage des éléments pour combler le trou
    if (indice < tableau->taille - 1) {
        memmove(tableau->contenu + indice, tableau->contenu + indice + 1, 
                (tableau->taille - indice - 1) * sizeof(void*));
    }
    
    tableau->taille--;
    
    // Réduction optionnelle du tableau s'il est trop grand
    if (tableau->taille < tableau->capacite / 4 && tableau->capacite > 8) {
        redimensionner(tableau, tableau->capacite / 2);
    }
    
    return element_supprime;
}

/**
 * Supprime tous les éléments du tableau.
 * @param tableau Pointeur non-nul vers le tableau.
 */
void tabdyn_vider(tabdyn_t* tableau) {
    assert(tableau != NULL);
    
    tableau->taille = 0;
    
    // Redimensionnement optionnel pour économiser de la mémoire
    if (tableau->capacite > 8) {
        redimensionner(tableau, 8);
    }
}

/**
 * Vérifie si le tableau est vide.
 * @param tableau Pointeur non-nul vers le tableau.
 * @return true si le tableau est vide, false sinon.
 */
bool tabdyn_est_vide(const tabdyn_t* tableau) {
    assert(tableau != NULL);
    return tableau->taille == 0;
}

/**
 * Affiche le contenu du tableau dynamique dans un format utile au débogage.
 * @param tableau Pointeur non-nul vers le tableau dynamique.
 */
void tabdyn_afficher(const tabdyn_t *tableau) {
    assert(tableau != NULL);
    printf("Tableau dynamique - capacité : %zu, taille: %zu\n", tableau->capacite, tableau->taille);
    printf("{ ");
    for (size_t i = 0; i < tableau->taille; i++) {
        printf("[%p]", tableau->contenu[i]);
        
        if (i < tableau->taille - 1) {
            printf(", ");
        }
    }
    printf(" }\n");
}