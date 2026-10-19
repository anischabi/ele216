/**
 * @file fileprio.h
 * Interface pour une file prioritaire générique
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include "tabdyn_generique.h"
#include "fileprio.h"
#include "file_generique.h"

/**
 * Définition de la structure file_prioritaire
 * 
 * Cette structure est cachée dans le fichier .c pour encapsulation
 */
struct file_prioritaire {
    tabdyn_t *listes;      /* tableau de file_t */
};

/**
 * Crée une nouvelle file générique vide.
 * Les files interne d'indice inférieur sont plus prioritaires.
 * La priorité 0 est la plus grande.
 * @param nbPriorites Le nombre de priorités supportées.
 * @return Pointeur vers la file créée ou NULL en cas d'échec d'allocation mémoire.
 */
fileprio_t *fileprio_creer(uint8_t nbPriorites) {
    fileprio_t *file = malloc(sizeof(fileprio_t*));
    if (file == NULL) {
        return NULL;
    }

    file->listes = tabdyn_creer(nbPriorites);
    if (file->listes == NULL) {
        free(file);
        return NULL;
    }

    // creation des files et ajout dans le tabdyn
    for (int i = 0; i < nbPriorites; i++) {
        file_t *f = file_creer();
        if (f == NULL) {
            tabdyn_detruire(file->listes);
            free(file);
            return NULL;
        }
        tabdyn_ajouter(file->listes, f);
    }
    return file;
}

/**
 * Ajoute un élément à la fin de la file selon sa priorite.
 * @param file Pointeur non-nul vers la file.
 * @param element Pointeur vers l'élément à ajouter.
 * @param priorite niveau de priorité de l'élément (0 = plus prioritaire).
 * @return true si l'opération a réussi, false sinon.
 */
bool fileprio_enfiler(fileprio_t *file, void *element, uint8_t priorite) {
    assert(file != NULL);
    assert(priorite < tabdyn_taille(file->listes));

    return file_enfiler(tabdyn_obtenir(file->listes, priorite), element);
}

/**
 * Retire un élément au début de la file non-vide la plus prioritaire
 * et le retourne. Au moins une file doit être non-vide.
 * @param file Pointeur non-nul vers la file.
 * @return Pointeur vers l'élément retiré ou NULL si la file est vide.
 */
void *fileprio_defiler(fileprio_t *file) {
    assert(file != NULL);
    if (fileprio_est_vide(file)) { return NULL; }

    for (int i = 0; i < tabdyn_taille(file->listes); i++) {
        if (! file_est_vide(tabdyn_obtenir(file->listes, i))) {
            return file_defiler(tabdyn_obtenir(file->listes, i));
        }
    }
    return NULL;    // ne sera jamais atteint
}

/**
 * Regarde l'élément au début de la file sans le retirer.
 * @param file Pointeur non-nul vers la file.
 * @return Pointeur vers l'élément en tête ou NULL si la file est vide.
 */
//void *fileprio_regarder_tete(const fileprio_t *file);

/**
 * Libère la mémoire allouée pour une file générique.
 * @param file Pointeur vers la file à libérer. Si le pointeur est NULL,
 *             la fonction n'a pas d'effet (voir la définition de free()).
 */
//void fileprio_liberer(fileprio_t *file);

/**
 * Obtient le nombre d'éléments dans la fileprio.
 * @param file Pointeur non-nul vers la file.
 * @return Le nombre d'éléments dans la file.
 */
size_t fileprio_taille(const fileprio_t *file) {
    assert(file != NULL);
    int taille = 0;
    for (int i = 0; i < tabdyn_taille(file->listes); i++) {
        taille += file_taille(tabdyn_obtenir(file->listes, i));
    }
    return taille;
}

/**
 * Obtient le nombre de niveaux de priorite.
 * @param file Pointeur non-nul vers la file.
 * @return Le nombre de niveaux.
 */
size_t fileprio_niveaux(const fileprio_t *file) {
    assert(file != NULL);
    return tabdyn_taille(file->listes);
}

/**
 * Vérifie si la fileprio est vide.
 * @param file Pointeur non-nul vers la file.
 * @return true si la file est vide, false sinon.
 */
bool fileprio_est_vide(const fileprio_t *file) {
    return fileprio_taille(file) == 0;
}

/**
 * Affiche le contenu de la file dans un format utile au débogage.
 * @param file Pointeur non-nul vers la file.
 */
void fileprio_afficher(const fileprio_t *file) {
    assert(file != NULL);

    for (int i = 0; i < tabdyn_taille(file->listes); i++) {
        file_afficher(tabdyn_obtenir(file->listes, i));
    }
}
