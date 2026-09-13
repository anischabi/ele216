#ifndef TABLEAUDYNAMIQUE_H
#define TABLEAUDYNAMIQUE_H

typedef struct tabdyn {
    int capacite;           // L'espace de stockage actuel
    int taille;             // L'espace utilisé
    int *contenu;           // Pointeur vers les données
} tabdyn_t;

/**
 * Crée une instance de tableau dynamique et réserve la mémoire nécessaire
 * pour stocker taille_initiale valeurs.
 *
 * @param taille_initiale le nombre de cases disponibles dans le tableau.
 * @return un pointeur vers le tableau initialisé. NULL en cas d'erreur d'allocation
 * mémoire.
 */
tabdyn_t *tabdyn_creer(int taille_initiale);

/**
 * Ajoute un élément dans le tableau dynamique.
 * Si l'ajout entraîne un dépassement de capacité, la taille du tableau
 * est doublée. Si l'agrandissement est impossible, le tableau n'est pas
 * modifié et cette fonction n'a pas d'effet.
 *
 * @param tabdyn pointeur non-nul vers le tableau dynamique.
 * @param valeur L'élément à insérer.
 * @return nombre d'élément si l'ajout est un succès. 0 si l'ajout est impossible.
 */
int tabdyn_ajouter(tabdyn_t* tabdyn, int valeur);

/**
 * Libère la mémoire du tableau dynamique.
 * @param tabdyn Le tableau à libérer. Si le pointeur est NULL, la fonction
 * n'a pas d'effet. (voir la définition de free()).
 */
void tabdyn_liberer(tabdyn_t* tabdyn);

/**
 * Affiche le contenu d'un tableau dynamique dans un format utile au débogage.
 * @param tabdyn pointeur non-nul vers le tableau dynamique.
 */
void tabdyn_afficher(tabdyn_t* tabdyn);

#endif // TABLEAUDYNAMIQUE_H

