/**
 * @file fileprio.h
 * Interface pour une file prioritaire générique
 */

#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


/**
 * Structure opaque représentant une file prioritaire
 */
typedef struct file_prioritaire fileprio_t;

/**
 * Crée une nouvelle file générique vide.
 * Les files interne d'indice inférieur sont plus prioritaires.
 * La priorité 0 est la plus grande.
 * @param nbPriorites Le nombre de priorités supportées.
 * @return Pointeur vers la file créée ou NULL en cas d'échec d'allocation mémoire.
 */
fileprio_t *fileprio_creer(uint8_t nbPriorites);

/**
 * Ajoute un élément à la fin de la file selon sa priorite.
 * @param file Pointeur non-nul vers la file.
 * @param element Pointeur vers l'élément à ajouter.
 * @param priorite niveau de priorité de l'élément (0 = plus prioritaire).
 * @return true si l'opération a réussi, false sinon.
 */
void fileprio_enfiler(fileprio_t *file, void *element, uint8_t priorite);

/**
 * Retire et retourne l'élément au début de la file (opération de défilage).
 * @param file Pointeur non-nul vers la file.
 * @return Pointeur vers l'élément retiré ou NULL si la file est vide.
 */
void *fileprio_defiler(fileprio_t *file);

/**
 * Regarde l'élément au début de la file sans le retirer.
 * @param file Pointeur non-nul vers la file.
 * @return Pointeur vers l'élément en tête ou NULL si la file est vide.
 */
void *fileprio_regarder_tete(const fileprio_t *file);

/**
 * Libère la mémoire allouée pour une file générique.
 * @param file Pointeur vers la file à libérer. Si le pointeur est NULL,
 *             la fonction n'a pas d'effet (voir la définition de free()).
 */
void fileprio_liberer(fileprio_t *file);

/**
 * Obtient le nombre d'éléments dans la file.
 * @param file Pointeur non-nul vers la file.
 * @return Le nombre d'éléments dans la file.
 */
size_t fileprio_taille(const fileprio_t *file);

/**
 * Vérifie si la file est vide.
 * @param file Pointeur non-nul vers la file.
 * @return true si la file est vide, false sinon.
 */
bool fileprio_est_vide(const fileprio_t *file);

/**
 * @brief Retourne le nombre de niveaux de priorité.
 * 
 * @param file 
 * @return uint8_t 
 */
uint8_t fileprio_niveaux(const fileprio_t *file);

/**
 * Affiche le contenu de la file dans un format utile au débogage.
 * @param file Pointeur non-nul vers la file.
 */
//void fileprio_afficher(const fileprio_t *file);
