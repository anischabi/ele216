#ifndef PILE_H
#define PILE_H

/**
 * @file pile.h
 * @brief Module de pile rechargeable à capacité bornée.
 *
 * La capacité maximale (mAh) et la tension nominale (V) sont fixées à la
 * création et ne peuvent plus changer. La charge courante est toujours
 * comprise entre 0 et la capacité maximale.
 */

/** Type opaque : les champs sont cachés dans pile.c. */
typedef struct pile pile_t;

/**
 * Crée une pile rechargeable, initialement déchargée (charge = 0).
 *
 * @param capacite_mAh capacité maximale en mAh. Doit être strictement positive.
 * @param tension_V    tension nominale en V. Doit être strictement positive.
 * @return un pointeur vers la pile créée. NULL si un paramètre est invalide
 * (<= 0) ou en cas d'erreur d'allocation mémoire.
 */
pile_t *pile_creer(float capacite_mAh, float tension_V);

/**
 * Libère la mémoire d'une pile.
 *
 * @param p la pile à détruire. Si le pointeur est NULL, la fonction n'a pas
 * d'effet (voir la définition de free()). Après l'appel, le pointeur de
 * l'appelant n'est plus valide.
 */
void pile_detruire(pile_t *p);

/**
 * Ajoute de la charge à la pile. La charge est saturée à la capacité
 * maximale. Une valeur négative ou nulle est ignorée.
 *
 * @param p   pointeur non-nul vers la pile.
 * @param mAh quantité de charge à ajouter, en mAh.
 */
void pile_charger(pile_t *p, float mAh);

/**
 * Retire de la charge à la pile. La charge ne descend jamais sous 0.
 * Une valeur négative ou nulle est ignorée.
 *
 * @param p   pointeur non-nul vers la pile.
 * @param mAh quantité de charge à retirer, en mAh.
 */
void pile_decharger(pile_t *p, float mAh);

/**
 * @param p pointeur non-nul vers la pile.
 * @return la capacité maximale, en mAh.
 */
float pile_capacite_max(const pile_t *p);

/**
 * @param p pointeur non-nul vers la pile.
 * @return la charge courante, en mAh.
 */
float pile_charge_courante(const pile_t *p);

/**
 * @param p pointeur non-nul vers la pile.
 * @return la tension nominale, en V.
 */
float pile_tension(const pile_t *p);

/**
 * Calcule le niveau de charge en pourcentage de la capacité maximale.
 *
 * @param p pointeur non-nul vers la pile.
 * @return charge courante / capacité maximale * 100, soit une valeur entre
 * 0 et 100.
 */
float pile_pourcentage(const pile_t *p);

/**
 * Affiche tous les champs de la pile sur la sortie standard, dans un format
 * utile au débogage.
 *
 * @param p pointeur non-nul vers la pile.
 */
void pile_afficher(const pile_t *p);

#endif // PILE_H
