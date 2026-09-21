#ifndef VECTEUR_H
#define VECTEUR_H

#include <stdbool.h>

// Type opaque pour interdire l'accès au contenu de la structure de l'extérieur du module
typedef struct vecteur vecteur_t;

/**
 * Crée une instance de tableau dynamique et réserve la mémoire nécessaire
 * pour stocker taille_initiale valeurs. Taille_initiale doit être strictement
 * supérieure à zéro.
 *
 * @param taille_initiale le nombre de cases disponibles dans le tableau.
 * @return un pointeur vers le tableau initialisé. NULL en cas d'erreur d'allocation
 * mémoire ou de taille_initiale inférieure ou égale à 0. 
 */
vecteur_t *vecteur_creer(int taille_initiale);

/**
 * Ajoute un élément dans le tableau dynamique.
 * Si l'ajout entraîne un dépassement de capacité, la taille du tableau
 * est doublée. Si l'agrandissement est impossible, le tableau n'est pas
 * modifié et cette fonction n'a pas d'effet.
 *
 * @param vect pointeur non-nul vers le tableau dynamique.
 * @param valeur L'élément à insérer.
 * @return nombre d'élément si l'ajout est un succès. 0 si l'ajout est impossible.
 */
int vecteur_ajouter(vecteur_t* vect, int valeur);

/**
 * Libère la mémoire du tableau dynamique.
 * @param vect Le tableau à libérer. Si le pointeur est NULL, la fonction
 * n'a pas d'effet. (voir la définition de free()).
 */
void vecteur_liberer(vecteur_t* vect);

/**
 * Affiche le contenu d'un tableau dynamique dans un format utile au débogage.
 * @param vect pointeur non-nul vers le tableau dynamique.
 */
void vecteur_afficher(vecteur_t* vect);

// Accesseurs
int vecteur_taille(vecteur_t* vect);
bool vecteur_est_vide(vecteur_t *vect);
int vecteur_capacite(vecteur_t* vect);

#endif // VECTEUR_H

