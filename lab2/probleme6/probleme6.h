#ifndef PROBLEME6_H
#define PROBLEME6_H

typedef struct {
    float x;
    float y;
} point2D_t;

/**
 * Calcule la distance euclidienne entre deux points.
 *
 * Reçoit des pointeurs (const, puisqu'on ne fait que LIRE les points)
 * plutôt que des point2D_t par valeur, pour éviter de copier la struct à
 * chaque appel.
 *
 * @param a premier point.
 * @param b deuxième point.
 * @return la distance entre a et b.
 */
float distance(const point2D_t *a, const point2D_t *b);

#endif // PROBLEME6_H
