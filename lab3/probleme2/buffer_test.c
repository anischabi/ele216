/**
 * @file buffer_test.c
 * @brief Tests unitaires (Unity) du module buffer (file FIFO circulaire).
 *
 * Chaque test reçoit un buffer neuf, vide, de capacité 3, créé par setUp() et
 * détruit par tearDown(). Une capacité de 3 est volontairement petite : le
 * wrap-around (retour au début du tableau) arrive après quelques opérations
 * seulement.
 *
 * Deux fonctions utilitaires (enfiler_ok, defiler_attendu) évitent de
 * répéter les vérifications de retour dans chaque test.
 *
 * Pour une trace pas à pas de tete, queue et du contenu du tableau dans
 * chaque test, voir TRACES_TESTS.md.
 */

#include <stdio.h>

#include "buffer.h"
#include "unity.h"

/** Capacité du buffer créé par setUp(). */
#define CAPACITE 3      // petit buffer : le wrap-around arrive vite

/** Buffer utilisé par les tests, recréé avant chaque test par setUp(). */
static buffer_t *b = NULL;

/** Fixture : crée un buffer neuf, vide, de capacité 3 avant chaque test. */
void setUp(void) {
    b = buffer_creer(CAPACITE);
    TEST_ASSERT_NOT_NULL(b);
}

/** Fixture : détruit le buffer après chaque test (aucune fuite mémoire). */
void tearDown(void) {
    buffer_detruire(b);
    b = NULL;
}

/** Enfile une valeur et vérifie que l'ajout a réussi (retour = 1). */
static void enfiler_ok(int valeur) {
    TEST_ASSERT_EQUAL_INT(1, buffer_enfiler(b, valeur));
}

/**
 * Défile une valeur, vérifie que l'opération a réussi (retour = 1) et que la
 * valeur retirée est bien celle attendue.
 */
static void defiler_attendu(int attendu) {
    int recu = -999;
    TEST_ASSERT_EQUAL_INT(1, buffer_defiler(b, &recu));
    TEST_ASSERT_EQUAL_INT(attendu, recu);
}

/* ---------- Création valide / invalide ---------- */

/**
 * @brief Un buffer créé avec une capacité valide est correctement initialisé.
 *
 * - Scénario : buffer_creer(5).
 * - Attendu  : non NULL, capacité 5, taille 0, est_vide = 1, est_plein = 0.
 * - Note     : capacité différente de celle de setUp(), pour prouver que c'est
 *              bien le paramètre reçu qui est stocké.
 */
void test_buffer_creer_valide(void) {
    buffer_t *q = buffer_creer(5);
    TEST_ASSERT_NOT_NULL(q);
    TEST_ASSERT_EQUAL_INT(5, buffer_capacite(q));
    TEST_ASSERT_EQUAL_INT(0, buffer_taille(q));
    TEST_ASSERT_EQUAL_INT(1, buffer_est_vide(q));
    TEST_ASSERT_EQUAL_INT(0, buffer_est_plein(q));
    buffer_detruire(q);
}

/**
 * @brief Une capacité nulle est refusée.
 *
 * - Scénario : buffer_creer(0).
 * - Attendu  : NULL.
 * - Détecte  : une validation `< 0` au lieu de `<= 0` (avec une capacité de 0,
 *              le `% capacite` de buffer_enfiler ferait une division par zéro).
 */
void test_buffer_creer_capacite_nulle(void) {
    TEST_ASSERT_NULL(buffer_creer(0));
}

/**
 * @brief Une capacité négative est refusée.
 *
 * - Scénario : buffer_creer(-4).
 * - Attendu  : NULL.
 */
void test_buffer_creer_capacite_negative(void) {
    TEST_ASSERT_NULL(buffer_creer(-4));
}

/**
 * @brief Cas limite : la plus petite capacité valide (1) est acceptée.
 *
 * - Scénario : buffer_creer(1).
 * - Attendu  : non NULL, capacité 1.
 */
void test_buffer_creer_capacite_un(void) {
    buffer_t *q = buffer_creer(1);
    TEST_ASSERT_NOT_NULL(q);
    TEST_ASSERT_EQUAL_INT(1, buffer_capacite(q));
    buffer_detruire(q);
}

/* ---------- Enfilage et défilage simples (ordre FIFO) ---------- */

/**
 * @brief Un enfilage simple met à jour la taille et les indicateurs.
 *
 * - Scénario : buffer vide, enfiler(10).
 * - Attendu  : taille 1, est_vide = 0, est_plein = 0.
 */
void test_buffer_enfiler_simple(void) {
    enfiler_ok(10);
    TEST_ASSERT_EQUAL_INT(1, buffer_taille(b));
    TEST_ASSERT_EQUAL_INT(0, buffer_est_vide(b));
    TEST_ASSERT_EQUAL_INT(0, buffer_est_plein(b));
}

/**
 * @brief L'ordre FIFO est respecté : premier entré, premier sorti.
 *
 * - Scénario : enfiler 10, 20, 30, puis défiler trois fois.
 * - Attendu  : les valeurs sortent dans l'ordre 10, 20, 30, puis le buffer est vide.
 * - Détecte  : un comportement LIFO (pile) ou un ordre mélangé.
 * - Trace    : TRACES_TESTS.md, test 4.
 */
void test_buffer_ordre_fifo(void) {
    enfiler_ok(10);
    enfiler_ok(20);
    enfiler_ok(30);
    defiler_attendu(10);
    defiler_attendu(20);
    defiler_attendu(30);
    TEST_ASSERT_EQUAL_INT(1, buffer_est_vide(b));
}

/**
 * @brief Un défilage diminue la taille de 1.
 *
 * - Scénario : enfiler 10 et 20, puis défiler une fois.
 * - Attendu  : valeur retirée = 10, taille 1, est_vide = 0.
 */
void test_buffer_defiler_diminue_la_taille(void) {
    enfiler_ok(10);
    enfiler_ok(20);
    defiler_attendu(10);
    TEST_ASSERT_EQUAL_INT(1, buffer_taille(b));
    TEST_ASSERT_EQUAL_INT(0, buffer_est_vide(b));
}

/* ---------- Buffer vide ---------- */

/**
 * @brief Défiler depuis un buffer vide échoue proprement.
 *
 * - Scénario : buffer neuf, defiler dans une variable initialisée à -999.
 * - Attendu  : retour 0, la variable vaut toujours -999, taille 0.
 * - Détecte  : un buffer_defiler qui modifie *valeur ou qui lit une case
 *              invalide quand il n'y a rien à retirer.
 * - Trace    : TRACES_TESTS.md, test 6.
 */
void test_buffer_defiler_vide_retourne_0(void) {
    int recu = -999;
    TEST_ASSERT_EQUAL_INT(0, buffer_defiler(b, &recu));
    TEST_ASSERT_EQUAL_INT(-999, recu);     // *valeur n'est pas modifiée
    TEST_ASSERT_EQUAL_INT(0, buffer_taille(b));
}

/**
 * @brief Un buffer vidé (et non neuf) se comporte comme un buffer vide.
 *
 * - Scénario : enfiler 1 et 2, défiler les deux, puis défiler une fois de plus.
 * - Attendu  : le dernier defiler retourne 0 et ne modifie pas la variable
 *              de l'appelant (-999).
 * - Note     : ici tete == queue == 2 (pas 0), et le tableau contient encore
 *              1 et 2 ; c'est le compteur d'éléments qui indique « vide ».
 * - Trace    : TRACES_TESTS.md, test 7.
 */
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

/**
 * @brief Enfiler dans un buffer plein échoue et ne change rien.
 *
 * - Scénario : enfiler 1, 2, 3 (buffer plein), puis enfiler(4).
 * - Attendu  : le dernier enfiler retourne 0, taille toujours 3, est_plein = 1.
 * - Détecte  : l'absence de test « plein » : le 4 s'écrirait à `queue`, soit
 *              par-dessus le 1 (le plus ancien).
 * - Trace    : TRACES_TESTS.md, test 8.
 */
void test_buffer_enfiler_plein_retourne_0(void) {
    enfiler_ok(1);
    enfiler_ok(2);
    enfiler_ok(3);
    TEST_ASSERT_EQUAL_INT(1, buffer_est_plein(b));

    TEST_ASSERT_EQUAL_INT(0, buffer_enfiler(b, 4));
    TEST_ASSERT_EQUAL_INT(3, buffer_taille(b));     // rien n'a changé
    TEST_ASSERT_EQUAL_INT(1, buffer_est_plein(b));
}

/**
 * @brief Un enfilage refusé ne modifie pas le contenu du buffer.
 *
 * - Scénario : buffer plein (1, 2, 3), enfiler(4) refusé, puis tout défiler.
 * - Attendu  : on retrouve exactement 1, 2, 3 (le 4 n'a ni remplacé le 1 ni
 *              été ajouté à la fin), puis le buffer est vide.
 * - Trace    : TRACES_TESTS.md, test 9.
 */
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

/**
 * @brief est_plein et est_vide suivent correctement les transitions plein -> vide -> plein.
 *
 * - Scénario : remplir (1, 2, 3), tout vider, puis remplir de nouveau (4, 5, 6).
 * - Attendu  : plein après le remplissage ; plus plein dès le 1er défilage ;
 *              vide après le 3e ; plein de nouveau après 3 enfilages.
 * - Trace    : TRACES_TESTS.md, test 10.
 */
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

/**
 * @brief Le wrap-around fonctionne quand tete et queue sont décalés (pas à 0).
 *
 * - Scénario : enfiler 1, 2, 3 ; défiler 1 et 2 ; enfiler 4 et 5 ; tenter
 *              enfiler(99) ; défiler tout.
 * - Attendu  : le 4 est écrit à la case 0 alors que tete est à la case 2 ;
 *              buffer plein à 3 éléments (enfiler(99) retourne 0) ; l'ordre de
 *              sortie est 3, 4, 5 (ordre FIFO), même si le tableau brut est
 *              [4, 5, 3].
 * - Détecte  : l'oubli du `%` (écriture hors du tableau) et une lecture qui
 *              suivrait l'ordre du tableau au lieu de tete.
 * - Trace    : TRACES_TESTS.md, test 11 (le plus important).
 */
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

/**
 * @brief Plusieurs cycles complets « remplir puis vider » de suite.
 *
 * - Scénario : 5 cycles ; dans chaque cycle, enfiler trois valeurs
 *              (base + 1, +2, +3), vérifier plein, tenter un enfilage refusé,
 *              défiler les trois valeurs dans l'ordre, vérifier vide.
 * - Attendu  : à chaque cycle, ordre FIFO respecté et états plein / vide corrects.
 * - Note     : à chaque début de cycle, tete et queue valent 0 ; ce test ne
 *              détecte donc pas les bugs qui n'apparaissent qu'avec un décalage
 *              (voir test_buffer_wrap_around_avec_decalage).
 * - Trace    : TRACES_TESTS.md, test 12.
 */
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

/**
 * @brief tete et queue font plusieurs tours du tableau, en restant décalés.
 *
 * - Scénario : 20 fois de suite, enfiler(i) puis défiler (attendu : i).
 * - Attendu  : chaque valeur ressort telle quelle ; le buffer est vide à la fin.
 * - Note     : le buffer contient 0 ou 1 élément, mais tete et queue avancent
 *              ensemble et passent environ 6 fois sur chaque case : le
 *              wrap-around est exercé à toutes les positions.
 * - Trace    : TRACES_TESTS.md, test 13.
 */
void test_buffer_entrelace_plusieurs_tours(void) {
    for (int i = 0; i < 20; i++) {
        enfiler_ok(i);
        defiler_attendu(i);
    }
    TEST_ASSERT_EQUAL_INT(1, buffer_est_vide(b));
}

/**
 * @brief Cas limite : un buffer de capacité 1 (une seule case).
 *
 * - Scénario : enfiler(7), enfiler(8) refusé, défiler, enfiler(9), défiler.
 * - Attendu  : 7 puis 9 ressortent ; est_plein = 1 après le 1er enfilage ;
 *              est_vide = 1 après le défilage ; le 8 est refusé.
 * - Note     : (index + 1) % 1 vaut toujours 0 : tete == queue == 0 dans tous
 *              les états ; seul le compteur distingue plein de vide.
 * - Trace    : TRACES_TESTS.md, test 14.
 */
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

/**
 * @brief La capacité ne change jamais, même après des opérations refusées.
 *
 * - Scénario : buffer plein (1, 2, 3), enfiler(4) refusé, défiler une fois.
 * - Attendu  : buffer_capacite vaut toujours 3 (le buffer ne grandit pas,
 *              contrairement au vecteur du cours).
 * - Trace    : TRACES_TESTS.md, test 15.
 */
void test_buffer_capacite_immuable(void) {
    enfiler_ok(1);
    enfiler_ok(2);
    enfiler_ok(3);
    TEST_ASSERT_EQUAL_INT(0, buffer_enfiler(b, 4));
    defiler_attendu(1);
    TEST_ASSERT_EQUAL_INT(CAPACITE, buffer_capacite(b));
}

/**
 * @brief Détruire un pointeur NULL est permis et sans effet.
 *
 * - Scénario : buffer_detruire(NULL).
 * - Attendu  : aucun plantage (le test réussit simplement s'il arrive au bout).
 */
void test_buffer_detruire_null(void) {
    buffer_detruire(NULL);      // ne doit pas planter
    TEST_PASS();
}

/**
 * @brief Point d'entrée : démonstration de buffer_afficher, puis exécution des tests.
 *
 * La démonstration affiche le buffer vide, puis [1, 2, 3], puis [2, 3, 4]
 * (l'ordre FIFO après un wrap-around, alors que le tableau interne est
 * [4, 2, 3]). buffer_afficher ne se prête pas à une vérification automatique,
 * on la regarde à l'œil.
 */
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
