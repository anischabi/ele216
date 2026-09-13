#ifndef PROBLEME5_H
#define PROBLEME5_H

/**
 * Alloue dynamiquement une matrice d'entiers : un tableau de pointeurs
 * (une entrée par ligne), où chaque ligne est ELLE-MÊME allouée
 * séparément. Contrairement à un tableau 2D statique (int mat[][N]), les
 * lignes ne sont PAS garanties contiguës en mémoire.
 *
 * NOTE : "colones" reprend l'orthographe utilisée dans l'énoncé du
 * laboratoire (au lieu de "colonnes") pour respecter la signature imposée.
 *
 * @param lignes  nombre de lignes de la matrice.
 * @param colones nombre de colonnes de la matrice.
 * @return pointeur vers la matrice allouée (tableau de `lignes` pointeurs,
 *         chacun menant à `colones` entiers), à libérer avec
 *         liberer_matrice(). NULL si une allocation échoue (aucune fuite :
 *         tout ce qui a déjà été alloué est libéré avant de retourner).
 */
int **creer_matrice(int lignes, int colones);

/**
 * Libère la mémoire d'une matrice allouée par creer_matrice.
 * Libère chaque ligne, PUIS le tableau de pointeurs lui-même (l'ordre
 * inverse causerait une fuite : on perdrait les adresses des lignes).
 *
 * @param matrice la matrice à libérer.
 * @param lignes  nombre de lignes de la matrice (pas besoin du nombre de
 *                colonnes : chaque ligne a été allouée indépendamment,
 *                free() sait combien libérer sans qu'on le lui redise).
 */
void liberer_matrice(int **matrice, int lignes);

/**
 * Remplit une matrice avec des valeurs aléatoires dans l'intervalle
 * [min, max] (inclusif).
 * @param matrice matrice à remplir (déjà allouée).
 * @param lignes  nombre de lignes.
 * @param colones nombre de colonnes.
 * @param min     valeur minimale possible.
 * @param max     valeur maximale possible.
 */
void remplir_matrice_aleatoire(int **matrice, int lignes, int colones, int min, int max);

/**
 * Affiche une matrice dynamique (int **) sous forme de grille alignée.
 * NOTE : distincte de afficher_matrice du problème 2, qui attend un
 * tableau 2D STATIQUE (int mat[][N]) — les deux types ne sont pas
 * interchangeables en C, même si l'accès mat[i][j] se ressemble.
 * @param matrice matrice à afficher.
 * @param lignes  nombre de lignes.
 * @param colones nombre de colonnes.
 */
void afficher_matrice_dyn(int **matrice, int lignes, int colones);

#endif // PROBLEME5_H
