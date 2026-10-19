/**
 * @file tabdyn_generique.h
 * Interface pour un tableau dynamique générique utilisant des pointeurs void
 */

#pragma once

#include <stddef.h> // Pour size_t
#include <stdbool.h>

/**
 * Structure opaque représentant un tableau dynamique générique
 */
typedef struct TableauDynamique tabdyn_t;


/**
 * Crée un nouveau tableau dynamique générique.
 * @param capacite_initiale Capacité initiale du tableau.
 * @return Pointeur vers le tableau créé ou NULL en cas d'échec.
 */
tabdyn_t* tabdyn_creer(size_t capacite_initiale);

/**
 * Libère les ressources allouées pour un tableau dynamique générique.
 * @param tableau Pointeur vers le tableau à détruire. Si le pointeur est NULL,
 *                la fonction n'a pas d'effet (voir la définition de free()).
 */
void tabdyn_detruire(tabdyn_t* tableau);

/**
 * Ajoute un élément à la fin du tableau.
 * Si l'ajout entraine un dépassement de capacité, la taille du tableau
 * est doublée. Si l'agrandissement est impossible, le tableau n'est pas
 * modifié et cette fonction retourne false.
 * @param tableau Pointeur non-nul vers le tableau.
 * @param element Élément à ajouter.
 * @return true si l'opération a réussi, false sinon.
 */
bool tabdyn_ajouter(tabdyn_t* tableau, void *element);

/**
 * Récupère un élément à l'indice spécifié.
 * @param tableau Pointeur non-nul vers le tableau.
 * @param indice Indice valide de l'élément à récupérer.
 * @return Pointeur vers l'élément à l'indice spécifié ou NULL si l'indice est invalide.
 */
void *tabdyn_obtenir(const tabdyn_t* tableau, size_t indice);

/**
 * Modifie un élément à l'indice spécifié.
 * @param tableau Pointeur non-nul vers le tableau.
 * @param indice Indice valide de l'élément à modifier.
 * @param element Nouveau pointeur vers l'élément.
 * @return true si l'opération a réussi, false si l'indice est invalide.
 */
bool tabdyn_modifier(tabdyn_t* tableau, size_t indice, void *element);

/**
 * Obtient le nombre d'éléments dans le tableau.
 * @param tableau Pointeur non-nul vers le tableau.
 * @return Le nombre d'éléments dans le tableau.
 */
size_t tabdyn_taille(const tabdyn_t* tableau);

/**
 * Obtient la capacité actuelle du tableau.
 * @param tableau Pointeur non-nul vers le tableau.
 * @return La capacité actuelle du tableau.
 */
size_t tabdyn_capacite(const tabdyn_t* tableau);

/**
 * Supprime et retourne le dernier élément du tableau.
 * @param tableau Pointeur non-nul vers le tableau.
 * @return Pointeur vers le dernier élément ou NULL si le tableau est vide.
 */
void *tabdyn_retirer_dernier(tabdyn_t* tableau);

/**
 * Insère un élément à un indice spécifié.
 * @param tableau Pointeur non-nul vers le tableau.
 * @param indice Indice valide où insérer l'élément.
 * @param element Élément à insérer.
 * @return true si l'opération a réussi, false sinon.
 */
bool tabdyn_inserer(tabdyn_t* tableau, size_t indice, void *element);

/**
 * Supprime un élément à un indice spécifié.
 * @param tableau Pointeur non-nul vers le tableau.
 * @param indice Indice valide de l'élément à supprimer.
 * @return Pointeur vers l'élément supprimé ou NULL si l'indice est invalide.
 */
void *tabdyn_supprimer(tabdyn_t* tableau, size_t indice);

/**
 * Supprime tous les éléments du tableau.
 * @param tableau Pointeur non-nul vers le tableau.
 */
void tabdyn_vider(tabdyn_t* tableau);

/**
 * Vérifie si le tableau est vide.
 * @param tableau Pointeur non-nul vers le tableau.
 * @return true si le tableau est vide, false sinon.
 */
bool tabdyn_est_vide(const tabdyn_t* tableau);

/**
 * Affiche le contenu du tableau dynamique dans un format utile au débogage.
 * @param tableau Pointeur non-nul vers le tableau dynamique.
 */
void tabdyn_afficher(const tabdyn_t *tableau);
