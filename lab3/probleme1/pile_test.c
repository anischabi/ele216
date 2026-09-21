/**
 * @file pile_test.c
 * @brief Tests unitaires (Unity) du module pile.
 *
 * Chaque test reçoit une pile neuve créée par setUp() (1000 mAh, 3.7 V,
 * charge = 0) et détruite par tearDown(), donc les tests sont indépendants.
 * Les tests sont regroupés par thème, dans l'ordre de l'énoncé.
 *
 * Les valeurs de charge sont comparées avec TEST_ASSERT_EQUAL_FLOAT quand
 * elles sont exactement représentables en float (250, 500, 1000...), et avec
 * TEST_ASSERT_FLOAT_WITHIN (tolérance) pour les pourcentages calculés par
 * division.
 */

#include <stdio.h>

#include "pile.h"
#include "unity.h"

/** Capacité maximale (mAh) de la pile créée par setUp(). */
#define CAPACITE 1000.0f
/** Tension nominale (V) de la pile créée par setUp(). */
#define TENSION  3.7f

/** Pile utilisée par les tests, recréée avant chaque test par setUp(). */
static pile_t *p = NULL;

/** Fixture : crée une pile neuve (1000 mAh, 3.7 V, charge = 0) avant chaque test. */
void setUp(void) {
    p = pile_creer(CAPACITE, TENSION);
    TEST_ASSERT_NOT_NULL(p);
}

/** Fixture : détruit la pile après chaque test (aucune fuite mémoire). */
void tearDown(void) {
    pile_detruire(p);
    p = NULL;
}

/* ---------- Création valide / invalide ---------- */

/**
 * @brief Une pile créée avec des paramètres valides est correctement initialisée.
 *
 * - Scénario : pile_creer(2500, 12).
 * - Attendu  : pointeur non NULL, capacité 2500, tension 12, charge 0.
 * - Note     : ces valeurs diffèrent de celles de setUp(), pour prouver que ce
 *              sont bien les paramètres reçus qui sont stockés.
 */
void test_pile_creer_valide(void) {
    pile_t *q = pile_creer(2500.0f, 12.0f);
    TEST_ASSERT_NOT_NULL(q);
    TEST_ASSERT_EQUAL_FLOAT(2500.0f, pile_capacite_max(q));
    TEST_ASSERT_EQUAL_FLOAT(12.0f, pile_tension(q));
    TEST_ASSERT_EQUAL_FLOAT(0.0f, pile_charge_courante(q));
    pile_detruire(q);
}

/**
 * @brief Une capacité nulle est refusée.
 *
 * - Scénario : pile_creer(0, 3.7).
 * - Attendu  : NULL.
 * - Détecte  : une validation écrite `< 0` au lieu de `<= 0` (une capacité de 0
 *              causerait ensuite une division par zéro dans pile_pourcentage).
 */
void test_pile_creer_capacite_nulle(void) {
    TEST_ASSERT_NULL(pile_creer(0.0f, 3.7f));
}

/**
 * @brief Une capacité négative est refusée.
 *
 * - Scénario : pile_creer(-500, 3.7).
 * - Attendu  : NULL.
 */
void test_pile_creer_capacite_negative(void) {
    TEST_ASSERT_NULL(pile_creer(-500.0f, 3.7f));
}

/**
 * @brief Une tension nulle est refusée.
 *
 * - Scénario : pile_creer(1000, 0).
 * - Attendu  : NULL.
 */
void test_pile_creer_tension_nulle(void) {
    TEST_ASSERT_NULL(pile_creer(1000.0f, 0.0f));
}

/**
 * @brief Une tension négative est refusée.
 *
 * - Scénario : pile_creer(1000, -1.5).
 * - Attendu  : NULL.
 */
void test_pile_creer_tension_negative(void) {
    TEST_ASSERT_NULL(pile_creer(1000.0f, -1.5f));
}

/* ---------- Charge partielle puis lecture ---------- */

/**
 * @brief Une charge partielle est bien lue par pile_charge_courante.
 *
 * - Scénario : pile vide de 1000 mAh, pile_charger(250).
 * - Attendu  : charge courante = 250.
 */
void test_pile_charger_partiel(void) {
    pile_charger(p, 250.0f);
    TEST_ASSERT_EQUAL_FLOAT(250.0f, pile_charge_courante(p));
}

/**
 * @brief Plusieurs charges successives s'additionnent.
 *
 * - Scénario : pile_charger(250) puis pile_charger(300).
 * - Attendu  : charge courante = 550.
 */
void test_pile_charger_cumul(void) {
    pile_charger(p, 250.0f);
    pile_charger(p, 300.0f);
    TEST_ASSERT_EQUAL_FLOAT(550.0f, pile_charge_courante(p));
}

/* ---------- Saturation à la capacité maximale ---------- */

/**
 * @brief Cas limite : charger exactement la capacité maximale.
 *
 * - Scénario : pile_charger(1000) sur une pile de 1000 mAh.
 * - Attendu  : charge courante = 1000 (la saturation ne fausse pas la valeur
 *              quand on atteint exactement le plafond).
 */
void test_pile_charger_jusqu_a_la_capacite_exacte(void) {
    pile_charger(p, 1000.0f);
    TEST_ASSERT_EQUAL_FLOAT(1000.0f, pile_charge_courante(p));
}

/**
 * @brief Charger plus que la capacité sature à la capacité maximale.
 *
 * - Scénario : pile_charger(1500) sur une pile de 1000 mAh.
 * - Attendu  : charge courante = 1000, pas 1500.
 * - Détecte  : l'oubli de la saturation (`charge += mAh` seul).
 */
void test_pile_charger_sature(void) {
    pile_charger(p, 1500.0f);
    TEST_ASSERT_EQUAL_FLOAT(1000.0f, pile_charge_courante(p));
}

/**
 * @brief La saturation s'applique aussi quand le dépassement arrive en plusieurs appels.
 *
 * - Scénario : pile_charger(700) deux fois (1400 au total), puis pile_charger(1).
 * - Attendu  : charge courante = 1000 après le 2e appel, et toujours 1000
 *              après avoir chargé une pile déjà pleine.
 */
void test_pile_charger_sature_apres_cumul(void) {
    pile_charger(p, 700.0f);
    pile_charger(p, 700.0f);
    TEST_ASSERT_EQUAL_FLOAT(1000.0f, pile_charge_courante(p));
    pile_charger(p, 1.0f);
    TEST_ASSERT_EQUAL_FLOAT(1000.0f, pile_charge_courante(p));
}

/* ---------- Décharge sans descendre sous 0 ---------- */

/**
 * @brief Une décharge partielle retire bien la quantité demandée.
 *
 * - Scénario : pile_charger(600) puis pile_decharger(200).
 * - Attendu  : charge courante = 400.
 */
void test_pile_decharger_partiel(void) {
    pile_charger(p, 600.0f);
    pile_decharger(p, 200.0f);
    TEST_ASSERT_EQUAL_FLOAT(400.0f, pile_charge_courante(p));
}

/**
 * @brief Cas limite : décharger exactement toute la charge.
 *
 * - Scénario : pile_charger(400) puis pile_decharger(400).
 * - Attendu  : charge courante = 0.
 */
void test_pile_decharger_complet_exact(void) {
    pile_charger(p, 400.0f);
    pile_decharger(p, 400.0f);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, pile_charge_courante(p));
}

/**
 * @brief Décharger plus que la charge présente s'arrête à 0.
 *
 * - Scénario : pile_charger(300) puis pile_decharger(1000).
 * - Attendu  : charge courante = 0, pas -700.
 * - Détecte  : l'oubli du plancher à 0.
 */
void test_pile_decharger_plus_que_la_charge(void) {
    pile_charger(p, 300.0f);
    pile_decharger(p, 1000.0f);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, pile_charge_courante(p));
}

/**
 * @brief Décharger une pile déjà vide la laisse à 0.
 *
 * - Scénario : pile_decharger(50) sur une pile neuve.
 * - Attendu  : charge courante = 0.
 */
void test_pile_decharger_pile_vide(void) {
    pile_decharger(p, 50.0f);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, pile_charge_courante(p));
}

/* ---------- Pourcentage à plusieurs niveaux ---------- */

/**
 * @brief Le pourcentage d'une pile vide vaut 0.
 *
 * - Scénario : pile neuve, aucune charge.
 * - Attendu  : pile_pourcentage = 0 (à la tolérance près).
 */
void test_pile_pourcentage_0(void) {
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, pile_pourcentage(p));
}

/**
 * @brief Le pourcentage d'une pile au quart vaut 25.
 *
 * - Scénario : pile_charger(250) sur 1000 mAh.
 * - Attendu  : pile_pourcentage = 25.
 */
void test_pile_pourcentage_25(void) {
    pile_charger(p, 250.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 25.0f, pile_pourcentage(p));
}

/**
 * @brief Le pourcentage d'une pile à moitié chargée vaut 50.
 *
 * - Scénario : pile_charger(500) sur 1000 mAh.
 * - Attendu  : pile_pourcentage = 50.
 */
void test_pile_pourcentage_50(void) {
    pile_charger(p, 500.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 50.0f, pile_pourcentage(p));
}

/**
 * @brief Le pourcentage d'une pile pleine vaut 100.
 *
 * - Scénario : pile_charger(1000) sur 1000 mAh.
 * - Attendu  : pile_pourcentage = 100.
 * - Détecte  : un oubli du `* 100` (on obtiendrait 1 au lieu de 100).
 */
void test_pile_pourcentage_100(void) {
    pile_charger(p, 1000.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 100.0f, pile_pourcentage(p));
}

/**
 * @brief Le pourcentage ne dépasse jamais 100, même après une charge excessive.
 *
 * - Scénario : pile_charger(5000) sur 1000 mAh.
 * - Attendu  : pile_pourcentage = 100, pas 500.
 * - Note     : ce test vérifie la saturation à travers pile_pourcentage.
 */
void test_pile_pourcentage_reste_a_100_apres_saturation(void) {
    pile_charger(p, 5000.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 100.0f, pile_pourcentage(p));
}

/* ---------- Robustesse et immuabilité ---------- */

/**
 * @brief Une valeur négative passée à pile_charger est ignorée.
 *
 * - Scénario : pile_charger(300) puis pile_charger(-100).
 * - Attendu  : charge courante = 300 (et non 200).
 * - Note     : l'énoncé ne dit rien sur ce cas ; ce comportement est un choix
 *              de conception documenté dans pile.h.
 */
void test_pile_charger_valeur_negative_ignoree(void) {
    pile_charger(p, 300.0f);
    pile_charger(p, -100.0f);
    TEST_ASSERT_EQUAL_FLOAT(300.0f, pile_charge_courante(p));
}

/**
 * @brief Une valeur négative passée à pile_decharger est ignorée.
 *
 * - Scénario : pile_charger(300) puis pile_decharger(-100).
 * - Attendu  : charge courante = 300 (et non 400).
 */
void test_pile_decharger_valeur_negative_ignoree(void) {
    pile_charger(p, 300.0f);
    pile_decharger(p, -100.0f);
    TEST_ASSERT_EQUAL_FLOAT(300.0f, pile_charge_courante(p));
}

/**
 * @brief La capacité maximale et la tension ne changent jamais.
 *
 * - Scénario : charge excessive (5000) puis décharge excessive (5000).
 * - Attendu  : capacité toujours 1000 mAh et tension toujours 3.7 V.
 */
void test_pile_capacite_et_tension_immuables(void) {
    pile_charger(p, 5000.0f);
    pile_decharger(p, 5000.0f);
    TEST_ASSERT_EQUAL_FLOAT(CAPACITE, pile_capacite_max(p));
    TEST_ASSERT_EQUAL_FLOAT(TENSION, pile_tension(p));
}

/**
 * @brief Détruire un pointeur NULL est permis et sans effet.
 *
 * - Scénario : pile_detruire(NULL).
 * - Attendu  : aucun plantage (le test réussit simplement s'il arrive au bout).
 */
void test_pile_detruire_null(void) {
    pile_detruire(NULL);    // ne doit pas planter
    TEST_PASS();
}

/**
 * @brief Point d'entrée : démonstration de pile_afficher, puis exécution des tests.
 *
 * La démonstration affiche une pile de 2000 mAh chargée à 500 mAh ;
 * pile_afficher ne se prête pas à une vérification automatique, on la
 * regarde à l'œil.
 */
int main(void) {
    printf("** Demonstration de pile_afficher **\n");
    pile_t *demo = pile_creer(2000.0f, 3.7f);
    pile_charger(demo, 500.0f);
    pile_afficher(demo);
    pile_detruire(demo);
    printf("** Fin de la demonstration **\n\n");

    UNITY_BEGIN();

    RUN_TEST(test_pile_creer_valide);
    RUN_TEST(test_pile_creer_capacite_nulle);
    RUN_TEST(test_pile_creer_capacite_negative);
    RUN_TEST(test_pile_creer_tension_nulle);
    RUN_TEST(test_pile_creer_tension_negative);

    RUN_TEST(test_pile_charger_partiel);
    RUN_TEST(test_pile_charger_cumul);

    RUN_TEST(test_pile_charger_jusqu_a_la_capacite_exacte);
    RUN_TEST(test_pile_charger_sature);
    RUN_TEST(test_pile_charger_sature_apres_cumul);

    RUN_TEST(test_pile_decharger_partiel);
    RUN_TEST(test_pile_decharger_complet_exact);
    RUN_TEST(test_pile_decharger_plus_que_la_charge);
    RUN_TEST(test_pile_decharger_pile_vide);

    RUN_TEST(test_pile_pourcentage_0);
    RUN_TEST(test_pile_pourcentage_25);
    RUN_TEST(test_pile_pourcentage_50);
    RUN_TEST(test_pile_pourcentage_100);
    RUN_TEST(test_pile_pourcentage_reste_a_100_apres_saturation);

    RUN_TEST(test_pile_charger_valeur_negative_ignoree);
    RUN_TEST(test_pile_decharger_valeur_negative_ignoree);
    RUN_TEST(test_pile_capacite_et_tension_immuables);
    RUN_TEST(test_pile_detruire_null);

    return UNITY_END();
}
