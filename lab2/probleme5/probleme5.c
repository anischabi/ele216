#include <stdio.h>
#include <stdlib.h>

#include "probleme5.h"

int **creer_matrice(int lignes, int colones) {
    // Première allocation : le tableau de POINTEURS, une case par ligne.
    int **matrice = malloc(lignes * sizeof(int *));
    if (matrice == NULL) {
        return NULL;
    }

    // Deuxième série d'allocations : chaque ligne est allouée
    // séparément, potentiellement à une adresse mémoire complètement
    // différente des autres lignes.
    for (int i = 0; i < lignes; i++) {
        matrice[i] = malloc(colones * sizeof(int));

        if (matrice[i] == NULL) {
            // Une ligne a échoué : on libère tout ce qui a déjà été
            // alloué avec succès (les lignes 0 à i-1) avant de
            // retourner NULL, pour éviter une fuite mémoire partielle.
            for (int j = 0; j < i; j++) {
                free(matrice[j]);
            }
            free(matrice);
            return NULL;
        }
    }

    return matrice;
}

void liberer_matrice(int **matrice, int lignes) {
    // On libère chaque ligne D'ABORD
    for (int i = 0; i < lignes; i++) {
        free(matrice[i]);
    }
    // PUIS le tableau de pointeurs lui-même. L'ordre inverse
    // libérerait le tableau de pointeurs en premier, perdant les
    // adresses des lignes : elles resteraient allouées mais
    // inaccessibles pour toujours (fuite mémoire irrécupérable).
    free(matrice);
}

void remplir_matrice_aleatoire(int **matrice, int lignes, int colones, int min, int max) {
    // Cette fonction ne fait qu'appeler rand() — elle suppose que la
    // "graine" (seed) du générateur a déjà été initialisée UNE SEULE FOIS
    // ailleurs dans le programme (ici, dans main(), avant l'appel à cette
    // fonction) :
    //
    //     #include <time.h>
    //     srand((unsigned int) time(NULL));
    //
    // - time(NULL) retourne l'heure actuelle (nombre de secondes depuis le
    //   1er janvier 1970), ce qui donne une graine différente à chaque
    //   exécution du programme.
    // - srand() ne doit être appelée QU'UNE FOIS, en début de programme.
    //   L'appeler à répétition (ex. ici, dans cette boucle) ré-initialise
    //   le générateur à chaque itération, ce qui produit des valeurs bien
    //   moins aléatoires (souvent identiques d'un appel à l'autre).
    // - Si on ne l'appelle jamais, rand() se comporte comme si
    //   srand(1) avait été appelé : la MÊME séquence de nombres est
    //   produite à chaque exécution du programme (utile pour du débogage
    //   reproductible, mais pas pour un vrai résultat "aléatoire").
    for (int i = 0; i < lignes; i++) {
        for (int j = 0; j < colones; j++) {
            // rand() % (max - min + 1) donne une valeur dans [0, max-min],
            // on ajoute min pour décaler l'intervalle vers [min, max].
            matrice[i][j] = rand() % (max - min + 1) + min;
        }
    }
}

void afficher_matrice_dyn(int **matrice, int lignes, int colones) {
    for (int i = 0; i < lignes; i++) {
        for (int j = 0; j < colones; j++) {
            printf("%4d", matrice[i][j]);
        }
        printf("\n");
    }
}
