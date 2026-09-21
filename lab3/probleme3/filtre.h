#ifndef FILTRE_H
#define FILTRE_H

/**
 * @file filtre.h
 * @brief Module de filtre passe-bas par moyenne mobile.
 *
 * Le filtre garde en mémoire les derniers échantillons reçus, dans une
 * fenêtre dont la taille est fixée à la création. Sa valeur est la moyenne des
 * échantillons de la fenêtre. Tant que la fenêtre n'est pas remplie, la
 * moyenne porte uniquement sur les échantillons reçus jusque-là.
 */

/** Type opaque : les champs sont cachés dans filtre.c. */
typedef struct filtre filtre_t;

/**
 * Crée un filtre vide (aucun échantillon reçu). Alloue la structure ET son
 * tableau interne.
 *
 * @param taille_fenetre nombre d'échantillons de la fenêtre. Doit être
 * strictement positif.
 * @return un pointeur vers le filtre créé. NULL si taille_fenetre <= 0 ou en
 * cas d'erreur d'allocation mémoire.
 */
filtre_t *filtre_creer(int taille_fenetre);

/**
 * Libère la mémoire du filtre ET de son tableau interne.
 *
 * @param f le filtre à détruire. Si le pointeur est NULL, la fonction n'a pas
 * d'effet (voir la définition de free()). Après l'appel, le pointeur de
 * l'appelant n'est plus valide.
 */
void filtre_detruire(filtre_t *f);

/**
 * Ajoute un échantillon au filtre. Lorsque la fenêtre est pleine, le plus
 * ancien échantillon est remplacé.
 *
 * @param f          pointeur non-nul vers le filtre.
 * @param echantillon la nouvelle valeur.
 */
void filtre_ajouter(filtre_t *f, float echantillon);

/**
 * Calcule la moyenne des échantillons actuellement dans la fenêtre.
 *
 * @param f pointeur non-nul vers le filtre.
 * @return la moyenne des échantillons disponibles (au plus taille_fenetre, les
 * plus récents). 0 si aucun échantillon n'a été ajouté.
 */
float filtre_valeur(const filtre_t *f);

/**
 * @param f pointeur non-nul vers le filtre.
 * @return la taille de la fenêtre (immuable).
 */
int filtre_taille_fenetre(const filtre_t *f);

/**
 * @param f pointeur non-nul vers le filtre.
 * @return le nombre d'échantillons dans la fenêtre courante : le nombre
 * d'échantillons reçus, plafonné à la taille de la fenêtre.
 */
int filtre_nb_echantillons(const filtre_t *f);

/**
 * Affiche sur la sortie standard les échantillons de la fenêtre, du plus
 * ancien au plus récent, ainsi que la moyenne courante, dans un format utile
 * au débogage.
 *
 * @param f pointeur non-nul vers le filtre.
 */
void filtre_afficher(const filtre_t *f);

#endif // FILTRE_H
