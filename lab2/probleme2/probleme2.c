#include <stdio.h>

#include "probleme2.h"

void afficher_matrice(int mat[][NB_COLONNES], int lignes) {
    for (int i = 0; i < lignes; i++) {
        for (int j = 0; j < NB_COLONNES; j++) {
            // Largeur de champ fixe (%4d) pour que les colonnes restent
            // alignées même si les valeurs n'ont pas le même nombre de
            // chiffres (contrairement à afficher_tableau du problème 1, où
            // un simple espace suffisait).
            printf("%4d", mat[i][j]);
        }
        printf("\n");
    }
}

void sommes_lignes(int mat[][NB_COLONNES], int lignes, int resultats[]) {
    // Une ligne est contiguë en mémoire : on additionne simplement ses
    // NB_COLONNES éléments.
    for (int i = 0; i < lignes; i++) {
        int somme = 0;
        for (int j = 0; j < NB_COLONNES; j++) {
            somme += mat[i][j];
        }
        resultats[i] = somme;
    }
}

/*
 * Deux façons valides de calculer les sommes de colonnes :
 *
 * |                      | "colonne par colonne" (v1)                | "ligne par ligne" (v2, utilisée ci-dessous) |
 * |----------------------|-------------------------------------------|----------------------------------------------|
 * | Ordre de parcours    | Non contigu (saute de NB_COLONNES cases)  | Contigu (row-major, ordre naturel en mémoire) |
 * | Init à 0 nécessaire ?| Non (somme locale calculée par colonne)   | Oui (resultats[j] = 0 avant d'accumuler)      |
 * | Lisibilité           | Symétrique avec sommes_lignes             | Même structure de boucle que afficher_matrice |
 *
 * Les deux donnent exactement le même résultat. v2 est gardée comme
 * implémentation "officielle" (sommes_colonnes) parce qu'elle accède à la
 * mémoire dans l'ordre où elle est réellement stockée ; v1 est conservée
 * ci-dessous (sommes_colonnes_par_colonne) à titre de comparaison.
 */

void sommes_colonnes(int mat[][NB_COLONNES], int lignes, int resultats[]) {
    // On initialise chaque case à 0, car on va accumuler dans resultats[j]
    // au fil des lignes (une ligne à la fois), plutôt que de calculer la
    // somme complète d'une colonne avant de l'assigner.
    for (int j = 0; j < NB_COLONNES; j++) {
        resultats[j] = 0;
    }

    // On parcourt la matrice ligne par ligne (l'ordre où elle est stockée
    // en mémoire, donc de façon contiguë), et pour chaque élément on
    // l'ajoute directement à la somme de sa colonne.
    for (int i = 0; i < lignes; i++) {
        for (int j = 0; j < NB_COLONNES; j++) {
            resultats[j] += mat[i][j];
        }
    }
}

// Version alternative (v1) : conservée à titre de comparaison, voir le
// tableau ci-dessus. C'est un deuxième exemple gardé pour la discussion en labo.
void sommes_colonnes_par_colonne(int mat[][NB_COLONNES], int lignes, int resultats[]) {
    // Une colonne n'est PAS contiguë en mémoire (les éléments mat[0][j],
    // mat[1][j], sont espacés de NB_COLONNES cases) : on parcourt donc
    // la matrice colonne par colonne, ligne par ligne.
    for (int j = 0; j < NB_COLONNES; j++) {
        int somme = 0;
        for (int i = 0; i < lignes; i++) {
            somme += mat[i][j];
        }
        resultats[j] = somme;
    }
}
