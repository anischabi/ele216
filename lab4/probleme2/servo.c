#include "servo.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Définition de la structure dans le .c : les champs sont invisibles de
// l'extérieur du module. Comme aucune fonction ne modifie angle_min,
// angle_max et vitesse après servo_creer, ces champs sont immuables. Seule
// servo_aller_a modifie angle, et elle maintient l'invariant
// angle_min <= angle <= angle_max.
struct servo {
    float angle;            // en degrés, toujours dans [angle_min, angle_max]
    float angle_min;        // en degrés, fixé à la création
    float angle_max;        // en degrés, fixé à la création, > angle_min
    float vitesse;          // en degrés/s, fixée à la création, > 0
};

servo_t *servo_creer(float angle_min, float angle_max, float vitesse) {
    // Écrit avec des négations plutôt que (angle_min >= angle_max || vitesse <= 0)
    // pour refuser aussi NaN : toute comparaison avec NaN est fausse.
    if (!(angle_min < angle_max) || !(vitesse > 0.0f)) { return NULL; }

    servo_t *s = malloc(sizeof(servo_t));
    if (s == NULL) { return NULL; }

    s->angle = angle_min;
    s->angle_min = angle_min;
    s->angle_max = angle_max;
    s->vitesse = vitesse;
    return s;
}

void servo_detruire(servo_t *s) {
    free(s);    // free(NULL) est sans effet
}

void servo_aller_a(servo_t *s, float angle) {
    assert(s != NULL);
    if (isnan(angle)) { return; }   // sinon angle deviendrait NaN

    if (angle < s->angle_min) {
        s->angle = s->angle_min;
    } else if (angle > s->angle_max) {
        s->angle = s->angle_max;
    } else {
        s->angle = angle;
    }
}

float servo_temps_pour_atteindre(const servo_t *s, float angle) {
    assert(s != NULL);
    // Même astuce que servo_creer : un angle NaN est traité comme hors butées.
    if (!(angle >= s->angle_min && angle <= s->angle_max)) { return -1.0f; }

    // vitesse > 0 est garanti par servo_creer : pas de division par zéro.
    return fabsf(angle - s->angle) / s->vitesse;
}

float servo_angle(const servo_t *s) {
    assert(s != NULL);
    return s->angle;
}

float servo_angle_min(const servo_t *s) {
    assert(s != NULL);
    return s->angle_min;
}

float servo_angle_max(const servo_t *s) {
    assert(s != NULL);
    return s->angle_max;
}

float servo_vitesse(const servo_t *s) {
    assert(s != NULL);
    return s->vitesse;
}

void servo_afficher(const servo_t *s) {
    assert(s != NULL);
    printf("Servo { angle = %.1f deg, butees = [%.1f, %.1f] deg, vitesse = %.1f deg/s }\n",
           s->angle, s->angle_min, s->angle_max, s->vitesse);
}
