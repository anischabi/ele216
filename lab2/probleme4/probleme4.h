#ifndef PROBLEME4_H
#define PROBLEME4_H

/**
 * Alloue dynamiquement un tableau de n entiers, et initialise chaque
 * élément à l'indice i * 2.
 *
 * @param n taille du tableau à créer.
 * @return pointeur vers le tableau alloué (à libérer par l'appelant avec
 *         free()). NULL si l'allocation échoue.
 */
int *creer_tableau(int n);

#endif // PROBLEME4_H
