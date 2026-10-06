#include "resultat.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Définition de la structure dans le .c : impossible d'accéder aux champs
// de l'extérieur du module, donc impossible de modifier le verdict.
struct resultat {
    char *id;         // Copie de l'identifiant (allouée dynamiquement)
    float attendu;    // Valeur nominale
    float mesure;     // Valeur mesurée
    float tolerance;  // Tolérance en pourcentage
    int reussi;       // Verdict calculé à la création : 1 = PASS, 0 = FAIL
};

static int calculer_verdict(float attendu, float mesure, float tolerance);

resultat_t *resultat_creer(const char *id, float attendu, float mesure,
                           float tolerance) {
    if (id == NULL) { return NULL; }

    resultat_t *r = malloc(sizeof(resultat_t));
    if (r == NULL) { return NULL; }

    // Copie profonde de l'identifiant : on ne garde pas le pointeur de
    // l'appelant, qui pourrait pointer vers un tampon temporaire.
    size_t longueur = strlen(id) + 1;
    r->id = malloc(longueur);
    if (r->id == NULL) {
        free(r);
        return NULL;
    }
    memcpy(r->id, id, longueur);

    r->attendu = attendu;
    r->mesure = mesure;
    r->tolerance = tolerance;
    r->reussi = calculer_verdict(attendu, mesure, tolerance);
    return r;
}

/**
 * Calcule le verdict : PASS si |mesure - attendu| <= |attendu| * tolérance / 100.
 *
 * La valeur absolue de attendu garde l'intervalle dans le bon ordre quand la
 * valeur attendue est négative (ex. une tension de -12 V). Une mesure NaN
 * rend la comparaison fausse, donc FAIL.
 *
 * @return 1 si PASS, 0 si FAIL.
 */
static int calculer_verdict(float attendu, float mesure, float tolerance) {
    float ecart_max = fabsf(attendu) * tolerance / 100.0f;
    return fabsf(mesure - attendu) <= ecart_max;
}

void resultat_detruire(resultat_t *r) {
    if (r == NULL) { return; }
    free(r->id);
    free(r);
}

const char *resultat_id(const resultat_t *r) {
    assert(r != NULL);
    return r->id;
}

float resultat_attendu(const resultat_t *r) {
    assert(r != NULL);
    return r->attendu;
}

float resultat_mesure(const resultat_t *r) {
    assert(r != NULL);
    return r->mesure;
}

float resultat_tolerance(const resultat_t *r) {
    assert(r != NULL);
    return r->tolerance;
}

int resultat_est_reussi(const resultat_t *r) {
    assert(r != NULL);
    return r->reussi;
}

void resultat_afficher(const resultat_t *r) {
    assert(r != NULL);
    printf("%-12s attendu = %8.3f  mesure = %8.3f  tol = %5.1f %%  %s\n",
           r->id, r->attendu, r->mesure, r->tolerance,
           r->reussi ? "PASS" : "FAIL");
}
