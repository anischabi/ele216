/**
 * @file filtre_test.c
 * @brief Tests unitaires (Unity) du module filtre (moyenne mobile).
 *
 * Chaque test reçoit un filtre neuf, sans échantillon, avec une fenêtre de 4,
 * créé par setUp() et détruit par tearDown().
 *
 * Les moyennes sont comparées avec une tolérance (TOL) car les float sont
 * approximatifs. Les valeurs des tests ont été choisies pour que les moyennes
 * attendues se calculent facilement à la main (par exemple 2, 4, 6, 8 -> 5).
 */

#include <stdio.h>

#include "filtre.h"
#include "unity.h"

/** Taille de la fenêtre du filtre créé par setUp(). */
#define TAILLE 4            // taille de fenêtre du filtre de test
/** Tolérance utilisée pour comparer des float. */
#define TOL    0.0001f      // tolérance pour comparer des float

/** Filtre utilisé par les tests, recréé avant chaque test par setUp(). */
static filtre_t *f = NULL;

/** Fixture : crée un filtre neuf (fenêtre de 4, aucun échantillon) avant chaque test. */
void setUp(void) {
    f = filtre_creer(TAILLE);
    TEST_ASSERT_NOT_NULL(f);
}

/** Fixture : détruit le filtre après chaque test (aucune fuite mémoire). */
void tearDown(void) {
    filtre_detruire(f);
    f = NULL;
}

/**
 * Vérifie que la valeur du filtre est égale à `attendue`, à la tolérance TOL
 * près (les float sont approximatifs).
 */
static void valeur_attendue(float attendue) {
    TEST_ASSERT_FLOAT_WITHIN(TOL, attendue, filtre_valeur(f));
}

/* ---------- Création valide / invalide ---------- */

/**
 * @brief Un filtre créé avec une taille valide est correctement initialisé.
 *
 * - Scénario : filtre_creer(8).
 * - Attendu  : non NULL, taille de fenêtre 8, 0 échantillon.
 * - Note     : taille différente de celle de setUp(), pour prouver que c'est
 *              bien le paramètre reçu qui est stocké.
 */
void test_filtre_creer_valide(void) {
    filtre_t *g = filtre_creer(8);
    TEST_ASSERT_NOT_NULL(g);
    TEST_ASSERT_EQUAL_INT(8, filtre_taille_fenetre(g));
    TEST_ASSERT_EQUAL_INT(0, filtre_nb_echantillons(g));
    filtre_detruire(g);
}

/**
 * @brief Une taille de fenêtre nulle est refusée.
 *
 * - Scénario : filtre_creer(0).
 * - Attendu  : NULL.
 * - Détecte  : une validation `< 0` au lieu de `<= 0` (avec une taille de 0,
 *              le `% taille_fenetre` de filtre_ajouter ferait une division par
 *              zéro).
 */
void test_filtre_creer_taille_nulle(void) {
    TEST_ASSERT_NULL(filtre_creer(0));
}

/**
 * @brief Une taille de fenêtre négative est refusée.
 *
 * - Scénario : filtre_creer(-3).
 * - Attendu  : NULL.
 */
void test_filtre_creer_taille_negative(void) {
    TEST_ASSERT_NULL(filtre_creer(-3));
}

/**
 * @brief Cas limite : la plus petite taille de fenêtre valide (1) est acceptée.
 *
 * - Scénario : filtre_creer(1).
 * - Attendu  : non NULL, taille de fenêtre 1.
 */
void test_filtre_creer_taille_un(void) {
    filtre_t *g = filtre_creer(1);
    TEST_ASSERT_NOT_NULL(g);
    TEST_ASSERT_EQUAL_INT(1, filtre_taille_fenetre(g));
    filtre_detruire(g);
}

/* ---------- Avant tout échantillon ---------- */

/**
 * @brief Un filtre sans échantillon retourne 0 (et non NaN).
 *
 * - Scénario : filtre neuf, appel direct de filtre_valeur.
 * - Attendu  : valeur 0, nb_echantillons 0.
 * - Détecte  : l'absence de cas particulier (on calculerait 0 / 0, soit NaN).
 */
void test_filtre_valeur_avant_tout_echantillon(void) {
    TEST_ASSERT_EQUAL_FLOAT(0.0f, filtre_valeur(f));
    TEST_ASSERT_EQUAL_INT(0, filtre_nb_echantillons(f));
}

/* ---------- Moyenne partielle (fenêtre pas encore remplie) ---------- */

/**
 * @brief Avec un seul échantillon, la moyenne est cet échantillon.
 *
 * - Scénario : filtre_ajouter(5).
 * - Attendu  : valeur 5, nb_echantillons 1.
 * - Détecte  : une division par la taille de la fenêtre (5 / 4 = 1.25).
 */
void test_filtre_un_seul_echantillon(void) {
    filtre_ajouter(f, 5.0f);
    valeur_attendue(5.0f);
    TEST_ASSERT_EQUAL_INT(1, filtre_nb_echantillons(f));
}

/**
 * @brief Tant que la fenêtre n'est pas remplie, la moyenne porte seulement sur
 *        les échantillons reçus.
 *
 * - Scénario : ajouter 2, puis 4, puis 6 dans une fenêtre de 4, en vérifiant la
 *              valeur après chaque ajout.
 * - Attendu  : 2 (2/1), puis 3 ((2+4)/2), puis 4 ((2+4+6)/3, et non /4) ;
 *              nb_echantillons = 3 à la fin.
 * - Détecte  : une division par la taille de la fenêtre au lieu du nombre
 *              d'échantillons disponibles (moyennes trop basses au démarrage).
 */
void test_filtre_moyenne_partielle(void) {
    filtre_ajouter(f, 2.0f);
    valeur_attendue(2.0f);              // 2 / 1
    filtre_ajouter(f, 4.0f);
    valeur_attendue(3.0f);              // (2 + 4) / 2
    filtre_ajouter(f, 6.0f);
    valeur_attendue(4.0f);              // (2 + 4 + 6) / 3  (et non / 4)
    TEST_ASSERT_EQUAL_INT(3, filtre_nb_echantillons(f));
}

/* ---------- Fenêtre exactement remplie ---------- */

/**
 * @brief Une fenêtre exactement remplie donne la moyenne de tous ses échantillons.
 *
 * - Scénario : ajouter 2, 4, 6, 8 (4 échantillons, fenêtre de 4).
 * - Attendu  : valeur 5 ((2+4+6+8)/4), nb_echantillons 4.
 * - Note     : cas limite entre « partielle » et « débordement » : aucun
 *              échantillon n'a encore été remplacé.
 */
void test_filtre_fenetre_exactement_remplie(void) {
    filtre_ajouter(f, 2.0f);
    filtre_ajouter(f, 4.0f);
    filtre_ajouter(f, 6.0f);
    filtre_ajouter(f, 8.0f);
    valeur_attendue(5.0f);              // (2 + 4 + 6 + 8) / 4
    TEST_ASSERT_EQUAL_INT(4, filtre_nb_echantillons(f));
}

/* ---------- Débordement : le plus ancien est remplacé ---------- */

/**
 * @brief Quand la fenêtre est pleine, c'est bien le plus ancien échantillon qui est remplacé.
 *
 * - Scénario : ajouter 1, 2, 3, 4, puis 100, puis 200.
 * - Attendu  : 2.5 après le 4 ; 27.25 après le 100 (fenêtre 2, 3, 4, 100 : le 1
 *              est remplacé) ; 76.75 après le 200 (fenêtre 3, 4, 100, 200 : le
 *              2 est remplacé) ; nb_echantillons reste à 4.
 * - Détecte  : le remplacement du mauvais échantillon (ex. remplacer le 4 au
 *              lieu du 1 donnerait 26.5 au lieu de 27.25). Les valeurs sont
 *              toutes différentes pour que chaque erreur donne un résultat
 *              différent.
 * - Trace    : QUESTIONS.md, section « Aide-mémoire ».
 */
void test_filtre_debordement_remplace_le_plus_ancien(void) {
    filtre_ajouter(f, 1.0f);
    filtre_ajouter(f, 2.0f);
    filtre_ajouter(f, 3.0f);
    filtre_ajouter(f, 4.0f);
    valeur_attendue(2.5f);              // (1 + 2 + 3 + 4) / 4

    filtre_ajouter(f, 100.0f);          // remplace le 1
    valeur_attendue(27.25f);            // (2 + 3 + 4 + 100) / 4

    filtre_ajouter(f, 200.0f);          // remplace le 2
    valeur_attendue(76.75f);            // (3 + 4 + 100 + 200) / 4

    TEST_ASSERT_EQUAL_INT(4, filtre_nb_echantillons(f));    // reste à 4
}

/**
 * @brief Le remplacement continue de fonctionner après plusieurs tours du tableau interne.
 *
 * - Scénario : ajouter les valeurs 1, 2, ..., 12 dans une fenêtre de 4 (3 tours
 *              complets du tableau).
 * - Attendu  : la fenêtre contient 9, 10, 11, 12, donc valeur 10.5 ; nb_echantillons 4.
 * - Détecte  : un remplacement qui fonctionne au premier tour mais pas ensuite
 *              (par exemple toujours la case 0, ou un index qui dépasse le tableau).
 */
void test_filtre_debordement_plusieurs_tours(void) {
    for (int i = 1; i <= 12; i++) {
        filtre_ajouter(f, (float)i);
    }
    valeur_attendue(10.5f);             // fenêtre = 9, 10, 11, 12
    TEST_ASSERT_EQUAL_INT(4, filtre_nb_echantillons(f));
}

/* ---------- Signal constant ---------- */

/**
 * @brief Un signal constant donne toujours cette constante, quel que soit l'état de la fenêtre.
 *
 * - Scénario : ajouter 3.3 deux fois (fenêtre partielle), puis 18 fois de plus
 *              (fenêtre pleine et débordée, 20 échantillons au total).
 * - Attendu  : valeur 3.3 (à la précision flottante près) aux deux moments.
 * - Note     : c'est une propriété de la moyenne : elle se vérifie sans avoir
 *              à calculer une valeur attendue à la main.
 * - Détecte  : une division par la mauvaise quantité (partielle) ou des
 *              cases non initialisées prises en compte.
 */
void test_filtre_signal_constant(void) {
    filtre_ajouter(f, 3.3f);
    filtre_ajouter(f, 3.3f);
    valeur_attendue(3.3f);              // fenêtre partielle

    for (int i = 0; i < 18; i++) {
        filtre_ajouter(f, 3.3f);        // 20 échantillons au total
    }
    valeur_attendue(3.3f);              // fenêtre pleine et débordée
}

/* ---------- Autres cas ---------- */

/**
 * @brief Le filtre gère correctement les valeurs négatives.
 *
 * - Scénario : ajouter -4 et -2, puis 2 et 4.
 * - Attendu  : -3 après les deux premières valeurs ; 0 après les quatre
 *              ((-4-2+2+4)/4).
 */
void test_filtre_valeurs_negatives(void) {
    filtre_ajouter(f, -4.0f);
    filtre_ajouter(f, -2.0f);
    valeur_attendue(-3.0f);
    filtre_ajouter(f, 2.0f);
    filtre_ajouter(f, 4.0f);
    valeur_attendue(0.0f);              // (-4 - 2 + 2 + 4) / 4
}

/**
 * @brief nb_echantillons augmente jusqu'à la taille de la fenêtre puis reste plafonné.
 *
 * - Scénario : ajouter 10 échantillons dans une fenêtre de 4, en vérifiant
 *              nb_echantillons après chaque ajout.
 * - Attendu  : 1, 2, 3, 4, puis 4, 4, 4, 4, 4, 4 (jamais plus que 4).
 * - Détecte  : un compteur qui continue de croître (l'énoncé demande « saturé
 *              à la taille »).
 */
void test_filtre_nb_echantillons_est_plafonne(void) {
    for (int i = 0; i < 10; i++) {
        filtre_ajouter(f, (float)i);
        int attendu = (i + 1 < TAILLE) ? i + 1 : TAILLE;
        TEST_ASSERT_EQUAL_INT(attendu, filtre_nb_echantillons(f));
    }
}

/**
 * @brief Cas limite : avec une fenêtre de 1, le filtre suit le dernier échantillon.
 *
 * - Scénario : filtre_creer(1), ajouter 5 puis 9.
 * - Attendu  : valeur 5 puis 9 (aucun filtrage : la moyenne d'un seul élément
 *              est cet élément) ; nb_echantillons reste à 1.
 * - Note     : (index + 1) % 1 vaut toujours 0, donc chaque ajout écrase la
 *              seule case du tableau.
 */
void test_filtre_taille_un_suit_le_dernier_echantillon(void) {
    filtre_t *g = filtre_creer(1);
    TEST_ASSERT_NOT_NULL(g);

    filtre_ajouter(g, 5.0f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 5.0f, filtre_valeur(g));
    filtre_ajouter(g, 9.0f);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 9.0f, filtre_valeur(g));
    TEST_ASSERT_EQUAL_INT(1, filtre_nb_echantillons(g));

    filtre_detruire(g);
}

/**
 * @brief La taille de la fenêtre ne change jamais, même après beaucoup d'ajouts.
 *
 * - Scénario : ajouter 10 échantillons dans une fenêtre de 4.
 * - Attendu  : filtre_taille_fenetre vaut toujours 4.
 */
void test_filtre_taille_fenetre_immuable(void) {
    for (int i = 0; i < 10; i++) {
        filtre_ajouter(f, (float)i);
    }
    TEST_ASSERT_EQUAL_INT(TAILLE, filtre_taille_fenetre(f));
}

/**
 * @brief Détruire un pointeur NULL est permis et sans effet.
 *
 * - Scénario : filtre_detruire(NULL).
 * - Attendu  : aucun plantage (le test réussit simplement s'il arrive au bout).
 */
void test_filtre_detruire_null(void) {
    filtre_detruire(NULL);      // ne doit pas planter
    TEST_PASS();
}

/**
 * @brief Point d'entrée : démonstration de filtre_afficher, puis exécution des tests.
 *
 * La démonstration affiche un filtre de fenêtre 3 après 0, 2, 3 puis 4
 * échantillons (2, 4, 6, 8) : au 4e ajout, le 2 est remplacé et l'affichage
 * montre bien [4, 6, 8] (du plus ancien au plus récent). filtre_afficher ne se
 * prête pas à une vérification automatique, on la regarde à l'œil.
 */
int main(void) {
    printf("** Demonstration de filtre_afficher **\n");
    filtre_t *demo = filtre_creer(3);
    filtre_afficher(demo);                  // vide
    filtre_ajouter(demo, 2.0f);
    filtre_ajouter(demo, 4.0f);
    filtre_afficher(demo);                  // 2, 4 : moyenne 3
    filtre_ajouter(demo, 6.0f);
    filtre_afficher(demo);                  // 2, 4, 6 : moyenne 4
    filtre_ajouter(demo, 8.0f);
    filtre_afficher(demo);                  // 4, 6, 8 : moyenne 6 (le 2 est remplace)
    filtre_detruire(demo);
    printf("** Fin de la demonstration **\n\n");

    UNITY_BEGIN();

    RUN_TEST(test_filtre_creer_valide);
    RUN_TEST(test_filtre_creer_taille_nulle);
    RUN_TEST(test_filtre_creer_taille_negative);
    RUN_TEST(test_filtre_creer_taille_un);

    RUN_TEST(test_filtre_valeur_avant_tout_echantillon);

    RUN_TEST(test_filtre_un_seul_echantillon);
    RUN_TEST(test_filtre_moyenne_partielle);

    RUN_TEST(test_filtre_fenetre_exactement_remplie);

    RUN_TEST(test_filtre_debordement_remplace_le_plus_ancien);
    RUN_TEST(test_filtre_debordement_plusieurs_tours);

    RUN_TEST(test_filtre_signal_constant);

    RUN_TEST(test_filtre_valeurs_negatives);
    RUN_TEST(test_filtre_nb_echantillons_est_plafonne);
    RUN_TEST(test_filtre_taille_un_suit_le_dernier_echantillon);
    RUN_TEST(test_filtre_taille_fenetre_immuable);
    RUN_TEST(test_filtre_detruire_null);

    return UNITY_END();
}
