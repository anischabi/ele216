#include <stdio.h>

#include "pile.h"
#include "unity.h"

#define CAPACITE 1000.0f
#define TENSION  3.7f

static pile_t *p = NULL;

// Fixture : chaque test reçoit une pile neuve (1000 mAh, 3.7 V, charge = 0).
void setUp(void) {
    p = pile_creer(CAPACITE, TENSION);
    TEST_ASSERT_NOT_NULL(p);
}

void tearDown(void) {
    pile_detruire(p);
    p = NULL;
}

/* ---------- Création valide / invalide ---------- */

void test_pile_creer_valide(void) {
    pile_t *q = pile_creer(2500.0f, 12.0f);
    TEST_ASSERT_NOT_NULL(q);
    TEST_ASSERT_EQUAL_FLOAT(2500.0f, pile_capacite_max(q));
    TEST_ASSERT_EQUAL_FLOAT(12.0f, pile_tension(q));
    TEST_ASSERT_EQUAL_FLOAT(0.0f, pile_charge_courante(q));
    pile_detruire(q);
}

void test_pile_creer_capacite_nulle(void) {
    TEST_ASSERT_NULL(pile_creer(0.0f, 3.7f));
}

void test_pile_creer_capacite_negative(void) {
    TEST_ASSERT_NULL(pile_creer(-500.0f, 3.7f));
}

void test_pile_creer_tension_nulle(void) {
    TEST_ASSERT_NULL(pile_creer(1000.0f, 0.0f));
}

void test_pile_creer_tension_negative(void) {
    TEST_ASSERT_NULL(pile_creer(1000.0f, -1.5f));
}

/* ---------- Charge partielle puis lecture ---------- */

void test_pile_charger_partiel(void) {
    pile_charger(p, 250.0f);
    TEST_ASSERT_EQUAL_FLOAT(250.0f, pile_charge_courante(p));
}

void test_pile_charger_cumul(void) {
    pile_charger(p, 250.0f);
    pile_charger(p, 300.0f);
    TEST_ASSERT_EQUAL_FLOAT(550.0f, pile_charge_courante(p));
}

/* ---------- Saturation à la capacité maximale ---------- */

void test_pile_charger_jusqu_a_la_capacite_exacte(void) {
    pile_charger(p, 1000.0f);
    TEST_ASSERT_EQUAL_FLOAT(1000.0f, pile_charge_courante(p));
}

void test_pile_charger_sature(void) {
    pile_charger(p, 1500.0f);
    TEST_ASSERT_EQUAL_FLOAT(1000.0f, pile_charge_courante(p));
}

void test_pile_charger_sature_apres_cumul(void) {
    pile_charger(p, 700.0f);
    pile_charger(p, 700.0f);
    TEST_ASSERT_EQUAL_FLOAT(1000.0f, pile_charge_courante(p));
    pile_charger(p, 1.0f);
    TEST_ASSERT_EQUAL_FLOAT(1000.0f, pile_charge_courante(p));
}

/* ---------- Décharge sans descendre sous 0 ---------- */

void test_pile_decharger_partiel(void) {
    pile_charger(p, 600.0f);
    pile_decharger(p, 200.0f);
    TEST_ASSERT_EQUAL_FLOAT(400.0f, pile_charge_courante(p));
}

void test_pile_decharger_complet_exact(void) {
    pile_charger(p, 400.0f);
    pile_decharger(p, 400.0f);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, pile_charge_courante(p));
}

void test_pile_decharger_plus_que_la_charge(void) {
    pile_charger(p, 300.0f);
    pile_decharger(p, 1000.0f);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, pile_charge_courante(p));
}

void test_pile_decharger_pile_vide(void) {
    pile_decharger(p, 50.0f);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, pile_charge_courante(p));
}

/* ---------- Pourcentage à plusieurs niveaux ---------- */

void test_pile_pourcentage_0(void) {
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, pile_pourcentage(p));
}

void test_pile_pourcentage_25(void) {
    pile_charger(p, 250.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 25.0f, pile_pourcentage(p));
}

void test_pile_pourcentage_50(void) {
    pile_charger(p, 500.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 50.0f, pile_pourcentage(p));
}

void test_pile_pourcentage_100(void) {
    pile_charger(p, 1000.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 100.0f, pile_pourcentage(p));
}

void test_pile_pourcentage_reste_a_100_apres_saturation(void) {
    pile_charger(p, 5000.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 100.0f, pile_pourcentage(p));
}

/* ---------- Robustesse et immuabilité ---------- */

void test_pile_charger_valeur_negative_ignoree(void) {
    pile_charger(p, 300.0f);
    pile_charger(p, -100.0f);
    TEST_ASSERT_EQUAL_FLOAT(300.0f, pile_charge_courante(p));
}

void test_pile_decharger_valeur_negative_ignoree(void) {
    pile_charger(p, 300.0f);
    pile_decharger(p, -100.0f);
    TEST_ASSERT_EQUAL_FLOAT(300.0f, pile_charge_courante(p));
}

void test_pile_capacite_et_tension_immuables(void) {
    pile_charger(p, 5000.0f);
    pile_decharger(p, 5000.0f);
    TEST_ASSERT_EQUAL_FLOAT(CAPACITE, pile_capacite_max(p));
    TEST_ASSERT_EQUAL_FLOAT(TENSION, pile_tension(p));
}

void test_pile_detruire_null(void) {
    pile_detruire(NULL);    // ne doit pas planter
    TEST_PASS();
}

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
