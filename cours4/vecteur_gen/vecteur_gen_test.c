#include <stdio.h>
#include <stdbool.h>

#include "vecteur_gen.h"
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
    // v1 doit etre libere pour eviter une fuite de memoire
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
    int i1 = 22;
    vecteur_ajouter(v, &i1);
    TEST_ASSERT_EQUAL(2, vecteur_capacite(v));
    TEST_ASSERT_EQUAL(1, vecteur_taille(v));
    TEST_ASSERT_FALSE(vecteur_est_vide(v));
}

void test_vecteur_ajouter_plein(void) {
    int tab[3] = {32, 42, 52};
    vecteur_ajouter(v, &tab[0]);
    vecteur_ajouter(v, &tab[1]);
    vecteur_ajouter(v, &tab[2]);
    TEST_ASSERT_EQUAL(4, vecteur_capacite(v));
    TEST_ASSERT_EQUAL(3, vecteur_taille(v));
    TEST_ASSERT_FALSE(vecteur_est_vide(v));
}

void test_vecteur_retirer_premier(void) {
    int tab[3] = {32, 42, 52};
    vecteur_ajouter(v, &tab[0]);
    vecteur_ajouter(v, &tab[1]);
    vecteur_ajouter(v, &tab[2]);
    int *p_i = vecteur_retirer_premier(v);
    TEST_ASSERT_EQUAL(4, vecteur_capacite(v));
    TEST_ASSERT_EQUAL(2, vecteur_taille(v));
    TEST_ASSERT_FALSE(vecteur_est_vide(v));
    TEST_ASSERT_EQUAL(tab[0], *p_i);
}

void test_vecteur_retirer_dernier(void) {
    int tab[3] = {32, 42, 52};
    vecteur_ajouter(v, &tab[0]);
    vecteur_ajouter(v, &tab[1]);
    vecteur_ajouter(v, &tab[2]);
    int *p_i = vecteur_retirer_dernier(v);
    TEST_ASSERT_EQUAL(4, vecteur_capacite(v));
    TEST_ASSERT_EQUAL(2, vecteur_taille(v));
    TEST_ASSERT_FALSE(vecteur_est_vide(v));
    TEST_ASSERT_EQUAL(tab[2], *p_i);
}

int main(void){

    printf("** Tests Manuel **\n");
    vecteur_t *tab_test = vecteur_creer(2);
    float tab_manuel[] = {1.0, 2.0, 3.0, 4.0, 5.0};
    vecteur_ajouter(tab_test, &tab_manuel[0]);
    vecteur_ajouter(tab_test, &tab_manuel[1]);
    vecteur_ajouter(tab_test, &tab_manuel[2]);
    vecteur_ajouter(tab_test, &tab_manuel[3]);
    vecteur_ajouter(tab_test, &tab_manuel[4]);
    vecteur_afficher(tab_test);
    vecteur_liberer(tab_test);

    printf("** Fin Tests Manuel **\n\n");

    UNITY_BEGIN();

    RUN_TEST(test_vecteur_creer_valide);
    RUN_TEST(test_vecteur_creer_taille_zero);
    RUN_TEST(test_vecteur_creer_taille_negative);
    RUN_TEST(test_vecteur_ajouter_avec_espace);
    RUN_TEST(test_vecteur_ajouter_plein);
    RUN_TEST(test_vecteur_retirer_premier);
    RUN_TEST(test_vecteur_retirer_dernier);

    return UNITY_END();
}
