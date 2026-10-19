/**
 * @file file_generique.h
 * Interface pour une file générique utilisant un tableau dynamique générique
 */

#pragma once
#include <stdbool.h>
#include <stddef.h>

/**
 * Structure opaque représentant une file générique
 */
typedef struct file_generique file_t;

/**
 * Type de fonction pour afficher un élément de la file
 * @param element Pointeur vers l'élément à afficher
 */
typedef void (*afficher_element_fn)(const void *element);

/**
 * Type de fonction pour libérer un élément de la file
 * @param element Pointeur vers l'élément à libérer
 */
typedef void (*liberer_element_fn)(void *element);

/**
 * Crée une nouvelle file générique vide.
 * @return Pointeur vers la file créée ou NULL en cas d'échec d'allocation mémoire.
 */
file_t *file_creer(void);

/**
 * Ajoute un élément à la fin de la file (opération d'enfilage).
 * @param file Pointeur non-nul vers la file.
 * @param element Pointeur vers l'élément à ajouter.
 * @return true si l'opération a réussi, false sinon.
 */
bool file_enfiler(file_t *file, void *element);

/**
 * Retire et retourne l'élément au début de la file (opération de défilage).
 * @param file Pointeur non-nul vers la file.
 * @return Pointeur vers l'élément retiré ou NULL si la file est vide.
 */
void *file_defiler(file_t *file);

/**
 * Regarde l'élément au début de la file sans le retirer.
 * @param file Pointeur non-nul vers la file.
 * @return Pointeur vers l'élément en tête ou NULL si la file est vide.
 */
void *file_regarder_tete(const file_t *file);

/**
 * Libère la mémoire allouée pour une file générique.
 * @param file Pointeur vers la file à libérer. Si le pointeur est NULL,
 *             la fonction n'a pas d'effet (voir la définition de free()).
 */
void file_liberer(file_t *file);

/**
 * Obtient le nombre d'éléments dans la file.
 * @param file Pointeur non-nul vers la file.
 * @return Le nombre d'éléments dans la file.
 */
size_t file_taille(const file_t *file);

/**
 * Vérifie si la file est vide.
 * @param file Pointeur non-nul vers la file.
 * @return true si la file est vide, false sinon.
 */
bool file_est_vide(const file_t *file);

/**
 * Vide complètement la file en retirant tous les éléments.
 * @param file Pointeur non-nul vers la file.
 */
void file_vider(file_t *file);

/**
 * Affiche le contenu de la file dans un format utile au débogage.
 * @param file Pointeur non-nul vers la file.
 */
void file_afficher(const file_t *file);
