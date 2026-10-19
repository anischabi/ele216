#ifndef FILE_GEN_H
#define FILE_GEN_H

#include <stdbool.h>

typedef struct file file_t;

/**
 * @brief Crée une nouvelle file vide.
 * 
 * @return file_t* Un pointeur vers la nouvelle file, NULL en cas d'erreur.
 */
file_t *file_creer();

/**
 * @brief Libère la mémoire de la file. Le programme appelant est responsable de 
 * libérer la mémoire des éléments.
 * 
 * 
 */
void file_liberer(file_t *f);

/**
 * @brief Ajoute un élément à la fin de la file.
 * 
 * @param f un pointeur non-nul vers la file à modifier.
 * @param element un pointeur non-nul vers l'élément à ajouter.
 */
void file_enfiler(file_t *f, void *element);

/**
 * @brief Retire et retourne le premier élément de la file.
 * 
 * @param f un pointeur non-nul vers la file à modifier.
 * @return void* un pointeur non-nul vers l'élément retiré.
 */
void *file_defiler(file_t *f);

/**
 * @brief Retourne le premier élément de la file.
 * 
 * @param f un pointeur non-nul vers la file à consulter.
 * @return void* un pointeur non-nul vers l'élément consulté.
 */
void *file_regarder(file_t *f);

// Accesseurs
bool file_est_vide(file_t *f);
int  file_taille(file_t *f);

#endif