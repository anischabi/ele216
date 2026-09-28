#include <stdio.h>
#include <stdbool.h>

#include "file_gen.h"
#include "unity.h"

file_gen_t *f = NULL;

void setUp(void) {
    f = file_gen_creer();
}

void tearDown(void) {
    file_gen_liberer(f);
}

void test_file_gen_creer_valide(void) {
    file_gen_t *f1 = file_gen_creer();
    TEST_ASSERT_NOT_NULL(f1);
    TEST_ASSERT_TRUE(file_gen_est_vide(f1));
    TEST_ASSERT_EQUAL(0, file_gen_taille(f1));
    // f1 doit etre libere pour eviter une fuite de memoire
}

void test_file_gen_enfiler(void) {
    int i1 = 22;
    file_gen_enfiler(f, &i1);
    TEST_ASSERT_FALSE(file_gen_est_vide(f));
    TEST_ASSERT_EQUAL(1, file_gen_taille(f));
}

void test_file_gen_defiler(void) {
    int i1 = 22;
    int i2 = 32;
    file_gen_enfiler(f, &i1);
    file_gen_enfiler(f, &i2);
    int *p_i = file_gen_defiler(f);
    TEST_ASSERT_EQUAL(i1, *p_i);
    TEST_ASSERT_FALSE(file_gen_est_vide(f));
    TEST_ASSERT_EQUAL(1, file_gen_taille(f));
}

int main(void){
    printf("** Tests Manuel **\n");

    printf("** Fin Tests Manuel **\n\n");

    UNITY_BEGIN();

    RUN_TEST(test_file_gen_creer_valide);
    RUN_TEST(test_file_gen_enfiler);
    RUN_TEST(test_file_gen_defiler);

    return UNITY_END();
}
