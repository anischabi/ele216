/**
 * @file file_generique.c
 * Implémentation d'une file générique utilisant un tableau dynamique générique
 */

#include "file_generique.h"
#include "tabdyn_generique.h"

#include <stdlib.h>
#include <assert.h>

#define TAILLE_INITIALE (8)

/**
 * Définition de la structure file_generique
 * 
 * Cette structure est cachée dans le fichier .c pour encapsulation
 */
struct file_generique {
    tabdyn_t *tableau;
};

/**
 * Crée une nouvelle file générique vide.
 * @return Pointeur vers la file créée ou NULL en cas d'échec d'allocation mémoire.
 */
file_t *file_creer(void) {
    file_t *nouveau = malloc(sizeof(file_t));
    if (nouveau == NULL) { 
        return NULL; 
    }

    nouveau->tableau = tabdyn_creer(TAILLE_INITIALE);
    if (nouveau->tableau == NULL) { 
        free(nouveau);
        return NULL; 
    }
    return nouveau;
}

/**
 * Ajoute un élément à la fin de la file (opération d'enfilage).
 * @param file Pointeur non-nul vers la file.
 * @param element Pointeur vers l'élément à ajouter.
 * @return true si l'opération a réussi, false sinon.
 */
bool file_enfiler(file_t *file, void *element) {
    assert(file != NULL);
    return tabdyn_ajouter(file->tableau, element);
}

/**
 * Retire et retourne l'élément au début de la file (opération de défilage).
 * @param file Pointeur non-nul vers la file.
 * @return Pointeur vers l'élément retiré ou NULL si la file est vide.
 */
void *file_defiler(file_t *file) {
    assert(file != NULL);
    return tabdyn_supprimer(file->tableau, 0);
}

/**
 * Regarde l'élément au début de la file sans le retirer.
 * @param file Pointeur non-nul vers la file.
 * @return Pointeur vers l'élément en tête ou NULL si la file est vide.
 */
void *file_regarder_tete(const file_t *file) {
    assert(file != NULL);
    return tabdyn_obtenir(file->tableau, 0);
}

/**
 * Libère la mémoire allouée pour une file générique.
 * @param file Pointeur vers la file à libérer. Si le pointeur est NULL,
 *             la fonction n'a pas d'effet (voir la définition de free()).
 */
void file_liberer(file_t *file) {
    if (file == NULL) {
        return;
    }
    
    tabdyn_detruire(file->tableau);
    free(file);
}

/**
 * Obtient le nombre d'éléments dans la file.
 * @param file Pointeur non-nul vers la file.
 * @return Le nombre d'éléments dans la file.
 */
size_t file_taille(const file_t *file) {
    assert(file != NULL);
    return tabdyn_taille(file->tableau);
}

/**
 * Vérifie si la file est vide.
 * @param file Pointeur non-nul vers la file.
 * @return true si la file est vide, false sinon.
 */
bool file_est_vide(const file_t *file) {
    assert(file != NULL);
    return tabdyn_est_vide(file->tableau);
}

/**
 * Vide complètement la file en retirant tous les éléments.
 * @param file Pointeur non-nul vers la file.
 */
void file_vider(file_t *file) {
    assert(file != NULL);
    tabdyn_vider(file->tableau);
}

/**
 * Affiche le contenu de la file dans un format utile au débogage.
 * @param file Pointeur non-nul vers la file.
 */
void file_afficher(const file_t *file) {
    assert(file != NULL);
    tabdyn_afficher(file->tableau);
}
