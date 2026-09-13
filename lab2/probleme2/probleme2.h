#ifndef PROBLEME2_H
#define PROBLEME2_H

// Nombre de colonnes fixé pour toutes les matrices de ce problème.
// Le compilateur a besoin de connaître ce nombre à la compilation pour
// pouvoir calculer l'adresse de mat[i][j] (la mémoire d'un tableau 2D est
// un seul bloc contigu, rangé ligne par ligne).
#define NB_COLONNES 5

/**
 * Affiche une matrice 2D sous forme de grille (lignes et colonnes alignées).
 * @param mat    la matrice, NB_COLONNES colonnes par ligne.
 * @param lignes le nombre de lignes de la matrice.
 */
void afficher_matrice(int mat[][NB_COLONNES], int lignes);

/**
 * Calcule la somme de chaque ligne de la matrice.
 * @param mat       la matrice, NB_COLONNES colonnes par ligne.
 * @param lignes    le nombre de lignes de la matrice.
 * @param resultats tableau (déjà alloué par l'appelant) d'au moins
 *                  `lignes` éléments ; resultats[i] recevra la somme de la
 *                  ligne i.
 */
void sommes_lignes(int mat[][NB_COLONNES], int lignes, int resultats[]);

/**
 * Calcule la somme de chaque colonne de la matrice.
 * @param mat       la matrice, NB_COLONNES colonnes par ligne.
 * @param lignes    le nombre de lignes de la matrice.
 * @param resultats tableau (déjà alloué par l'appelant) d'au moins
 *                  NB_COLONNES éléments ; resultats[j] recevra la somme de
 *                  la colonne j.
 */
void sommes_colonnes(int mat[][NB_COLONNES], int lignes, int resultats[]);

/**
 * Version alternative de sommes_colonnes, gardée uniquement à titre de
 * comparaison pédagogique (voir le tableau comparatif dans probleme2.c) —
 * ne fait PAS partie de l'API demandée par l'énoncé du laboratoire.
 * Parcourt la matrice colonne par colonne plutôt que ligne par ligne.
 * @param mat       la matrice, NB_COLONNES colonnes par ligne.
 * @param lignes    le nombre de lignes de la matrice.
 * @param resultats tableau (déjà alloué par l'appelant) d'au moins
 *                  NB_COLONNES éléments ; resultats[j] recevra la somme de
 *                  la colonne j.
 */
void sommes_colonnes_par_colonne(int mat[][NB_COLONNES], int lignes, int resultats[]);

#endif // PROBLEME2_H
