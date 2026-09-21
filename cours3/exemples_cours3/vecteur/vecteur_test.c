#include <stdio.h>
#include <stdbool.h>

#include "vecteur.h"
#include "unity.h"

static vecteur_t *v = NULL;

void setUp(void) {
    v = vecteur_creer(2);
}

void tearDown(void) {
    vecteur_liberer(v);
}

void test_vecteur_creer_valide(void) {
    vecteur_t *v1 = vecteur_creer(16);
    TEST_ASSERT_EQUAL(16, vecteur_capacite(v1));
    TEST_ASSERT_EQUAL(0, vecteur_taille(v1));
    TEST_ASSERT_TRUE(vecteur_est_vide(v1));
}

void test_vecteur_creer_taille_zero(void) {
    vecteur_t *v1 = vecteur_creer(0);
    TEST_ASSERT_NULL(v1);
}

void test_vecteur_creer_taille_negative(void) {
    vecteur_t *v1 = vecteur_creer(-7);
    TEST_ASSERT_NULL(v1);
}

void test_vecteur_ajouter_avec_espace(void) {
    vecteur_ajouter(v, 22);
    TEST_ASSERT_EQUAL(2, vecteur_capacite(v));
    TEST_ASSERT_EQUAL(1, vecteur_taille(v));
    TEST_ASSERT_FALSE(vecteur_est_vide(v));
}

void test_vecteur_ajouter_plein(void) {
    vecteur_ajouter(v, 32);
    vecteur_ajouter(v, 42);
    vecteur_ajouter(v, 52);
    TEST_ASSERT_EQUAL(4, vecteur_capacite(v));
    TEST_ASSERT_EQUAL(3, vecteur_taille(v));
    TEST_ASSERT_FALSE(vecteur_est_vide(v));
}

int main(void){

    printf("** Tests Manuel **\n");
    vecteur_t *tab_test = vecteur_creer(2);
    vecteur_ajouter(tab_test, 1);
    vecteur_ajouter(tab_test, 2);
    vecteur_ajouter(tab_test, 3);
    vecteur_ajouter(tab_test, 4);
    vecteur_ajouter(tab_test, 5);
    vecteur_afficher(tab_test);
    vecteur_liberer(tab_test);

    printf("** Fin Tests Manuel **\n\n");

    UNITY_BEGIN();

    RUN_TEST(test_vecteur_creer_valide);
    RUN_TEST(test_vecteur_creer_taille_zero);
    RUN_TEST(test_vecteur_creer_taille_negative);
    RUN_TEST(test_vecteur_ajouter_avec_espace);
    RUN_TEST(test_vecteur_ajouter_plein);

    return UNITY_END();
}
