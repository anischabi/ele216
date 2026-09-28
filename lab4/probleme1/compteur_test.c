/**
 * @file compteur_test.c
 * @brief Tests unitaires (Unity) du module compteur.
 *
 * Chaque test reçoit un compteur neuf créé par setUp() (seuil = 5,
 * valeur = 0, dépassements = 0) et détruit par tearDown(), donc les tests
 * sont indépendants. Les tests sont regroupés par thème, dans l'ordre de
 * l'énoncé.
 */

#include <stdio.h>

#include "compteur.h"
#include "unity.h"

/** Seuil du compteur créé par setUp(). */
#define SEUIL 5

/** Compteur utilisé par les tests, recréé avant chaque test par setUp(). */
static compteur_t *c = NULL;

/** Fixture : crée un compteur neuf (seuil = 5) avant chaque test. */
void setUp(void) {
    c = compteur_creer(SEUIL);
    TEST_ASSERT_NOT_NULL(c);
}

/** Fixture : détruit le compteur après chaque test (aucune fuite mémoire). */
void tearDown(void) {
    compteur_detruire(c);
    c = NULL;
}

/** Appelle compteur_incrementer n fois sur le compteur cpt. */
static void incrementer_n(compteur_t *cpt, int n) {
    for (int i = 0; i < n; i++) {
        compteur_incrementer(cpt);
    }
}

/* ---------- Création valide / invalide ---------- */

/**
 * @brief Un compteur créé avec un seuil valide est correctement initialisé.
 *
 * - Scénario : compteur_creer(12).
 * - Attendu  : pointeur non NULL, seuil 12, valeur 0, dépassements 0.
 * - Note     : le seuil diffère de celui de setUp(), pour prouver que c'est
 *              bien le paramètre reçu qui est stocké.
 */
void test_compteur_creer_valide(void) {
    compteur_t *c1 = compteur_creer(12);
    TEST_ASSERT_NOT_NULL(c1);
    TEST_ASSERT_EQUAL_INT(12, compteur_seuil(c1));
    TEST_ASSERT_EQUAL_INT(0, compteur_valeur(c1));
    TEST_ASSERT_EQUAL_INT(0, compteur_depassements(c1));
    compteur_detruire(c1);
}

/**
 * @brief Un seuil nul est refusé.
 *
 * - Scénario : compteur_creer(0).
 * - Attendu  : NULL.
 * - Détecte  : une validation écrite `< 0` au lieu de `<= 0`.
 */
void test_compteur_creer_seuil_nul(void) {
    TEST_ASSERT_NULL(compteur_creer(0));
}

/**
 * @brief Un seuil négatif est refusé.
 *
 * - Scénario : compteur_creer(-3).
 * - Attendu  : NULL.
 */
void test_compteur_creer_seuil_negatif(void) {
    TEST_ASSERT_NULL(compteur_creer(-3));
}

/**
 * @brief Un seuil de 1 est le plus petit seuil valide : chaque incrément
 * est un dépassement.
 *
 * - Scénario : compteur_creer(1), puis 3 incréments.
 * - Attendu  : valeur 0, 3 dépassements.
 * - Détecte  : une validation écrite `<= 1`, ou une comparaison `>` au lieu
 *              de `>=` dans compteur_incrementer (la valeur resterait à 1).
 */
void test_compteur_seuil_un(void) {
    compteur_t *c1 = compteur_creer(1);
    TEST_ASSERT_NOT_NULL(c1);
    incrementer_n(c1, 3);
    TEST_ASSERT_EQUAL_INT(0, compteur_valeur(c1));
    TEST_ASSERT_EQUAL_INT(3, compteur_depassements(c1));
    compteur_detruire(c1);
}

/**
 * @brief compteur_detruire(NULL) n'a pas d'effet.
 *
 * - Scénario : compteur_detruire(NULL).
 * - Attendu  : aucun plantage (le test se termine).
 */
void test_compteur_detruire_null(void) {
    compteur_detruire(NULL);
}

/* ---------- Incréments sans atteindre le seuil ---------- */

/**
 * @brief Des incréments sous le seuil augmentent seulement la valeur.
 *
 * - Scénario : seuil 5, 3 incréments.
 * - Attendu  : valeur 3, dépassements 0, seuil inchangé.
 */
void test_compteur_incrementer_sous_seuil(void) {
    incrementer_n(c, 3);
    TEST_ASSERT_EQUAL_INT(3, compteur_valeur(c));
    TEST_ASSERT_EQUAL_INT(0, compteur_depassements(c));
    TEST_ASSERT_EQUAL_INT(SEUIL, compteur_seuil(c));
}

/**
 * @brief Juste avant le seuil, aucun dépassement n'a encore eu lieu.
 *
 * - Scénario : seuil 5, 4 incréments (seuil - 1).
 * - Attendu  : valeur 4, dépassements 0.
 * - Détecte  : un dépassement déclenché un incrément trop tôt
 *              (comparaison `valeur + 1 >= seuil` ou `valeur >= seuil - 1`).
 */
void test_compteur_incrementer_juste_avant_seuil(void) {
    incrementer_n(c, SEUIL - 1);
    TEST_ASSERT_EQUAL_INT(SEUIL - 1, compteur_valeur(c));
    TEST_ASSERT_EQUAL_INT(0, compteur_depassements(c));
}

/* ---------- Atteinte exacte du seuil ---------- */

/**
 * @brief Atteindre exactement le seuil produit un dépassement et remet la
 * valeur à 0.
 *
 * - Scénario : seuil 5, 5 incréments.
 * - Attendu  : valeur 0, dépassements 1.
 * - Détecte  : une comparaison `>` au lieu de `>=` (la valeur vaudrait 5 et
 *              le dépassement n'aurait lieu qu'au 6e incrément).
 */
void test_compteur_atteinte_exacte_seuil(void) {
    incrementer_n(c, SEUIL);
    TEST_ASSERT_EQUAL_INT(0, compteur_valeur(c));
    TEST_ASSERT_EQUAL_INT(1, compteur_depassements(c));
}

/**
 * @brief Après un dépassement, le compteur repart normalement de 0.
 *
 * - Scénario : seuil 5, 5 + 2 incréments.
 * - Attendu  : valeur 2, dépassements 1.
 * - Détecte  : une valeur remise à 1 au lieu de 0, ou un dépassement compté
 *              à chaque incrément une fois le seuil franchi.
 */
void test_compteur_repart_apres_depassement(void) {
    incrementer_n(c, SEUIL + 2);
    TEST_ASSERT_EQUAL_INT(2, compteur_valeur(c));
    TEST_ASSERT_EQUAL_INT(1, compteur_depassements(c));
}

/* ---------- Plusieurs cycles consécutifs ---------- */

/**
 * @brief Trois cycles complets donnent trois dépassements.
 *
 * - Scénario : seuil 5, 15 incréments (3 * seuil).
 * - Attendu  : valeur 0, dépassements 3.
 */
void test_compteur_plusieurs_cycles_complets(void) {
    incrementer_n(c, 3 * SEUIL);
    TEST_ASSERT_EQUAL_INT(0, compteur_valeur(c));
    TEST_ASSERT_EQUAL_INT(3, compteur_depassements(c));
}

/**
 * @brief Plusieurs cycles suivis d'un cycle partiel.
 *
 * - Scénario : seuil 5, 23 incréments.
 * - Attendu  : 23 = 4 * 5 + 3, donc dépassements 4 et valeur 3.
 * - Trace    : la valeur suit 1 2 3 4 0 | 1 2 3 4 0 | ... | 1 2 3.
 */
void test_compteur_plusieurs_cycles_et_reste(void) {
    incrementer_n(c, 23);
    TEST_ASSERT_EQUAL_INT(3, compteur_valeur(c));
    TEST_ASSERT_EQUAL_INT(4, compteur_depassements(c));
    TEST_ASSERT_EQUAL_INT(SEUIL, compteur_seuil(c));
}

/* ---------- Réinitialisation ---------- */

/**
 * @brief La réinitialisation remet la valeur à 0 et préserve les
 * dépassements.
 *
 * - Scénario : seuil 5, 13 incréments (2 dépassements, valeur 3), puis
 *              compteur_reinitialiser.
 * - Attendu  : valeur 0, dépassements toujours 2, seuil inchangé.
 * - Détecte  : une réinitialisation qui remet aussi depassements à 0.
 */
void test_compteur_reinitialiser_preserve_depassements(void) {
    incrementer_n(c, 2 * SEUIL + 3);
    TEST_ASSERT_EQUAL_INT(3, compteur_valeur(c));
    TEST_ASSERT_EQUAL_INT(2, compteur_depassements(c));

    compteur_reinitialiser(c);
    TEST_ASSERT_EQUAL_INT(0, compteur_valeur(c));
    TEST_ASSERT_EQUAL_INT(2, compteur_depassements(c));
    TEST_ASSERT_EQUAL_INT(SEUIL, compteur_seuil(c));
}

/**
 * @brief Après une réinitialisation, il faut de nouveau « seuil » incréments
 * pour obtenir un dépassement.
 *
 * - Scénario : seuil 5, 3 incréments, réinitialisation, puis 4 incréments.
 * - Attendu  : valeur 4, dépassements 0 (sans la réinitialisation, 3 + 4 = 7
 *              aurait donné 1 dépassement). Un 5e incrément donne ensuite
 *              valeur 0 et 1 dépassement.
 * - Détecte  : une réinitialisation qui ne remet pas vraiment la valeur à 0.
 */
void test_compteur_reinitialiser_puis_incrementer(void) {
    incrementer_n(c, 3);
    compteur_reinitialiser(c);
    incrementer_n(c, SEUIL - 1);
    TEST_ASSERT_EQUAL_INT(SEUIL - 1, compteur_valeur(c));
    TEST_ASSERT_EQUAL_INT(0, compteur_depassements(c));

    compteur_incrementer(c);
    TEST_ASSERT_EQUAL_INT(0, compteur_valeur(c));
    TEST_ASSERT_EQUAL_INT(1, compteur_depassements(c));
}

/**
 * @brief Réinitialiser un compteur neuf ne change rien.
 *
 * - Scénario : compteur neuf, compteur_reinitialiser.
 * - Attendu  : valeur 0, dépassements 0.
 */
void test_compteur_reinitialiser_compteur_neuf(void) {
    compteur_reinitialiser(c);
    TEST_ASSERT_EQUAL_INT(0, compteur_valeur(c));
    TEST_ASSERT_EQUAL_INT(0, compteur_depassements(c));
}

/**
 * @brief Point d'entrée : démonstration de compteur_afficher, puis exécution des tests.
 *
 * La démonstration affiche un compteur de seuil 3 après chacun de 7
 * incréments, puis après une réinitialisation ; compteur_afficher ne se
 * prête pas à une vérification automatique, on la regarde à l'œil.
 */
int main(void) {
    printf("** Demonstration de compteur_afficher **\n");
    compteur_t *demo = compteur_creer(3);
    compteur_afficher(demo);
    for (int i = 1; i <= 7; i++) {
        compteur_incrementer(demo);
        printf("apres %d increment(s) : ", i);
        compteur_afficher(demo);
    }
    compteur_reinitialiser(demo);
    printf("apres reinitialisation : ");
    compteur_afficher(demo);
    compteur_detruire(demo);
    printf("** Fin de la demonstration **\n\n");

    UNITY_BEGIN();

    RUN_TEST(test_compteur_creer_valide);
    RUN_TEST(test_compteur_creer_seuil_nul);
    RUN_TEST(test_compteur_creer_seuil_negatif);
    RUN_TEST(test_compteur_seuil_un);
    RUN_TEST(test_compteur_detruire_null);

    RUN_TEST(test_compteur_incrementer_sous_seuil);
    RUN_TEST(test_compteur_incrementer_juste_avant_seuil);

    RUN_TEST(test_compteur_atteinte_exacte_seuil);
    RUN_TEST(test_compteur_repart_apres_depassement);

    RUN_TEST(test_compteur_plusieurs_cycles_complets);
    RUN_TEST(test_compteur_plusieurs_cycles_et_reste);

    RUN_TEST(test_compteur_reinitialiser_preserve_depassements);
    RUN_TEST(test_compteur_reinitialiser_puis_incrementer);
    RUN_TEST(test_compteur_reinitialiser_compteur_neuf);

    return UNITY_END();
}
