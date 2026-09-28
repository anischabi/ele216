#include <stdio.h>
#include <stdbool.h>

#include "pile_gen.h"
#include "unity.h"

pile_gen_t *p = NULL;

void setUp(void) {
    p = pile_gen_creer();
}

void tearDown(void) {
    pile_gen_liberer(p);
}

void test_pile_gen_creer_valide(void) {
    pile_gen_t *p1 = pile_gen_creer();
    TEST_ASSERT_NOT_NULL(p1);
    TEST_ASSERT_TRUE(pile_gen_est_vide(p1));
    TEST_ASSERT_EQUAL(0, pile_gen_taille(p1));
}

void test_pile_gen_empiler(void) {
    int i1 = 22;
    pile_gen_empiler(p, &i1);
    TEST_ASSERT_FALSE(pile_gen_est_vide(p));
    TEST_ASSERT_EQUAL(1, pile_gen_taille(p));
}

void test_pile_gen_depiler(void) {
    int i1 = 22;
    int i2 = 32;
    pile_gen_empiler(p, &i1);
    pile_gen_empiler(p, &i2);
    int *p_i = pile_gen_depiler(p);
    TEST_ASSERT_EQUAL(i2, *p_i);
    TEST_ASSERT_FALSE(pile_gen_est_vide(p));
    TEST_ASSERT_EQUAL(1, pile_gen_taille(p));
}

int main(void){
    printf("** Tests Manuel **\n");

    printf("** Fin Tests Manuel **\n\n");

    UNITY_BEGIN();

    RUN_TEST(test_pile_gen_creer_valide);
    RUN_TEST(test_pile_gen_empiler);
    RUN_TEST(test_pile_gen_depiler);

    return UNITY_END();
}
