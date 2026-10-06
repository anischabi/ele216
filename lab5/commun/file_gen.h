#ifndef FILE_GEN_H
#define FILE_GEN_H

#include <stdbool.h>

typedef struct file file_gen_t;

/**
 * @brief Crée une nouvelle file vide.
 * 
 * @return file_gen_t* Un pointeur vers la nouvelle file, NULL en cas d'erreur.
 */
file_gen_t *file_gen_creer();

/**
 * @brief Libère la mémoire de la file. Le programme appelant est responsable de 
 * libérer la mémoire des éléments.
 * 
 * 
 */
void file_gen_liberer(file_gen_t *f);

/**
 * @brief Ajoute un élément à la fin de la file.
 * 
 * @param f un pointeur non-nul vers la file à modifier.
 * @param element un pointeur non-nul vers l'élément à ajouter.
 */
void file_gen_enfiler(file_gen_t *f, void *element);

/**
 * @brief Retire et retourne le premier élément de la file.
 * 
 * @param f un pointeur non-nul vers la file à modifier.
 * @return void* un pointeur non-nul vers l'élément retiré.
 */
void *file_gen_defiler(file_gen_t *f);

/**
 * @brief Retourne le premier élément de la file.
 * 
 * @param f un pointeur non-nul vers la file à consulter.
 * @return void* un pointeur non-nul vers l'élément consulté.
 */
void *file_gen_regarder(file_gen_t *f);

// Accesseurs
bool file_gen_est_vide(file_gen_t *f);
int  file_gen_taille(file_gen_t *f);

#endif