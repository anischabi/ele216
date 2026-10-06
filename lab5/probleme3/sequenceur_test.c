/**
 * @file sequenceur_test.c
 * @brief Tests unitaires (Unity) du module sequenceur.
 *
 * Chaque test reçoit un séquenceur et un journal vides créés par setUp()
 * et détruits par tearDown(), donc les tests sont indépendants. Les tests
 * de CMD_MESURER fixent le seed avec srand() ; comme la suite de rand()
 * dépend de la bibliothèque C, ils vérifient des propriétés (bornes du
 * bruit, reproductibilité) plutôt qu'une valeur exacte. Les tests suivent
 * l'ordre de l'énoncé, plus quelques cas bonus.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sequenceur.h"
#include "unity.h"

/** Précision des comparaisons de float. */
#define EPSILON 1e-6f

/** Seed fixe pour les tests de CMD_MESURER. */
#define SEED 42

/** Séquenceur et journal recréés avant chaque test par setUp(). */
static sequenceur_t *s = NULL;
static journal_t *j = NULL;

/** Fixture : crée un séquenceur et un journal vides avant chaque test. */
void setUp(void) {
    s = sequenceur_creer();
    j = journal_creer();
    TEST_ASSERT_NOT_NULL(s);
    TEST_ASSERT_NOT_NULL(j);
}

/** Fixture : détruit le séquenceur et le journal après chaque test. */
void tearDown(void) {
    sequenceur_detruire(s);
    journal_detruire(j);
    s = NULL;
    j = NULL;
}

/**
 * Construit une commande. id peut être NULL pour les commandes qui n'en
 * utilisent pas.
 */
static commande_t commande(type_commande_t type, const char *id, float valeur) {
    commande_t c;
    memset(&c, 0, sizeof(c));
    c.type = type;
    if (id != NULL) {
        strncpy(c.id, id, COMMANDE_ID_MAX - 1);
    }
    c.valeur = valeur;
    return c;
}

/** Enfile une commande sans id ni valeur (PUSH, POP, RAPPORT). */
static void enfiler_simple(type_commande_t type) {
    sequenceur_enfiler(s, commande(type, NULL, 0.0f));
}

/** Enfile une commande CMD_SET_* avec sa valeur. */
static void enfiler_set(type_commande_t type, float valeur) {
    sequenceur_enfiler(s, commande(type, NULL, valeur));
}

/** Vérifie les trois champs du contexte courant. */
static void verifier_contexte(float tension, float courant, float tolerance) {
    const config_t *ctx = sequenceur_contexte(s);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, tension, ctx->tension);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, courant, ctx->courant);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, tolerance, ctx->tolerance);
}

/* ---------- Création et destruction ---------- */

/** Création : file vide et contexte par défaut (0 V, 0 A, 5 %). */
void test_sequenceur_creer_defaut(void) {
    TEST_ASSERT_EQUAL_INT(0, sequenceur_nb_en_attente(s));
    verifier_contexte(0.0f, 0.0f, 5.0f);
}

/**
 * Destruction avec des commandes en attente et des contextes empilés :
 * sequenceur_detruire doit libérer chaque élément (vérifier avec un outil
 * de détection de fuites, ex. Dr. Memory ou valgrind).
 */
void test_sequenceur_detruire_rempli(void) {
    enfiler_simple(CMD_PUSH);
    enfiler_simple(CMD_PUSH);
    sequenceur_executer_tout(s, j);  // Deux contextes restent empilés.
    for (int i = 0; i < 10; i++) {   // Dépasse la capacité initiale (4).
        enfiler_set(CMD_SET_TENSION, (float)i);
    }
    TEST_ASSERT_EQUAL_INT(10, sequenceur_nb_en_attente(s));
    // tearDown détruit le séquenceur dans cet état.
}

/** sequenceur_detruire(NULL) n'a pas d'effet. */
void test_sequenceur_detruire_null(void) {
    sequenceur_detruire(NULL);
}

/* ---------- Enfilage et compteur en attente ---------- */

/** Le compteur suit les enfilages et les exécutions. */
void test_sequenceur_enfiler_compteur(void) {
    enfiler_set(CMD_SET_TENSION, 3.3f);
    TEST_ASSERT_EQUAL_INT(1, sequenceur_nb_en_attente(s));
    enfiler_simple(CMD_PUSH);
    enfiler_set(CMD_SET_COURANT, 0.5f);
    TEST_ASSERT_EQUAL_INT(3, sequenceur_nb_en_attente(s));

    sequenceur_executer_prochaine(s, j);
    TEST_ASSERT_EQUAL_INT(2, sequenceur_nb_en_attente(s));
}

/** Enfiler ne modifie pas le contexte : l'exécution est différée. */
void test_sequenceur_enfiler_n_execute_pas(void) {
    enfiler_set(CMD_SET_TENSION, 12.0f);
    verifier_contexte(0.0f, 0.0f, 5.0f);
}

/** Un id sans '\0' (tableau plein) est tronqué dans la copie enfilée. */
void test_sequenceur_enfiler_id_sans_terminateur(void) {
    commande_t c = commande(CMD_MESURER, NULL, 1.0f);
    memset(c.id, 'A', COMMANDE_ID_MAX);  // Aucun '\0'.
    sequenceur_enfiler(s, c);

    srand(SEED);
    sequenceur_executer_prochaine(s, j);
    const resultat_t *r = journal_obtenir(j, 0);
    TEST_ASSERT_NOT_NULL(r);
    TEST_ASSERT_EQUAL_size_t(COMMANDE_ID_MAX - 1, strlen(resultat_id(r)));
}

/* ---------- Commandes CMD_SET_* ---------- */

/** CMD_SET_TENSION modifie seulement la tension. */
void test_sequenceur_set_tension(void) {
    enfiler_set(CMD_SET_TENSION, 3.3f);
    TEST_ASSERT_EQUAL_INT(1, sequenceur_executer_prochaine(s, j));
    verifier_contexte(3.3f, 0.0f, 5.0f);
}

/** CMD_SET_COURANT et CMD_SET_TOLERANCE modifient leur champ. */
void test_sequenceur_set_courant_tolerance(void) {
    enfiler_set(CMD_SET_COURANT, 0.25f);
    enfiler_set(CMD_SET_TOLERANCE, 2.0f);
    sequenceur_executer_tout(s, j);
    verifier_contexte(0.0f, 0.25f, 2.0f);
}

/** Les commandes sont exécutées dans l'ordre d'arrivée (FIFO). */
void test_sequenceur_ordre_fifo(void) {
    enfiler_set(CMD_SET_TENSION, 1.0f);
    enfiler_set(CMD_SET_TENSION, 2.0f);

    sequenceur_executer_prochaine(s, j);
    verifier_contexte(1.0f, 0.0f, 5.0f);
    sequenceur_executer_prochaine(s, j);
    verifier_contexte(2.0f, 0.0f, 5.0f);
}

/* ---------- CMD_PUSH et CMD_POP ---------- */

/** PUSH, modifications, POP : le contexte d'avant le PUSH est restauré. */
void test_sequenceur_push_modif_pop_restaure(void) {
    enfiler_set(CMD_SET_TENSION, 5.0f);
    enfiler_simple(CMD_PUSH);
    enfiler_set(CMD_SET_TENSION, 12.0f);
    enfiler_set(CMD_SET_COURANT, 1.5f);
    enfiler_set(CMD_SET_TOLERANCE, 1.0f);

    sequenceur_executer_tout(s, j);
    verifier_contexte(12.0f, 1.5f, 1.0f);  // Le PUSH n'a pas figé le courant.

    enfiler_simple(CMD_POP);
    sequenceur_executer_prochaine(s, j);
    verifier_contexte(5.0f, 0.0f, 5.0f);
}

/** PUSH imbriqués : les POP restaurent les contextes en ordre inverse. */
void test_sequenceur_push_imbrique(void) {
    enfiler_simple(CMD_PUSH);              // Sauvegarde (0, 0, 5).
    enfiler_set(CMD_SET_TENSION, 3.3f);
    enfiler_simple(CMD_PUSH);              // Sauvegarde (3.3, 0, 5).
    enfiler_set(CMD_SET_TENSION, 5.0f);
    sequenceur_executer_tout(s, j);
    verifier_contexte(5.0f, 0.0f, 5.0f);

    enfiler_simple(CMD_POP);
    sequenceur_executer_prochaine(s, j);
    verifier_contexte(3.3f, 0.0f, 5.0f);

    enfiler_simple(CMD_POP);
    sequenceur_executer_prochaine(s, j);
    verifier_contexte(0.0f, 0.0f, 5.0f);
}

/** CMD_POP sur une pile vide : pas de plantage, contexte conservé. */
void test_sequenceur_pop_pile_vide(void) {
    enfiler_set(CMD_SET_TENSION, 3.3f);
    enfiler_simple(CMD_POP);
    enfiler_simple(CMD_POP);

    TEST_ASSERT_EQUAL_INT(1, sequenceur_executer_prochaine(s, j));
    // La commande est consommée même si elle est ignorée.
    TEST_ASSERT_EQUAL_INT(1, sequenceur_executer_prochaine(s, j));
    TEST_ASSERT_EQUAL_INT(1, sequenceur_executer_prochaine(s, j));
    TEST_ASSERT_EQUAL_INT(0, sequenceur_nb_en_attente(s));
    verifier_contexte(3.3f, 0.0f, 5.0f);
}

/* ---------- CMD_MESURER ---------- */

/** CMD_MESURER ajoute un résultat (id, nominal, ±10 %, tolérance courante). */
void test_sequenceur_mesurer_ajoute_resultat(void) {
    srand(SEED);
    enfiler_set(CMD_SET_TOLERANCE, 2.0f);
    sequenceur_enfiler(s, commande(CMD_MESURER, "V_REF_3V3", 3.3f));
    sequenceur_executer_tout(s, j);

    TEST_ASSERT_EQUAL_INT(1, journal_nb_resultats(j));
    const resultat_t *r = journal_obtenir(j, 0);
    TEST_ASSERT_NOT_NULL(r);
    TEST_ASSERT_EQUAL_STRING("V_REF_3V3", resultat_id(r));
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 3.3f, resultat_attendu(r));
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 2.0f, resultat_tolerance(r));
    // Bruit de ±10 % : la mesure est dans [2.97, 3.63].
    TEST_ASSERT_FLOAT_WITHIN(0.33f + EPSILON, 3.3f, resultat_mesure(r));
}

/**
 * Sur plusieurs tirages, le bruit reste dans ±10 % et n'est pas nul : les
 * mesures ne sont pas toutes égales (sinon la simulation ne simule rien).
 */
void test_sequenceur_mesurer_bornes_bruit(void) {
    srand(SEED);
    for (int i = 0; i < 50; i++) {
        sequenceur_enfiler(s, commande(CMD_MESURER, "V_5V", 5.0f));
    }
    sequenceur_executer_tout(s, j);

    TEST_ASSERT_EQUAL_INT(50, journal_nb_resultats(j));
    float min = resultat_mesure(journal_obtenir(j, 0));
    float max = min;
    for (int i = 0; i < 50; i++) {
        float m = resultat_mesure(journal_obtenir(j, i));
        TEST_ASSERT_FLOAT_WITHIN(0.5f + EPSILON, 5.0f, m);
        if (m < min) { min = m; }
        if (m > max) { max = m; }
    }
    TEST_ASSERT_TRUE(max - min > EPSILON);
}

/** Même seed, même mesure : c'est ce qui rend les tests reproductibles. */
void test_sequenceur_mesurer_reproductible(void) {
    srand(SEED);
    sequenceur_enfiler(s, commande(CMD_MESURER, "T1", 10.0f));
    sequenceur_executer_prochaine(s, j);

    srand(SEED);
    sequenceur_enfiler(s, commande(CMD_MESURER, "T2", 10.0f));
    sequenceur_executer_prochaine(s, j);

    TEST_ASSERT_EQUAL_FLOAT(resultat_mesure(journal_obtenir(j, 0)),
                            resultat_mesure(journal_obtenir(j, 1)));
}

/** CMD_MESURER sans journal : ignorée, pas de plantage. */
void test_sequenceur_mesurer_journal_null(void) {
    sequenceur_enfiler(s, commande(CMD_MESURER, "V_REF", 1.0f));
    TEST_ASSERT_EQUAL_INT(1, sequenceur_executer_prochaine(s, NULL));
    TEST_ASSERT_EQUAL_INT(0, journal_nb_resultats(j));
}

/* ---------- Exécution ---------- */

/** Exécuter depuis une file vide retourne 0. */
void test_sequenceur_executer_file_vide(void) {
    TEST_ASSERT_EQUAL_INT(0, sequenceur_executer_prochaine(s, j));
}

/** executer_tout vide la file et applique toutes les commandes. */
void test_sequenceur_executer_tout(void) {
    enfiler_set(CMD_SET_TENSION, 3.3f);
    enfiler_set(CMD_SET_COURANT, 0.1f);
    enfiler_simple(CMD_PUSH);
    enfiler_set(CMD_SET_TENSION, 5.0f);
    enfiler_simple(CMD_POP);

    sequenceur_executer_tout(s, j);
    TEST_ASSERT_EQUAL_INT(0, sequenceur_nb_en_attente(s));
    verifier_contexte(3.3f, 0.1f, 5.0f);
    TEST_ASSERT_EQUAL_INT(0, sequenceur_executer_prochaine(s, j));
}

/** CMD_RAPPORT s'exécute (affiche le journal) sans modifier l'état. */
void test_sequenceur_rapport(void) {
    journal_ajouter(j, "V_REF", 3.3f, 3.3f, 5.0f);
    enfiler_simple(CMD_RAPPORT);
    TEST_ASSERT_EQUAL_INT(1, sequenceur_executer_prochaine(s, j));
    TEST_ASSERT_EQUAL_INT(1, journal_nb_resultats(j));
}

/* ---------- Affichage de la file ---------- */

/** afficher_file conserve le nombre et l'ordre des commandes. */
void test_sequenceur_afficher_file_preserve(void) {
    enfiler_set(CMD_SET_TENSION, 1.0f);
    enfiler_set(CMD_SET_TENSION, 2.0f);
    enfiler_set(CMD_SET_TENSION, 3.0f);

    sequenceur_afficher_file(s);
    TEST_ASSERT_EQUAL_INT(3, sequenceur_nb_en_attente(s));

    sequenceur_executer_prochaine(s, j);
    verifier_contexte(1.0f, 0.0f, 5.0f);
    sequenceur_executer_prochaine(s, j);
    verifier_contexte(2.0f, 0.0f, 5.0f);
}

/** afficher_file sur une file vide : pas de plantage. */
void test_sequenceur_afficher_file_vide(void) {
    sequenceur_afficher_file(s);
    TEST_ASSERT_EQUAL_INT(0, sequenceur_nb_en_attente(s));
}

int main(void) {
    // Démonstration visuelle : scénario complet avec PUSH/POP et rapport.
    printf("** Demonstration du sequenceur **\n");
    srand(SEED);
    sequenceur_t *demo = sequenceur_creer();
    journal_t *jdemo = journal_creer();
    sequenceur_enfiler(demo, commande(CMD_SET_TENSION, NULL, 3.3f));
    sequenceur_enfiler(demo, commande(CMD_SET_COURANT, NULL, 0.5f));
    sequenceur_enfiler(demo, commande(CMD_MESURER, "V_REF_3V3", 3.3f));
    sequenceur_enfiler(demo, commande(CMD_PUSH, NULL, 0.0f));
    sequenceur_enfiler(demo, commande(CMD_SET_TOLERANCE, NULL, 10.0f));
    sequenceur_enfiler(demo, commande(CMD_MESURER, "I_REPOS_mA", 20.0f));
    sequenceur_enfiler(demo, commande(CMD_POP, NULL, 0.0f));
    sequenceur_enfiler(demo, commande(CMD_MESURER, "V_REF_5V", 5.0f));
    sequenceur_enfiler(demo, commande(CMD_POP, NULL, 0.0f));  // Pile vide.
    sequenceur_enfiler(demo, commande(CMD_RAPPORT, NULL, 0.0f));
    sequenceur_afficher_file(demo);
    sequenceur_executer_tout(demo, jdemo);
    sequenceur_detruire(demo);
    journal_detruire(jdemo);
    printf("** Fin de la demonstration **\n\n");

    UNITY_BEGIN();

    RUN_TEST(test_sequenceur_creer_defaut);
    RUN_TEST(test_sequenceur_detruire_rempli);
    RUN_TEST(test_sequenceur_detruire_null);

    RUN_TEST(test_sequenceur_enfiler_compteur);
    RUN_TEST(test_sequenceur_enfiler_n_execute_pas);
    RUN_TEST(test_sequenceur_enfiler_id_sans_terminateur);

    RUN_TEST(test_sequenceur_set_tension);
    RUN_TEST(test_sequenceur_set_courant_tolerance);
    RUN_TEST(test_sequenceur_ordre_fifo);

    RUN_TEST(test_sequenceur_push_modif_pop_restaure);
    RUN_TEST(test_sequenceur_push_imbrique);
    RUN_TEST(test_sequenceur_pop_pile_vide);

    RUN_TEST(test_sequenceur_mesurer_ajoute_resultat);
    RUN_TEST(test_sequenceur_mesurer_bornes_bruit);
    RUN_TEST(test_sequenceur_mesurer_reproductible);
    RUN_TEST(test_sequenceur_mesurer_journal_null);

    RUN_TEST(test_sequenceur_executer_file_vide);
    RUN_TEST(test_sequenceur_executer_tout);
    RUN_TEST(test_sequenceur_rapport);

    RUN_TEST(test_sequenceur_afficher_file_preserve);
    RUN_TEST(test_sequenceur_afficher_file_vide);

    return UNITY_END();
}
