#include <stdlib.h>

#include "probleme4.h"

int *creer_tableau(int n) {
    int *tableau = malloc(n * sizeof(int));
    // malloc peut échouer (mémoire insuffisante) et retourner NULL : on
    // vérifie et on propage NULL à l'appelant plutôt que d'écrire dans un
    // pointeur invalide.
    if (tableau == NULL) {
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        tableau[i] = i * 2;
    }

    return tableau;
}
