#ifndef SERVO_H
#define SERVO_H

/**
 * @file servo.h
 * @brief Module de servo-moteur avec butées physiques et vitesse constante.
 *
 * Les butées (angle minimal et maximal) et la vitesse angulaire sont des
 * caractéristiques matérielles fixées à la création : elles ne peuvent plus
 * changer. L'angle courant est toujours compris entre les deux butées.
 * Tous les angles sont en degrés et la vitesse en degrés par seconde.
 */

/** Type opaque : les champs sont cachés dans servo.c. */
typedef struct servo servo_t;

/**
 * Crée un servo-moteur. L'angle courant initial est égal à l'angle minimum.
 *
 * @param angle_min butée minimale, en degrés.
 * @param angle_max butée maximale, en degrés. Doit être strictement
 * supérieure à angle_min.
 * @param vitesse   vitesse angulaire, en degrés/s. Doit être strictement
 * positive.
 * @return un pointeur vers le servo créé. NULL si angle_min >= angle_max,
 * si vitesse <= 0, si un paramètre vaut NaN ou en cas d'erreur d'allocation
 * mémoire.
 */
servo_t *servo_creer(float angle_min, float angle_max, float vitesse);

/**
 * Libère la mémoire d'un servo.
 *
 * @param s le servo à détruire. Si le pointeur est NULL, la fonction n'a pas
 * d'effet (voir la définition de free()). Après l'appel, le pointeur de
 * l'appelant n'est plus valide.
 */
void servo_detruire(servo_t *s);

/**
 * Déplace le servo à l'angle demandé. Une consigne inférieure à l'angle
 * minimum est saturée à l'angle minimum, et une consigne supérieure à
 * l'angle maximum est saturée à l'angle maximum. Une consigne NaN est
 * ignorée.
 *
 * @param s     pointeur non-nul vers le servo.
 * @param angle consigne, en degrés.
 */
void servo_aller_a(servo_t *s, float angle);

/**
 * Calcule le temps nécessaire pour atteindre un angle depuis la position
 * courante, sans modifier l'état du servo : |angle - angle courant| / vitesse.
 *
 * @param s     pointeur non-nul vers le servo.
 * @param angle angle à atteindre, en degrés.
 * @return le temps en secondes (>= 0). Une valeur négative si l'angle est en
 * dehors des butées (ou vaut NaN).
 */
float servo_temps_pour_atteindre(const servo_t *s, float angle);

/**
 * @param s pointeur non-nul vers le servo.
 * @return l'angle courant, en degrés, entre angle_min et angle_max.
 */
float servo_angle(const servo_t *s);

/**
 * @param s pointeur non-nul vers le servo.
 * @return la butée minimale, en degrés.
 */
float servo_angle_min(const servo_t *s);

/**
 * @param s pointeur non-nul vers le servo.
 * @return la butée maximale, en degrés.
 */
float servo_angle_max(const servo_t *s);

/**
 * @param s pointeur non-nul vers le servo.
 * @return la vitesse angulaire, en degrés/s.
 */
float servo_vitesse(const servo_t *s);

/**
 * Affiche tous les champs du servo sur le terminal, sur une ligne.
 *
 * @param s pointeur non-nul vers le servo.
 */
void servo_afficher(const servo_t *s);

#endif // SERVO_H
