/**
 * @file journal_test.c
 * @brief Tests unitaires (Unity) du module journal.
 *
 * Chaque test reçoit un journal vide créé par setUp() et détruit par
 * tearDown(), donc les tests sont indépendants. Les valeurs utilisées
 * (100, 5 %...) sont exactement représentables en float : le verdict de
 * chaque ajout est connu sans ambiguïté. Les tests suivent l'ordre de
 * l'énoncé, plus quelques cas bonus.
 */

#include <stdio.h>
#include <string.h>

#include "journal.h"
#include "unity.h"

/** Précision des comparaisons de float. */
#define EPSILON 1e-6f

/** Fichier temporaire écrit par les tests de journal_sauvegarder. */
#define FICHIER_RAPPORT "journal_test_rapport.txt"

/** Journal utilisé par les tests, recréé avant chaque test par setUp(). */
static journal_t *j = NULL;

/** Fixture : crée un journal vide avant chaque test. */
void setUp(void) {
    j = journal_creer();
    TEST_ASSERT_NOT_NULL(j);
}

/** Fixture : détruit le journal (et ses résultats) après chaque test. */
void tearDown(void) {
    journal_detruire(j);
    j = NULL;
}

/** Ajoute un résultat PASS (attendu 100, mesure 101, tolérance 5 %). */
static void ajouter_pass(journal_t *jr, const char *id) {
    journal_ajouter(jr, id, 100.0f, 101.0f, 5.0f);
}

/** Ajoute un résultat FAIL (attendu 100, mesure 110, tolérance 5 %). */
static void ajouter_fail(journal_t *jr, const char *id) {
    journal_ajouter(jr, id, 100.0f, 110.0f, 5.0f);
}

/* ---------- Création et destruction ---------- */

/**
 * @brief Un journal neuf est vide.
 *
 * - Scénario : journal_creer() (fait par setUp()).
 * - Attendu  : 0 résultat, 0 réussi, 0 échoué.
 */
void test_journal_creer_vide(void) {
    TEST_ASSERT_EQUAL_INT(0, journal_nb_resultats(j));
    TEST_ASSERT_EQUAL_INT(0, journal_nb_reussis(j));
    TEST_ASSERT_EQUAL_INT(0, journal_nb_echoues(j));
}

/**
 * @brief Création puis destruction d'un journal rempli.
 *
 * - Scénario : créer un 2e journal, y ajouter 10 résultats (plus que la
 *              capacité initiale du Vecteur), le détruire.
 * - Attendu  : pas de plantage. Unity ne détecte pas les fuites : pour
 *              vérifier « sans fuite », lancer sous valgrind (Linux) ou
 *              Dr. Memory (Windows) ; ce test fournit le scénario.
 * - Détecte  : un journal_detruire qui libère les résultats après le
 *              Vecteur (lecture de mémoire libérée → plantage probable).
 */
void test_journal_creer_detruire_rempli(void) {
    journal_t *j2 = journal_creer();
    TEST_ASSERT_NOT_NULL(j2);
    for (int i = 0; i < 10; i++) {
        ajouter_pass(j2, "T");
    }
    TEST_ASSERT_EQUAL_INT(10, journal_nb_resultats(j2));
    journal_detruire(j2);
}

/**
 * @brief Détruire NULL n'a pas d'effet.
 *
 * - Scénario : journal_detruire(NULL).
 * - Attendu  : pas de plantage.
 * - Détecte  : un appel à vecteur_taille(NULL->resultats) (le
 *              vecteur_liberer du cours fait un assert sur NULL).
 */
void test_journal_detruire_null(void) {
    journal_detruire(NULL);
    TEST_PASS();
}

/* ---------- Ajout et compteurs ---------- */

/**
 * @brief Chaque ajout incrémente le total, et le bon compteur.
 *
 * - Scénario : ajouter 3 résultats PASS.
 * - Attendu  : total 3, réussis 3, échoués 0.
 */
void test_journal_ajouter_compteurs(void) {
    ajouter_pass(j, "A");
    ajouter_pass(j, "B");
    ajouter_pass(j, "C");
    TEST_ASSERT_EQUAL_INT(3, journal_nb_resultats(j));
    TEST_ASSERT_EQUAL_INT(3, journal_nb_reussis(j));
    TEST_ASSERT_EQUAL_INT(0, journal_nb_echoues(j));
}

/**
 * @brief Un ajout avec un id NULL est ignoré.
 *
 * - Scénario : un ajout valide, puis journal_ajouter(j, NULL, ...).
 * - Attendu  : total toujours 1, pas de plantage.
 * - Détecte  : un appel à vecteur_ajouter avec un pointeur NULL (assert du
 *              Vecteur) ou un compteur incrémenté malgré l'échec.
 * - Note     : cas non exigé par l'énoncé (bonus).
 */
void test_journal_ajouter_id_null_ignore(void) {
    ajouter_pass(j, "A");
    journal_ajouter(j, NULL, 1.0f, 1.0f, 5.0f);
    TEST_ASSERT_EQUAL_INT(1, journal_nb_resultats(j));
}

/* ---------- Obtenir par index ---------- */

/**
 * @brief Obtenir par un index valide retourne le bon résultat.
 *
 * - Scénario : ajouter A (PASS), B (FAIL), C, puis obtenir l'index 1.
 * - Attendu  : le résultat B avec ses 4 champs et le verdict FAIL.
 * - Détecte  : une erreur de décalage d'index (retourner i + 1 ou i - 1).
 */
void test_journal_obtenir_index_valide(void) {
    ajouter_pass(j, "A");
    journal_ajouter(j, "B", 3.3f, 3.0f, 2.0f);  // écart 0.3 > 0.066 : FAIL
    ajouter_pass(j, "C");

    const resultat_t *r = journal_obtenir(j, 1);
    TEST_ASSERT_NOT_NULL(r);
    TEST_ASSERT_EQUAL_STRING("B", resultat_id(r));
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 3.3f, resultat_attendu(r));
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 3.0f, resultat_mesure(r));
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 2.0f, resultat_tolerance(r));
    TEST_ASSERT_EQUAL_INT(0, resultat_est_reussi(r));
}

/**
 * @brief L'ordre d'ajout est conservé, même après agrandissement.
 *
 * - Scénario : ajouter T0 à T9 (le Vecteur doit doubler au moins une fois),
 *              puis obtenir chaque index.
 * - Attendu  : journal_obtenir(j, i) a l'id "Ti".
 * - Détecte  : des résultats perdus ou écrasés lors du realloc du Vecteur,
 *              ou un id stocké par pointeur (tous les ids vaudraient "T9"
 *              puisque le tampon est réutilisé).
 */
void test_journal_obtenir_ordre_apres_agrandissement(void) {
    char id[8];
    for (int i = 0; i < 10; i++) {
        snprintf(id, sizeof(id), "T%d", i);
        ajouter_pass(j, id);
    }
    TEST_ASSERT_EQUAL_INT(10, journal_nb_resultats(j));
    for (int i = 0; i < 10; i++) {
        snprintf(id, sizeof(id), "T%d", i);
        TEST_ASSERT_EQUAL_STRING(id, resultat_id(journal_obtenir(j, i)));
    }
}

/**
 * @brief Obtenir par un index invalide retourne NULL.
 *
 * - Scénario : 2 résultats, puis index -1, 2 (= taille) et 100.
 * - Attendu  : NULL dans les 3 cas (pas d'arrêt par assert).
 * - Détecte  : la condition `index > taille` au lieu de `>=` (l'index 2
 *              lirait hors du tableau), ou l'oubli de `index < 0`.
 */
void test_journal_obtenir_index_invalide(void) {
    ajouter_pass(j, "A");
    ajouter_pass(j, "B");
    TEST_ASSERT_NULL(journal_obtenir(j, -1));
    TEST_ASSERT_NULL(journal_obtenir(j, 2));
    TEST_ASSERT_NULL(journal_obtenir(j, 100));
}

/**
 * @brief Obtenir dans un journal vide retourne NULL.
 *
 * - Scénario : journal vide, index 0.
 * - Attendu  : NULL.
 */
void test_journal_obtenir_journal_vide(void) {
    TEST_ASSERT_NULL(journal_obtenir(j, 0));
}

/* ---------- Mélange PASS / FAIL ---------- */

/**
 * @brief Les compteurs restent corrects avec un mélange de verdicts.
 *
 * - Scénario : P F P F F P P (4 PASS, 3 FAIL), ajoutés en alternance.
 * - Attendu  : total 7, réussis 4, échoués 3, réussis + échoués = total.
 * - Détecte  : un compteur qui compte tout (retourne le total) ou qui
 *              inverse PASS et FAIL.
 */
void test_journal_melange_pass_fail(void) {
    ajouter_pass(j, "P1");
    ajouter_fail(j, "F1");
    ajouter_pass(j, "P2");
    ajouter_fail(j, "F2");
    ajouter_fail(j, "F3");
    ajouter_pass(j, "P3");
    ajouter_pass(j, "P4");
    TEST_ASSERT_EQUAL_INT(7, journal_nb_resultats(j));
    TEST_ASSERT_EQUAL_INT(4, journal_nb_reussis(j));
    TEST_ASSERT_EQUAL_INT(3, journal_nb_echoues(j));
    TEST_ASSERT_EQUAL_INT(journal_nb_resultats(j),
                          journal_nb_reussis(j) + journal_nb_echoues(j));
}

/**
 * @brief Seulement des FAIL : aucun réussi.
 *
 * - Scénario : 3 résultats FAIL.
 * - Attendu  : réussis 0, échoués 3.
 */
void test_journal_seulement_fail(void) {
    ajouter_fail(j, "F1");
    ajouter_fail(j, "F2");
    ajouter_fail(j, "F3");
    TEST_ASSERT_EQUAL_INT(0, journal_nb_reussis(j));
    TEST_ASSERT_EQUAL_INT(3, journal_nb_echoues(j));
}

/* ---------- Sauvegarde (bonus) ---------- */

/**
 * @brief Le rapport sauvegardé contient les résultats et le résumé.
 *
 * - Scénario : ajouter ALPHA (PASS), BETA (FAIL), GAMMA (PASS), sauvegarder
 *              dans un fichier, le relire ligne par ligne.
 * - Attendu  : les 3 ids apparaissent, la ligne de résumé indique
 *              Total 3, Reussis 2, Echoues 1. Le fichier est ensuite effacé.
 * - Détecte  : un journal_sauvegarder qui écrit sur stdout au lieu du
 *              fichier, ou qui oublie fclose (contenu non vidé sur disque).
 * - Note     : cas non exigé par l'énoncé (bonus).
 */
void test_journal_sauvegarder_contenu(void) {
    ajouter_pass(j, "ALPHA");
    ajouter_fail(j, "BETA");
    ajouter_pass(j, "GAMMA");
    journal_sauvegarder(j, FICHIER_RAPPORT);

    FILE *f = fopen(FICHIER_RAPPORT, "r");
    TEST_ASSERT_NOT_NULL_MESSAGE(f, "le fichier n'a pas ete cree");

    int vu_alpha = 0, vu_beta = 0, vu_gamma = 0, vu_resume = 0;
    char ligne[256];
    while (fgets(ligne, sizeof(ligne), f) != NULL) {
        if (strstr(ligne, "ALPHA") != NULL) { vu_alpha = 1; }
        if (strstr(ligne, "BETA") != NULL) { vu_beta = 1; }
        if (strstr(ligne, "GAMMA") != NULL) { vu_gamma = 1; }
        if (strstr(ligne, "Total : 3   Reussis : 2   Echoues : 1") != NULL) {
            vu_resume = 1;
        }
    }
    fclose(f);
    remove(FICHIER_RAPPORT);

    TEST_ASSERT_TRUE(vu_alpha);
    TEST_ASSERT_TRUE(vu_beta);
    TEST_ASSERT_TRUE(vu_gamma);
    TEST_ASSERT_TRUE(vu_resume);
}

/**
 * @brief Un chemin impossible à ouvrir ne fait pas planter le programme.
 *
 * - Scénario : sauvegarder dans un dossier qui n'existe pas.
 * - Attendu  : message sur stderr, pas de plantage.
 * - Détecte  : un fprintf sur un FILE* NULL (oubli de vérifier fopen).
 * - Note     : cas non exigé par l'énoncé (bonus).
 */
void test_journal_sauvegarder_chemin_invalide(void) {
    ajouter_pass(j, "A");
    journal_sauvegarder(j, "dossier_inexistant/rapport.txt");
    TEST_PASS();
}

int main(void) {
    printf("** Demonstration de journal_afficher **\n");
    journal_t *demo = journal_creer();
    journal_ajouter(demo, "V_REF_3V3", 3.3f, 3.35f, 5.0f);
    journal_ajouter(demo, "V_REF_5V", 5.0f, 5.40f, 5.0f);
    journal_ajouter(demo, "I_REPOS_mA", 20.0f, 21.0f, 10.0f);
    journal_ajouter(demo, "V_NEG_12", -12.0f, -12.4f, 5.0f);
    journal_ajouter(demo, "OFFSET_mV", 0.0f, 0.2f, 5.0f);
    journal_afficher(demo);
    journal_detruire(demo);
    printf("** Fin de la demonstration **\n\n");

    UNITY_BEGIN();

    RUN_TEST(test_journal_creer_vide);
    RUN_TEST(test_journal_creer_detruire_rempli);
    RUN_TEST(test_journal_detruire_null);

    RUN_TEST(test_journal_ajouter_compteurs);
    RUN_TEST(test_journal_ajouter_id_null_ignore);

    RUN_TEST(test_journal_obtenir_index_valide);
    RUN_TEST(test_journal_obtenir_ordre_apres_agrandissement);
    RUN_TEST(test_journal_obtenir_index_invalide);
    RUN_TEST(test_journal_obtenir_journal_vide);

    RUN_TEST(test_journal_melange_pass_fail);
    RUN_TEST(test_journal_seulement_fail);

    RUN_TEST(test_journal_sauvegarder_contenu);
    RUN_TEST(test_journal_sauvegarder_chemin_invalide);

    return UNITY_END();
}
