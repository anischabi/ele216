#include <math.h>

#include "probleme6.h"

float distance(const point2D_t *a, const point2D_t *b) {
    float dx = b->x - a->x;
    float dy = b->y - a->y;
    // Distance euclidienne : sqrt((b.x - a.x)^2 + (b.y - a.y)^2).
    // sqrtf/powf existent en versions "float" (plutôt que sqrt/pow qui sont
    // en double) ; ici sqrtf suffit puisque nos champs sont des float.
    return sqrtf(dx * dx + dy * dy);
}
