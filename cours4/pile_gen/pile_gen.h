#ifndef PILE_GEN_H
#define PILE_GEN_H

#include <stdbool.h>

typedef struct pile pile_gen_t;

/**
 * @brief Crée un nouvelle pile vide.
 * 
 * @return pile_gen_t* Un pointeur vers la nouvelle pile, NULL en cas d'erreur.
 */
pile_gen_t *pile_gen_creer();

/**
 * @brief Libère la mémoire de la pile. Le programme appelant est responsable de 
 * libérer la mémoire des éléments.
 * 
 */
void pile_gen_liberer(pile_gen_t *p);

/**
 * @brief Ajoute un élément sur la pile.
 * 
 * @param p un pointeur non-nul vers la pile à modifier.
 * @param element un pointeur non-nul vers l'élément à empiler.
 */
void pile_gen_empiler(pile_gen_t *p, void *element);

/**
 * @brief Retire l'élément du dessus de la pile.
 * 
 * @param p un pointeur non-nul vers la pile à modifier.
 * @return void* un pointeur non-nul vers l'élément retiré.
 */
void *pile_gen_depiler(pile_gen_t *p);

/**
 * @brief Retourne l'élément du dessus de la pile, sans le retirer.
 * 
 * @param p un pointeur non-nul vers la pile à modifier.
 * @return void* un pointeur non-nul vers l'élément consulté.
 */
void *pile_gen_regarder(pile_gen_t *p);

// Accesseurs
bool pile_gen_est_vide(pile_gen_t *p);
int  pile_gen_taille(pile_gen_t *p);

#endif