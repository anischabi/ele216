#include <stdio.h>

#include "probleme6.h"

#define NB_POINTS 5

int main(void) {
    // Tableau STATIQUE : la taille (5 points) est connue et fixe à la
    // compilation, pas besoin de la flexibilité d'un tableau dynamique ici
    // (contrairement aux problèmes 4 et 5).
    point2D_t points[NB_POINTS] = {
        {0.0f, 0.0f},
        {3.0f, 4.0f},
        {3.0f, 0.0f},
        {6.0f, 4.0f},
        {10.0f, 4.0f},
    };

    float distance_totale = 0.0f;

    // On calcule les distances entre points CONSÉCUTIFS : (0,1), (1,2),
    // (2,3), (3,4) — donc 4 distances pour 5 points. La boucle doit
    // s'arrêter à NB_POINTS - 1 (donc i < NB_POINTS - 1), sinon on
    // accéderait à points[NB_POINTS], qui n'existe pas.
    for (int i = 0; i < NB_POINTS - 1; i++) {
        float d = distance(&points[i], &points[i + 1]);
        printf("Distance %d -> %d : %.2f\n", i + 1, i + 2, d);
        distance_totale += d;
    }

    printf("Distance totale : %.2f\n", distance_totale);

    return 0;
}
