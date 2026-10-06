/**
 * @file resultat_test.c
 * @brief Tests unitaires (Unity) du module resultat.
 *
 * Chaque test crée et détruit ses propres résultats (le module n'a pas
 * d'état modifiable, donc pas besoin de fixture commune). Les valeurs
 * choisies (100, 5 %, 105, 95...) sont exactement représentables en
 * float, pour que les tests aux bornes ne dépendent pas d'erreurs
 * d'arrondi. Les tests suivent l'ordre de l'énoncé.
 */

#include <stdio.h>
#include <string.h>

#include "resultat.h"
#include "unity.h"

/** Précision des comparaisons de float pour les accesseurs. */
#define EPSILON 1e-6f

/** Fixture vide : chaque test gère ses propres résultats. */
void setUp(void) {}

/** Fixture vide : chaque test détruit ses propres résultats. */
void tearDown(void) {}

/* ---------- Création valide et accesseurs ---------- */

/**
 * @brief Une création valide conserve tous les champs.
 *
 * - Scénario : resultat_creer("V_REF", 3.3, 3.35, 5).
 * - Attendu  : pointeur non NULL, chaque accesseur retourne la valeur reçue,
 *              verdict PASS (écart 0.05 <= 0.165).
 * - Détecte  : un accesseur qui retourne le mauvais champ (ex. attendu et
 *              mesure inversés).
 */
void test_resultat_creer_valide(void) {
    resultat_t *r = resultat_creer("V_REF", 3.3f, 3.35f, 5.0f);
    TEST_ASSERT_NOT_NULL(r);
    TEST_ASSERT_EQUAL_STRING("V_REF", resultat_id(r));
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 3.3f, resultat_attendu(r));
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 3.35f, resultat_mesure(r));
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 5.0f, resultat_tolerance(r));
    TEST_ASSERT_EQUAL_INT(1, resultat_est_reussi(r));
    resultat_detruire(r);
}

/**
 * @brief L'identifiant est copié, pas simplement référencé.
 *
 * - Scénario : créer un résultat à partir d'un tampon local, puis écraser
 *              le tampon.
 * - Attendu  : resultat_id() retourne toujours "R1", et pas le même
 *              pointeur que le tampon.
 * - Détecte  : `r->id = id;` (copie superficielle) : le résultat
 *              changerait dès que l'appelant réutilise son tampon.
 */
void test_resultat_id_copie_profonde(void) {
    char tampon[16] = "R1";
    resultat_t *r = resultat_creer(tampon, 10.0f, 10.0f, 1.0f);
    TEST_ASSERT_NOT_NULL(r);
    strcpy(tampon, "XXXX");
    TEST_ASSERT_EQUAL_STRING("R1", resultat_id(r));
    TEST_ASSERT_TRUE(resultat_id(r) != tampon);
    resultat_detruire(r);
}

/* ---------- Verdict PASS ---------- */

/**
 * @brief Une mesure à l'intérieur de la tolérance donne PASS.
 *
 * - Scénario : attendu 100, tolérance 5 % (intervalle [95, 105]),
 *              mesures 102 et 97.
 * - Attendu  : PASS dans les deux cas (au-dessus et en dessous).
 */
void test_resultat_pass_dans_tolerance(void) {
    resultat_t *haut = resultat_creer("HAUT", 100.0f, 102.0f, 5.0f);
    resultat_t *bas = resultat_creer("BAS", 100.0f, 97.0f, 5.0f);
    TEST_ASSERT_EQUAL_INT(1, resultat_est_reussi(haut));
    TEST_ASSERT_EQUAL_INT(1, resultat_est_reussi(bas));
    resultat_detruire(haut);
    resultat_detruire(bas);
}

/**
 * @brief Les bornes de l'intervalle sont incluses.
 *
 * - Scénario : attendu 100, tolérance 5 %, mesures 105 et 95 exactement.
 * - Attendu  : PASS dans les deux cas (intervalle fermé [95, 105]).
 * - Détecte  : une comparaison stricte `<` au lieu de `<=`.
 */
void test_resultat_pass_bornes_incluses(void) {
    resultat_t *haut = resultat_creer("BORNE_H", 100.0f, 105.0f, 5.0f);
    resultat_t *bas = resultat_creer("BORNE_B", 100.0f, 95.0f, 5.0f);
    TEST_ASSERT_EQUAL_INT(1, resultat_est_reussi(haut));
    TEST_ASSERT_EQUAL_INT(1, resultat_est_reussi(bas));
    resultat_detruire(haut);
    resultat_detruire(bas);
}

/* ---------- Verdict FAIL ---------- */

/**
 * @brief Une mesure hors tolérance donne FAIL, des deux côtés.
 *
 * - Scénario : attendu 100, tolérance 5 %, mesures 106 et 94.
 * - Attendu  : FAIL dans les deux cas.
 * - Détecte  : une vérification d'un seul côté de l'intervalle (ex.
 *              `mesure - attendu <= ecart` sans valeur absolue : 94 passerait).
 */
void test_resultat_fail_hors_tolerance(void) {
    resultat_t *haut = resultat_creer("HAUT", 100.0f, 106.0f, 5.0f);
    resultat_t *bas = resultat_creer("BAS", 100.0f, 94.0f, 5.0f);
    TEST_ASSERT_EQUAL_INT(0, resultat_est_reussi(haut));
    TEST_ASSERT_EQUAL_INT(0, resultat_est_reussi(bas));
    resultat_detruire(haut);
    resultat_detruire(bas);
}

/**
 * @brief La tolérance est un pourcentage, pas une valeur absolue.
 *
 * - Scénario : attendu 1000, tolérance 5 % (écart permis 50), mesure 1006.
 * - Attendu  : PASS (écart 6 <= 50).
 * - Détecte  : `|mesure - attendu| <= tolerance` (oubli de multiplier par
 *              attendu / 100) : 6 > 5 donnerait FAIL.
 */
void test_resultat_tolerance_en_pourcentage(void) {
    resultat_t *r = resultat_creer("POURCENT", 1000.0f, 1006.0f, 5.0f);
    TEST_ASSERT_EQUAL_INT(1, resultat_est_reussi(r));
    resultat_detruire(r);
}

/* ---------- Cas limites ---------- */

/**
 * @brief Avec une tolérance de 0 %, seule la valeur exacte passe.
 *
 * - Scénario : attendu 50, tolérance 0, mesures 50 et 50.5.
 * - Attendu  : PASS pour 50, FAIL pour 50.5.
 * - Détecte  : une comparaison stricte `<` (0 < 0 est faux, donc même la
 *              valeur exacte échouerait).
 */
void test_resultat_tolerance_zero(void) {
    resultat_t *exact = resultat_creer("EXACT", 50.0f, 50.0f, 0.0f);
    resultat_t *proche = resultat_creer("PROCHE", 50.0f, 50.5f, 0.0f);
    TEST_ASSERT_EQUAL_INT(1, resultat_est_reussi(exact));
    TEST_ASSERT_EQUAL_INT(0, resultat_est_reussi(proche));
    resultat_detruire(exact);
    resultat_detruire(proche);
}

/**
 * @brief Avec une valeur attendue de 0, la tolérance absolue est 0.
 *
 * - Scénario : attendu 0, tolérance 10 %, mesures 0 et 0.001.
 * - Attendu  : PASS pour 0, FAIL pour 0.001 (0 * 10 / 100 = 0, peu importe
 *              le pourcentage).
 */
void test_resultat_attendu_zero(void) {
    resultat_t *zero = resultat_creer("ZERO", 0.0f, 0.0f, 10.0f);
    resultat_t *petit = resultat_creer("PETIT", 0.0f, 0.001f, 10.0f);
    TEST_ASSERT_EQUAL_INT(1, resultat_est_reussi(zero));
    TEST_ASSERT_EQUAL_INT(0, resultat_est_reussi(petit));
    resultat_detruire(zero);
    resultat_detruire(petit);
}

/**
 * @brief Une valeur attendue négative fonctionne comme une positive.
 *
 * - Scénario : attendu -12, tolérance 5 % (intervalle [-12.6, -11.4]),
 *              mesures -12.5 et -13.
 * - Attendu  : PASS pour -12.5, FAIL pour -13.
 * - Détecte  : un calcul `attendu * tolerance / 100` sans valeur absolue :
 *              l'écart permis devient négatif et tout échoue (-12.5 aussi).
 * - Note     : cas non exigé par l'énoncé (bonus).
 */
void test_resultat_attendu_negatif(void) {
    resultat_t *dedans = resultat_creer("NEG_IN", -12.0f, -12.5f, 5.0f);
    resultat_t *dehors = resultat_creer("NEG_OUT", -12.0f, -13.0f, 5.0f);
    TEST_ASSERT_EQUAL_INT(1, resultat_est_reussi(dedans));
    TEST_ASSERT_EQUAL_INT(0, resultat_est_reussi(dehors));
    resultat_detruire(dedans);
    resultat_detruire(dehors);
}

/* ---------- Création invalide / destruction ---------- */

/**
 * @brief Un identifiant NULL est refusé.
 *
 * - Scénario : resultat_creer(NULL, 1, 1, 5).
 * - Attendu  : NULL (et pas de plantage dans strlen).
 */
void test_resultat_creer_id_null(void) {
    TEST_ASSERT_NULL(resultat_creer(NULL, 1.0f, 1.0f, 5.0f));
}

/**
 * @brief Un identifiant vide est accepté.
 *
 * - Scénario : resultat_creer("", 1, 1, 5).
 * - Attendu  : pointeur non NULL, id "" (seul NULL est invalide selon
 *              l'énoncé).
 */
void test_resultat_creer_id_vide(void) {
    resultat_t *r = resultat_creer("", 1.0f, 1.0f, 5.0f);
    TEST_ASSERT_NOT_NULL(r);
    TEST_ASSERT_EQUAL_STRING("", resultat_id(r));
    resultat_detruire(r);
}

/**
 * @brief Détruire NULL n'a pas d'effet.
 *
 * - Scénario : resultat_detruire(NULL).
 * - Attendu  : pas de plantage (le test se rend à la fin).
 * - Détecte  : un `free(r->id)` sans vérifier r != NULL.
 */
void test_resultat_detruire_null(void) {
    resultat_detruire(NULL);
    TEST_PASS();
}

int main(void) {
    printf("** Demonstration de resultat_afficher **\n");
    resultat_t *demo[] = {
        resultat_creer("V_REF_3V3", 3.3f, 3.35f, 5.0f),
        resultat_creer("I_REPOS", 0.020f, 0.025f, 10.0f),
        resultat_creer("V_NEG_12", -12.0f, -12.4f, 5.0f),
    };
    for (int i = 0; i < 3; i++) {
        resultat_afficher(demo[i]);
        resultat_detruire(demo[i]);
    }
    printf("** Fin de la demonstration **\n\n");

    UNITY_BEGIN();

    RUN_TEST(test_resultat_creer_valide);
    RUN_TEST(test_resultat_id_copie_profonde);

    RUN_TEST(test_resultat_pass_dans_tolerance);
    RUN_TEST(test_resultat_pass_bornes_incluses);

    RUN_TEST(test_resultat_fail_hors_tolerance);
    RUN_TEST(test_resultat_tolerance_en_pourcentage);

    RUN_TEST(test_resultat_tolerance_zero);
    RUN_TEST(test_resultat_attendu_zero);
    RUN_TEST(test_resultat_attendu_negatif);

    RUN_TEST(test_resultat_creer_id_null);
    RUN_TEST(test_resultat_creer_id_vide);
    RUN_TEST(test_resultat_detruire_null);

    return UNITY_END();
}
