#ifndef PROBLEME1_H
#define PROBLEME1_H

/**
 * Calcule la somme des éléments d'un tableau.
 * @param tab tableau d'entiers.
 * @param n   nombre d'éléments dans le tableau.
 * @return la somme des éléments.
 */
int somme_tableau(int tab[], int n);

/**
 * Trouve l'indice de la valeur maximale d'un tableau.
 * @param tab tableau d'entiers.
 * @param n   nombre d'éléments dans le tableau.
 * @return l'indice du premier élément trouvé égal au maximum.
 */
int indice_max(int tab[], int n);

/**
 * Affiche les éléments d'un tableau sur une seule ligne, séparés par des
 * espaces (pas d'espace après le dernier élément).
 * @param tab tableau d'entiers.
 * @param n   nombre d'éléments dans le tableau.
 */
void afficher_tableau(int tab[], int n);

#endif // PROBLEME1_H
