#ifndef COMPTEUR_H
#define COMPTEUR_H

/**
 * @file compteur.h
 * @brief Module de compteur d'événements avec seuil de dépassement.
 *
 * Chaque incrémentation rapproche la valeur courante du seuil. Lorsque la
 * valeur atteint le seuil, le nombre de dépassements augmente de 1 et la
 * valeur revient à 0 (comme un compteur qui compte ses tours).
 *
 * Le seuil est fixé à la création et ne peut plus changer. La valeur
 * courante est toujours comprise entre 0 et seuil - 1.
 */

/** Type opaque : les champs sont cachés dans compteur.c. */
typedef struct compteur compteur_t;

/**
 * Crée un compteur dont la valeur courante et le nombre de dépassements
 * valent 0.
 *
 * @param seuil nombre d'incrémentations qui provoquent un dépassement.
 * Doit être strictement positif.
 * @return un pointeur vers le compteur créé. NULL si seuil <= 0 ou en cas
 * d'erreur d'allocation mémoire.
 */
compteur_t *compteur_creer(int seuil);

/**
 * Libère la mémoire d'un compteur.
 *
 * @param c le compteur à détruire. Si le pointeur est NULL, la fonction n'a
 * pas d'effet (voir la définition de free()). Après l'appel, le pointeur de
 * l'appelant n'est plus valide.
 */
void compteur_detruire(compteur_t *c);

/**
 * Augmente la valeur courante de 1. Si la valeur atteint le seuil, le nombre
 * de dépassements augmente de 1 et la valeur revient à 0.
 *
 * @param c pointeur non-nul vers le compteur.
 */
void compteur_incrementer(compteur_t *c);

/**
 * Remet la valeur courante à 0. Le nombre de dépassements n'est PAS affecté.
 *
 * @param c pointeur non-nul vers le compteur.
 */
void compteur_reinitialiser(compteur_t *c);

/**
 * @param c pointeur non-nul vers le compteur.
 * @return la valeur courante, entre 0 et seuil - 1.
 */
int compteur_valeur(const compteur_t *c);

/**
 * @param c pointeur non-nul vers le compteur.
 * @return le seuil fixé à la création.
 */
int compteur_seuil(const compteur_t *c);

/**
 * @param c pointeur non-nul vers le compteur.
 * @return le nombre de dépassements depuis la création.
 */
int compteur_depassements(const compteur_t *c);

/**
 * Affiche tous les champs du compteur sur le terminal, sur une ligne.
 *
 * @param c pointeur non-nul vers le compteur.
 */
void compteur_afficher(const compteur_t *c);

#endif // COMPTEUR_H
