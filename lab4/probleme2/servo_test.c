/**
 * @file servo_test.c
 * @brief Tests unitaires (Unity) du module servo.
 *
 * Chaque test reçoit un servo neuf créé par setUp() (butées [-90, 90] deg,
 * vitesse 60 deg/s, angle initial -90) et détruit par tearDown(), donc les
 * tests sont indépendants. Les tests sont regroupés par thème, dans l'ordre
 * de l'énoncé.
 *
 * Les valeurs choisies (angles entiers, vitesse 60) donnent des résultats
 * exactement représentables en float (2.0, 1.5, 3.0...), donc
 * TEST_ASSERT_EQUAL_FLOAT suffit.
 */

#include <math.h>
#include <stdio.h>

#include "servo.h"
#include "unity.h"

/** Butée minimale (deg) du servo créé par setUp(). */
#define ANGLE_MIN -90.0f
/** Butée maximale (deg) du servo créé par setUp(). */
#define ANGLE_MAX  90.0f
/** Vitesse (deg/s) du servo créé par setUp(). */
#define VITESSE    60.0f

/** Servo utilisé par les tests, recréé avant chaque test par setUp(). */
static servo_t *s = NULL;

/** Fixture : crée un servo neuf ([-90, 90] deg, 60 deg/s) avant chaque test. */
void setUp(void) {
    s = servo_creer(ANGLE_MIN, ANGLE_MAX, VITESSE);
    TEST_ASSERT_NOT_NULL(s);
}

/** Fixture : détruit le servo après chaque test (aucune fuite mémoire). */
void tearDown(void) {
    servo_detruire(s);
    s = NULL;
}

/* ---------- Création valide / invalide ---------- */

/**
 * @brief Un servo créé avec des paramètres valides est correctement initialisé.
 *
 * - Scénario : servo_creer(0, 180, 120).
 * - Attendu  : pointeur non NULL, butées 0 et 180, vitesse 120, angle
 *              courant = angle minimum (0).
 * - Note     : ces valeurs diffèrent de celles de setUp(), pour prouver que
 *              ce sont bien les paramètres reçus qui sont stockés.
 */
void test_servo_creer_valide(void) {
    servo_t *s1 = servo_creer(0.0f, 180.0f, 120.0f);
    TEST_ASSERT_NOT_NULL(s1);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, servo_angle_min(s1));
    TEST_ASSERT_EQUAL_FLOAT(180.0f, servo_angle_max(s1));
    TEST_ASSERT_EQUAL_FLOAT(120.0f, servo_vitesse(s1));
    TEST_ASSERT_EQUAL_FLOAT(0.0f, servo_angle(s1));
    servo_detruire(s1);
}

/**
 * @brief L'angle initial est l'angle minimum, même quand il est négatif.
 *
 * - Scénario : servo de setUp() ([-90, 90]).
 * - Attendu  : angle courant -90.
 * - Détecte  : un angle initialisé à 0 (correct par hasard quand min = 0,
 *              d'où l'intérêt d'une butée minimale non nulle).
 */
void test_servo_creer_angle_initial_min(void) {
    TEST_ASSERT_EQUAL_FLOAT(ANGLE_MIN, servo_angle(s));
}

/**
 * @brief Des butées égales sont refusées.
 *
 * - Scénario : servo_creer(45, 45, 60).
 * - Attendu  : NULL.
 * - Détecte  : une validation écrite `min > max` au lieu de `min >= max`.
 */
void test_servo_creer_min_egal_max(void) {
    TEST_ASSERT_NULL(servo_creer(45.0f, 45.0f, 60.0f));
}

/**
 * @brief Des butées inversées sont refusées.
 *
 * - Scénario : servo_creer(90, -90, 60).
 * - Attendu  : NULL.
 */
void test_servo_creer_min_superieur_max(void) {
    TEST_ASSERT_NULL(servo_creer(90.0f, -90.0f, 60.0f));
}

/**
 * @brief Une vitesse nulle est refusée.
 *
 * - Scénario : servo_creer(-90, 90, 0).
 * - Attendu  : NULL.
 * - Détecte  : une validation écrite `< 0` au lieu de `<= 0` (une vitesse de
 *              0 causerait une division par zéro dans
 *              servo_temps_pour_atteindre).
 */
void test_servo_creer_vitesse_nulle(void) {
    TEST_ASSERT_NULL(servo_creer(-90.0f, 90.0f, 0.0f));
}

/**
 * @brief Une vitesse négative est refusée.
 *
 * - Scénario : servo_creer(-90, 90, -30).
 * - Attendu  : NULL.
 */
void test_servo_creer_vitesse_negative(void) {
    TEST_ASSERT_NULL(servo_creer(-90.0f, 90.0f, -30.0f));
}

/**
 * @brief Des paramètres NaN sont refusés.
 *
 * - Scénario : NaN tour à tour comme angle_min, angle_max et vitesse.
 * - Attendu  : NULL dans les trois cas.
 * - Détecte  : une validation écrite `min >= max || vitesse <= 0`, qui laisse
 *              passer NaN puisque toute comparaison avec NaN est fausse.
 * - Note     : cas hors énoncé, à ne pas exiger des étudiant.e.s.
 */
void test_servo_creer_nan(void) {
    TEST_ASSERT_NULL(servo_creer(NAN, 90.0f, 60.0f));
    TEST_ASSERT_NULL(servo_creer(-90.0f, NAN, 60.0f));
    TEST_ASSERT_NULL(servo_creer(-90.0f, 90.0f, NAN));
}

/**
 * @brief servo_detruire(NULL) n'a pas d'effet.
 *
 * - Scénario : servo_detruire(NULL).
 * - Attendu  : aucun plantage (le test se termine).
 */
void test_servo_detruire_null(void) {
    servo_detruire(NULL);
}

/* ---------- Déplacement dans la plage normale ---------- */

/**
 * @brief Une consigne dans la plage est atteinte telle quelle.
 *
 * - Scénario : servo_aller_a(30).
 * - Attendu  : angle 30, butées et vitesse inchangées.
 */
void test_servo_aller_a_dans_plage(void) {
    servo_aller_a(s, 30.0f);
    TEST_ASSERT_EQUAL_FLOAT(30.0f, servo_angle(s));
    TEST_ASSERT_EQUAL_FLOAT(ANGLE_MIN, servo_angle_min(s));
    TEST_ASSERT_EQUAL_FLOAT(ANGLE_MAX, servo_angle_max(s));
    TEST_ASSERT_EQUAL_FLOAT(VITESSE, servo_vitesse(s));
}

/**
 * @brief Plusieurs déplacements successifs, dans les deux sens.
 *
 * - Scénario : aller_a(45), puis aller_a(-60), puis aller_a(0).
 * - Attendu  : l'angle suit chaque consigne (45, -60, 0).
 */
void test_servo_aller_a_plusieurs_deplacements(void) {
    servo_aller_a(s, 45.0f);
    TEST_ASSERT_EQUAL_FLOAT(45.0f, servo_angle(s));
    servo_aller_a(s, -60.0f);
    TEST_ASSERT_EQUAL_FLOAT(-60.0f, servo_angle(s));
    servo_aller_a(s, 0.0f);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, servo_angle(s));
}

/**
 * @brief Les butées elles-mêmes sont des consignes valides.
 *
 * - Scénario : aller_a(90), puis aller_a(-90).
 * - Attendu  : angle 90, puis -90.
 */
void test_servo_aller_a_butees_exactes(void) {
    servo_aller_a(s, ANGLE_MAX);
    TEST_ASSERT_EQUAL_FLOAT(ANGLE_MAX, servo_angle(s));
    servo_aller_a(s, ANGLE_MIN);
    TEST_ASSERT_EQUAL_FLOAT(ANGLE_MIN, servo_angle(s));
}

/* ---------- Saturation aux butées ---------- */

/**
 * @brief Une consigne au-dessus du maximum est saturée à angle_max.
 *
 * - Scénario : servo_aller_a(200).
 * - Attendu  : angle 90, butées inchangées.
 * - Détecte  : une absence de saturation, ou une saturation qui modifie la
 *              butée au lieu de l'angle.
 */
void test_servo_aller_a_sature_max(void) {
    servo_aller_a(s, 200.0f);
    TEST_ASSERT_EQUAL_FLOAT(ANGLE_MAX, servo_angle(s));
    TEST_ASSERT_EQUAL_FLOAT(ANGLE_MAX, servo_angle_max(s));
}

/**
 * @brief Une consigne sous le minimum est saturée à angle_min.
 *
 * - Scénario : aller_a(30) d'abord (pour quitter la position initiale -90),
 *              puis aller_a(-200).
 * - Attendu  : angle -90, butées inchangées.
 * - Note     : sans le déplacement préalable, l'angle vaudrait déjà -90 et un
 *              aller_a qui ne fait rien passerait le test.
 */
void test_servo_aller_a_sature_min(void) {
    servo_aller_a(s, 30.0f);
    servo_aller_a(s, -200.0f);
    TEST_ASSERT_EQUAL_FLOAT(ANGLE_MIN, servo_angle(s));
    TEST_ASSERT_EQUAL_FLOAT(ANGLE_MIN, servo_angle_min(s));
}

/**
 * @brief Une consigne NaN est ignorée.
 *
 * - Scénario : aller_a(30), puis aller_a(NaN).
 * - Attendu  : angle toujours 30.
 * - Note     : cas hors énoncé, à ne pas exiger des étudiant.e.s.
 */
void test_servo_aller_a_nan_ignore(void) {
    servo_aller_a(s, 30.0f);
    servo_aller_a(s, NAN);
    TEST_ASSERT_EQUAL_FLOAT(30.0f, servo_angle(s));
}

/* ---------- Temps de déplacement ---------- */

/**
 * @brief Temps pour un angle connu.
 *
 * - Scénario : angle courant -90, cible 30, vitesse 60 deg/s.
 * - Attendu  : |30 - (-90)| / 60 = 120 / 60 = 2.0 s.
 * - Détecte  : une multiplication au lieu d'une division, ou un calcul
 *              depuis 0 plutôt que depuis l'angle courant (30 / 60 = 0.5).
 */
void test_servo_temps_angle_connu(void) {
    TEST_ASSERT_EQUAL_FLOAT(2.0f, servo_temps_pour_atteindre(s, 30.0f));
}

/**
 * @brief Le calcul du temps ne modifie pas l'état du servo.
 *
 * - Scénario : servo_temps_pour_atteindre(s, 30).
 * - Attendu  : l'angle courant reste -90.
 * - Détecte  : une fonction qui déplace le servo en calculant le temps.
 */
void test_servo_temps_ne_modifie_pas_etat(void) {
    servo_temps_pour_atteindre(s, 30.0f);
    TEST_ASSERT_EQUAL_FLOAT(ANGLE_MIN, servo_angle(s));
}

/**
 * @brief Temps pour un déplacement vers un angle plus petit (sens négatif).
 *
 * - Scénario : aller_a(30), puis temps vers -60.
 * - Attendu  : |-60 - 30| / 60 = 1.5 s (positif).
 * - Détecte  : un oubli de la valeur absolue (-1.5 serait confondu avec
 *              le code d'erreur « hors butées »).
 */
void test_servo_temps_sens_negatif(void) {
    servo_aller_a(s, 30.0f);
    TEST_ASSERT_EQUAL_FLOAT(1.5f, servo_temps_pour_atteindre(s, -60.0f));
}

/**
 * @brief Le temps pour rester sur place est nul.
 *
 * - Scénario : aller_a(30), puis temps vers 30.
 * - Attendu  : 0.0 s.
 */
void test_servo_temps_angle_courant(void) {
    servo_aller_a(s, 30.0f);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, servo_temps_pour_atteindre(s, 30.0f));
}

/**
 * @brief Le temps vers une butée exacte est valide (course complète).
 *
 * - Scénario : angle courant -90, cible 90.
 * - Attendu  : 180 / 60 = 3.0 s (et non une valeur négative).
 * - Détecte  : une vérification des butées écrite avec `>=` / `<=` au lieu
 *              de `>` / `<` (les butées seraient rejetées).
 */
void test_servo_temps_butee_exacte(void) {
    TEST_ASSERT_EQUAL_FLOAT(3.0f, servo_temps_pour_atteindre(s, ANGLE_MAX));
}

/**
 * @brief Un angle au-dessus du maximum donne une valeur négative.
 *
 * - Scénario : temps vers 120.
 * - Attendu  : valeur < 0.
 * - Détecte  : un temps calculé sans vérifier les butées (210 / 60 = 3.5),
 *              ou calculé vers l'angle saturé (3.0).
 */
void test_servo_temps_hors_butee_max(void) {
    TEST_ASSERT_TRUE(servo_temps_pour_atteindre(s, 120.0f) < 0.0f);
}

/**
 * @brief Un angle sous le minimum donne une valeur négative.
 *
 * - Scénario : aller_a(0), puis temps vers -100.
 * - Attendu  : valeur < 0.
 */
void test_servo_temps_hors_butee_min(void) {
    servo_aller_a(s, 0.0f);
    TEST_ASSERT_TRUE(servo_temps_pour_atteindre(s, -100.0f) < 0.0f);
}

/**
 * @brief Point d'entrée : démonstration de servo_afficher, puis exécution des tests.
 *
 * La démonstration déplace un servo [0, 180] vers 45, 180, 250 et -30 degrés
 * en affichant le temps de déplacement et l'état après chaque consigne ;
 * servo_afficher ne se prête pas à une vérification automatique, on la
 * regarde à l'œil.
 */
int main(void) {
    printf("** Demonstration de servo_afficher **\n");
    servo_t *demo = servo_creer(0.0f, 180.0f, 90.0f);
    servo_afficher(demo);
    float consignes[] = {45.0f, 180.0f, 250.0f, -30.0f};
    for (int i = 0; i < 4; i++) {
        printf("temps pour aller a %.1f : %.2f s -> ", consignes[i],
               servo_temps_pour_atteindre(demo, consignes[i]));
        servo_aller_a(demo, consignes[i]);
        servo_afficher(demo);
    }
    servo_detruire(demo);
    printf("** Fin de la demonstration **\n\n");

    UNITY_BEGIN();

    RUN_TEST(test_servo_creer_valide);
    RUN_TEST(test_servo_creer_angle_initial_min);
    RUN_TEST(test_servo_creer_min_egal_max);
    RUN_TEST(test_servo_creer_min_superieur_max);
    RUN_TEST(test_servo_creer_vitesse_nulle);
    RUN_TEST(test_servo_creer_vitesse_negative);
    RUN_TEST(test_servo_creer_nan);
    RUN_TEST(test_servo_detruire_null);

    RUN_TEST(test_servo_aller_a_dans_plage);
    RUN_TEST(test_servo_aller_a_plusieurs_deplacements);
    RUN_TEST(test_servo_aller_a_butees_exactes);

    RUN_TEST(test_servo_aller_a_sature_max);
    RUN_TEST(test_servo_aller_a_sature_min);
    RUN_TEST(test_servo_aller_a_nan_ignore);

    RUN_TEST(test_servo_temps_angle_connu);
    RUN_TEST(test_servo_temps_ne_modifie_pas_etat);
    RUN_TEST(test_servo_temps_sens_negatif);
    RUN_TEST(test_servo_temps_angle_courant);
    RUN_TEST(test_servo_temps_butee_exacte);
    RUN_TEST(test_servo_temps_hors_butee_max);
    RUN_TEST(test_servo_temps_hors_butee_min);

    return UNITY_END();
}
