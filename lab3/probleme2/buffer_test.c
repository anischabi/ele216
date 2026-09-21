#include <stdio.h>

#include "buffer.h"
#include "unity.h"

#define CAPACITE 3      // petit buffer : le wrap-around arrive vite

static buffer_t *b = NULL;

// Fixture : chaque test reçoit un buffer neuf, vide, de capacité 3.
void setUp(void) {
    b = buffer_creer(CAPACITE);
    TEST_ASSERT_NOT_NULL(b);
}

void tearDown(void) {
    buffer_detruire(b);
    b = NULL;
}

// Enfile une valeur et vérifie que l'ajout a réussi.
static void enfiler_ok(int valeur) {
    TEST_ASSERT_EQUAL_INT(1, buffer_enfiler(b, valeur));
}

// Défile une valeur, vérifie que l'opération a réussi et que la valeur
// retirée est bien celle attendue.
static void defiler_attendu(int attendu) {
    int recu = -999;
    TEST_ASSERT_EQUAL_INT(1, buffer_defiler(b, &recu));
    TEST_ASSERT_EQUAL_INT(attendu, recu);
}

/* ---------- Création valide / invalide ---------- */

void test_buffer_creer_valide(void) {
    buffer_t *q = buffer_creer(5);
    TEST_ASSERT_NOT_NULL(q);
    TEST_ASSERT_EQUAL_INT(5, buffer_capacite(q));
    TEST_ASSERT_EQUAL_INT(0, buffer_taille(q));
    TEST_ASSERT_EQUAL_INT(1, buffer_est_vide(q));
    TEST_ASSERT_EQUAL_INT(0, buffer_est_plein(q));
    buffer_detruire(q);
}

void test_buffer_creer_capacite_nulle(void) {
    TEST_ASSERT_NULL(buffer_creer(0));
}

void test_buffer_creer_capacite_negative(void) {
    TEST_ASSERT_NULL(buffer_creer(-4));
}

void test_buffer_creer_capacite_un(void) {
    buffer_t *q = buffer_creer(1);
    TEST_ASSERT_NOT_NULL(q);
    TEST_ASSERT_EQUAL_INT(1, buffer_capacite(q));
    buffer_detruire(q);
}

/* ---------- Enfilage et défilage simples (ordre FIFO) ---------- */

void test_buffer_enfiler_simple(void) {
    enfiler_ok(10);
    TEST_ASSERT_EQUAL_INT(1, buffer_taille(b));
    TEST_ASSERT_EQUAL_INT(0, buffer_est_vide(b));
    TEST_ASSERT_EQUAL_INT(0, buffer_est_plein(b));
}

void test_buffer_ordre_fifo(void) {
    enfiler_ok(10);
    enfiler_ok(20);
    enfiler_ok(30);
    defiler_attendu(10);
    defiler_attendu(20);
    defiler_attendu(30);
    TEST_ASSERT_EQUAL_INT(1, buffer_est_vide(b));
}

void test_buffer_defiler_diminue_la_taille(void) {
    enfiler_ok(10);
    enfiler_ok(20);
    defiler_attendu(10);
    TEST_ASSERT_EQUAL_INT(1, buffer_taille(b));
    TEST_ASSERT_EQUAL_INT(0, buffer_est_vide(b));
}

/* ---------- Buffer vide ---------- */

void test_buffer_defiler_vide_retourne_0(void) {
    int recu = -999;
    TEST_ASSERT_EQUAL_INT(0, buffer_defiler(b, &recu));
    TEST_ASSERT_EQUAL_INT(-999, recu);     // *valeur n'est pas modifiée
    TEST_ASSERT_EQUAL_INT(0, buffer_taille(b));
}

void test_buffer_defiler_vide_apres_avoir_tout_retire(void) {
    enfiler_ok(1);
    enfiler_ok(2);
    defiler_attendu(1);
    defiler_attendu(2);

    int recu = -999;
    TEST_ASSERT_EQUAL_INT(0, buffer_defiler(b, &recu));
    TEST_ASSERT_EQUAL_INT(-999, recu);
}

/* ---------- Buffer plein ---------- */

void test_buffer_enfiler_plein_retourne_0(void) {
    enfiler_ok(1);
    enfiler_ok(2);
    enfiler_ok(3);
    TEST_ASSERT_EQUAL_INT(1, buffer_est_plein(b));

    TEST_ASSERT_EQUAL_INT(0, buffer_enfiler(b, 4));
    TEST_ASSERT_EQUAL_INT(3, buffer_taille(b));     // rien n'a changé
    TEST_ASSERT_EQUAL_INT(1, buffer_est_plein(b));
}

void test_buffer_enfiler_plein_ne_modifie_pas_le_contenu(void) {
    enfiler_ok(1);
    enfiler_ok(2);
    enfiler_ok(3);
    TEST_ASSERT_EQUAL_INT(0, buffer_enfiler(b, 4));

    // Le 4 refusé ne doit pas avoir écrasé le plus ancien ni s'être ajouté.
    defiler_attendu(1);
    defiler_attendu(2);
    defiler_attendu(3);
    TEST_ASSERT_EQUAL_INT(1, buffer_est_vide(b));
}

void test_buffer_plein_puis_vide_puis_plein(void) {
    enfiler_ok(1);
    enfiler_ok(2);
    enfiler_ok(3);
    TEST_ASSERT_EQUAL_INT(1, buffer_est_plein(b));
    TEST_ASSERT_EQUAL_INT(0, buffer_est_vide(b));

    defiler_attendu(1);
    TEST_ASSERT_EQUAL_INT(0, buffer_est_plein(b));
    defiler_attendu(2);
    defiler_attendu(3);
    TEST_ASSERT_EQUAL_INT(1, buffer_est_vide(b));
    TEST_ASSERT_EQUAL_INT(0, buffer_est_plein(b));

    enfiler_ok(4);
    enfiler_ok(5);
    enfiler_ok(6);
    TEST_ASSERT_EQUAL_INT(1, buffer_est_plein(b));
}

/* ---------- Wrap-around ---------- */

// tete et queue sont décalés (pas à l'index 0) quand la queue "reboucle".
void test_buffer_wrap_around_avec_decalage(void) {
    enfiler_ok(1);
    enfiler_ok(2);
    enfiler_ok(3);
    defiler_attendu(1);
    defiler_attendu(2);
    // Le tableau interne contient maintenant [1, 2, 3] mais seul le 3 est valide.
    enfiler_ok(4);          // reboucle à l'index 0
    enfiler_ok(5);          // index 1
    TEST_ASSERT_EQUAL_INT(3, buffer_taille(b));
    TEST_ASSERT_EQUAL_INT(1, buffer_est_plein(b));
    TEST_ASSERT_EQUAL_INT(0, buffer_enfiler(b, 99));    // plein aussi après le wrap

    defiler_attendu(3);
    defiler_attendu(4);
    defiler_attendu(5);
    TEST_ASSERT_EQUAL_INT(1, buffer_est_vide(b));
}

// Plusieurs cycles complets remplir / vider sur un buffer de 3.
void test_buffer_plusieurs_cycles_remplir_vider(void) {
    for (int cycle = 0; cycle < 5; cycle++) {
        int base = cycle * 10;

        enfiler_ok(base + 1);
        enfiler_ok(base + 2);
        enfiler_ok(base + 3);
        TEST_ASSERT_EQUAL_INT(1, buffer_est_plein(b));
        TEST_ASSERT_EQUAL_INT(0, buffer_enfiler(b, -1));

        defiler_attendu(base + 1);
        defiler_attendu(base + 2);
        defiler_attendu(base + 3);
        TEST_ASSERT_EQUAL_INT(1, buffer_est_vide(b));
    }
}

// Un enfilage suivi d'un défilage, plusieurs fois plus que la capacité :
// tete et queue font plusieurs tours du tableau.
void test_buffer_entrelace_plusieurs_tours(void) {
    for (int i = 0; i < 20; i++) {
        enfiler_ok(i);
        defiler_attendu(i);
    }
    TEST_ASSERT_EQUAL_INT(1, buffer_est_vide(b));
}

void test_buffer_capacite_un_enfiler_defiler(void) {
    buffer_t *q = buffer_creer(1);
    TEST_ASSERT_NOT_NULL(q);

    TEST_ASSERT_EQUAL_INT(1, buffer_enfiler(q, 7));
    TEST_ASSERT_EQUAL_INT(1, buffer_est_plein(q));
    TEST_ASSERT_EQUAL_INT(0, buffer_enfiler(q, 8));

    int recu = -999;
    TEST_ASSERT_EQUAL_INT(1, buffer_defiler(q, &recu));
    TEST_ASSERT_EQUAL_INT(7, recu);
    TEST_ASSERT_EQUAL_INT(1, buffer_est_vide(q));

    TEST_ASSERT_EQUAL_INT(1, buffer_enfiler(q, 9));
    TEST_ASSERT_EQUAL_INT(1, buffer_defiler(q, &recu));
    TEST_ASSERT_EQUAL_INT(9, recu);

    buffer_detruire(q);
}

/* ---------- Immuabilité et destruction ---------- */

void test_buffer_capacite_immuable(void) {
    enfiler_ok(1);
    enfiler_ok(2);
    enfiler_ok(3);
    TEST_ASSERT_EQUAL_INT(0, buffer_enfiler(b, 4));
    defiler_attendu(1);
    TEST_ASSERT_EQUAL_INT(CAPACITE, buffer_capacite(b));
}

void test_buffer_detruire_null(void) {
    buffer_detruire(NULL);      // ne doit pas planter
    TEST_PASS();
}

int main(void) {
    printf("** Demonstration de buffer_afficher **\n");
    buffer_t *demo = buffer_creer(3);
    buffer_afficher(demo);                  // vide
    buffer_enfiler(demo, 1);
    buffer_enfiler(demo, 2);
    buffer_enfiler(demo, 3);
    buffer_afficher(demo);                  // 1, 2, 3
    int retire;
    buffer_defiler(demo, &retire);
    buffer_enfiler(demo, 4);
    buffer_afficher(demo);                  // 2, 3, 4 (ordre FIFO, apres wrap-around)
    buffer_detruire(demo);
    printf("** Fin de la demonstration **\n\n");

    UNITY_BEGIN();

    RUN_TEST(test_buffer_creer_valide);
    RUN_TEST(test_buffer_creer_capacite_nulle);
    RUN_TEST(test_buffer_creer_capacite_negative);
    RUN_TEST(test_buffer_creer_capacite_un);

    RUN_TEST(test_buffer_enfiler_simple);
    RUN_TEST(test_buffer_ordre_fifo);
    RUN_TEST(test_buffer_defiler_diminue_la_taille);

    RUN_TEST(test_buffer_defiler_vide_retourne_0);
    RUN_TEST(test_buffer_defiler_vide_apres_avoir_tout_retire);

    RUN_TEST(test_buffer_enfiler_plein_retourne_0);
    RUN_TEST(test_buffer_enfiler_plein_ne_modifie_pas_le_contenu);
    RUN_TEST(test_buffer_plein_puis_vide_puis_plein);

    RUN_TEST(test_buffer_wrap_around_avec_decalage);
    RUN_TEST(test_buffer_plusieurs_cycles_remplir_vider);
    RUN_TEST(test_buffer_entrelace_plusieurs_tours);
    RUN_TEST(test_buffer_capacite_un_enfiler_defiler);

    RUN_TEST(test_buffer_capacite_immuable);
    RUN_TEST(test_buffer_detruire_null);

    return UNITY_END();
}
