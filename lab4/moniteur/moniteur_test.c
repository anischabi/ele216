/**
 * @file moniteur_test.c
 * @brief Tests unitaires (Unity) et simulation du moniteur de batterie.
 *
 * Le main exécute d'abord la simulation demandée par l'énoncé (tests
 * manuels), puis les tests unitaires.
 *
 * Chaque test reçoit un moniteur neuf créé par setUp() (pile 1000 mAh,
 * 3.7 V, historique de 5 lectures, filtre de fenêtre 4, seuil d'alerte 25 %)
 * et détruit par tearDown(), donc les tests sont indépendants.
 *
 * Les charges utilisées (125, 250, 375, 500... mAh sur 1000) donnent des
 * pourcentages exactement représentables en float (12.5, 25, 37.5, 50...),
 * et les moyennes de ces valeurs le sont aussi : on peut donc tester la
 * frontière exacte du seuil d'alerte sans erreur d'arrondi.
 */

#include <stdio.h>

#include "moniteur.h"
#include "unity.h"

/** Capacité (mAh) de la pile du moniteur créé par setUp(). */
#define CAPACITE    1000.0f
/** Tension (V) de la pile du moniteur créé par setUp(). */
#define TENSION     3.7f
/** Nombre de lectures conservées dans l'historique du moniteur de setUp(). */
#define HISTORIQUE  5
/** Taille de la fenêtre du filtre du moniteur de setUp(). */
#define FENETRE     4
/** Seuil d'alerte (%) du moniteur créé par setUp(). */
#define SEUIL       25.0f

/** Moniteur utilisé par les tests, recréé avant chaque test par setUp(). */
static moniteur_t *m = NULL;

/** Fixture : crée un moniteur neuf (pile déchargée, historique et filtre vides). */
void setUp(void) {
    m = moniteur_creer(CAPACITE, TENSION, HISTORIQUE, FENETRE, SEUIL);
    TEST_ASSERT_NOT_NULL(m);
}

/** Fixture : détruit le moniteur et ses sous-modules après chaque test. */
void tearDown(void) {
    moniteur_detruire(m);
    m = NULL;
}

/** Appelle moniteur_enregistrer n fois sur le moniteur mon. */
static void enregistrer_n(moniteur_t *mon, int n) {
    for (int i = 0; i < n; i++) {
        moniteur_enregistrer(mon);
    }
}

/* ---------- Création valide / invalide ---------- */

/**
 * @brief Un moniteur créé avec des paramètres valides est correctement initialisé.
 *
 * - Scénario : moniteur_creer(2000, 12, 3, 2, 15).
 * - Attendu  : pointeur non NULL, seuil 15, pile déchargée (charge brute 0),
 *              historique vide.
 */
void test_moniteur_creer_valide(void) {
    moniteur_t *m1 = moniteur_creer(2000.0f, 12.0f, 3, 2, 15.0f);
    TEST_ASSERT_NOT_NULL(m1);
    TEST_ASSERT_EQUAL_FLOAT(15.0f, moniteur_seuil_alerte(m1));
    TEST_ASSERT_EQUAL_FLOAT(0.0f, moniteur_charge_brute(m1));
    TEST_ASSERT_EQUAL_INT(0, moniteur_nb_lectures(m1));
    moniteur_detruire(m1);
}

/**
 * @brief Un paramètre de pile invalide fait échouer la création.
 *
 * - Scénario : capacité 0, puis tension -1.
 * - Attendu  : NULL (pile_creer échoue en premier : rien d'autre à libérer
 *              sauf la structure du moniteur).
 */
void test_moniteur_creer_pile_invalide(void) {
    TEST_ASSERT_NULL(moniteur_creer(0.0f, TENSION, HISTORIQUE, FENETRE, SEUIL));
    TEST_ASSERT_NULL(moniteur_creer(CAPACITE, -1.0f, HISTORIQUE, FENETRE, SEUIL));
}

/**
 * @brief Une taille d'historique invalide fait échouer la création.
 *
 * - Scénario : taille_historique 0.
 * - Attendu  : NULL. La pile, déjà créée, doit être libérée (chemin de
 *              nettoyage partiel ; une fuite n'est pas visible par Unity,
 *              mais le test exécute ce chemin).
 */
void test_moniteur_creer_historique_invalide(void) {
    TEST_ASSERT_NULL(moniteur_creer(CAPACITE, TENSION, 0, FENETRE, SEUIL));
}

/**
 * @brief Une taille de filtre invalide fait échouer la création.
 *
 * - Scénario : taille_filtre -2.
 * - Attendu  : NULL. La pile et le buffer, déjà créés, doivent être libérés.
 */
void test_moniteur_creer_filtre_invalide(void) {
    TEST_ASSERT_NULL(moniteur_creer(CAPACITE, TENSION, HISTORIQUE, -2, SEUIL));
}

/**
 * @brief Un seuil d'alerte hors de [0, 100] est refusé.
 *
 * - Scénario : seuil -5, puis 150.
 * - Attendu  : NULL dans les deux cas.
 * - Note     : validation hors énoncé (l'énoncé ne parle que des échecs
 *              d'allocation), à ne pas exiger des étudiant.e.s.
 */
void test_moniteur_creer_seuil_invalide(void) {
    TEST_ASSERT_NULL(moniteur_creer(CAPACITE, TENSION, HISTORIQUE, FENETRE, -5.0f));
    TEST_ASSERT_NULL(moniteur_creer(CAPACITE, TENSION, HISTORIQUE, FENETRE, 150.0f));
}

/**
 * @brief moniteur_detruire(NULL) n'a pas d'effet.
 *
 * - Scénario : moniteur_detruire(NULL).
 * - Attendu  : aucun plantage (le test se termine).
 */
void test_moniteur_detruire_null(void) {
    moniteur_detruire(NULL);
}

/* ---------- Charges et décharges : charge brute ---------- */

/**
 * @brief Une séquence de charges et décharges se reflète dans la charge brute.
 *
 * - Scénario : charger 500 (50 %), décharger 125 (37.5 %), charger 625
 *              (100 %), décharger 750 (25 %).
 * - Attendu  : la charge brute suit chaque étape.
 */
void test_moniteur_sequence_charge_decharge(void) {
    moniteur_charger(m, 500.0f);
    TEST_ASSERT_EQUAL_FLOAT(50.0f, moniteur_charge_brute(m));
    moniteur_decharger(m, 125.0f);
    TEST_ASSERT_EQUAL_FLOAT(37.5f, moniteur_charge_brute(m));
    moniteur_charger(m, 625.0f);
    TEST_ASSERT_EQUAL_FLOAT(100.0f, moniteur_charge_brute(m));
    moniteur_decharger(m, 750.0f);
    TEST_ASSERT_EQUAL_FLOAT(25.0f, moniteur_charge_brute(m));
}

/**
 * @brief La saturation de la pile est visible dans la charge brute.
 *
 * - Scénario : charger 1500 (plus que la capacité), puis décharger 2000.
 * - Attendu  : 100 %, puis 0 %.
 */
void test_moniteur_charge_brute_saturee(void) {
    moniteur_charger(m, 1500.0f);
    TEST_ASSERT_EQUAL_FLOAT(100.0f, moniteur_charge_brute(m));
    moniteur_decharger(m, 2000.0f);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, moniteur_charge_brute(m));
}

/**
 * @brief Charger et décharger ne modifient ni l'historique ni le filtre.
 *
 * - Scénario : charger 500 et enregistrer (lissée 50), puis décharger 250
 *              sans enregistrer.
 * - Attendu  : charge brute 25, mais charge lissée toujours 50 et 1 seule
 *              lecture dans l'historique.
 * - Détecte  : un moniteur_decharger qui enregistre aussi une lecture.
 */
void test_moniteur_charger_ne_modifie_pas_filtre(void) {
    moniteur_charger(m, 500.0f);
    moniteur_enregistrer(m);
    moniteur_decharger(m, 250.0f);
    TEST_ASSERT_EQUAL_FLOAT(25.0f, moniteur_charge_brute(m));
    TEST_ASSERT_EQUAL_FLOAT(50.0f, moniteur_charge_lissee(m));
    TEST_ASSERT_EQUAL_INT(1, moniteur_nb_lectures(m));
}

/* ---------- Historique ---------- */

/**
 * @brief L'historique compte les lectures et plafonne à sa capacité.
 *
 * - Scénario : 3 enregistrements, puis 5 de plus (8 au total, capacité 5).
 * - Attendu  : 3 lectures, puis 5.
 * - Note     : le contenu (les 5 plus récentes) se vérifie visuellement avec
 *              moniteur_afficher dans la simulation : le buffer du labo 3 n'a
 *              pas de fonction pour consulter sans retirer.
 */
void test_moniteur_historique_plafonne(void) {
    moniteur_charger(m, 500.0f);
    enregistrer_n(m, 3);
    TEST_ASSERT_EQUAL_INT(3, moniteur_nb_lectures(m));
    enregistrer_n(m, 5);
    TEST_ASSERT_EQUAL_INT(HISTORIQUE, moniteur_nb_lectures(m));
}

/* ---------- Convergence de la charge lissée ---------- */

/**
 * @brief La charge lissée converge vers la charge brute après assez de
 * lectures stables.
 *
 * - Scénario : charger 1000 (100 %) et enregistrer, puis décharger 500
 *              (50 %) et enregistrer 4 fois (fenêtre 4).
 * - Trace    : lissée après chaque lecture à 50 % :
 *              (100+50)/2 = 75, (100+50+50)/3 = 66.67, (100+150)/4 = 62.5,
 *              puis (4 * 50)/4 = 50 : la lecture à 100 % est sortie de la
 *              fenêtre.
 * - Attendu  : 62.5 après 3 lectures stables (pas encore convergé), puis
 *              exactement 50 = charge brute après 4.
 */
void test_moniteur_lissee_converge(void) {
    moniteur_charger(m, 1000.0f);
    moniteur_enregistrer(m);
    moniteur_decharger(m, 500.0f);
    enregistrer_n(m, FENETRE - 1);
    TEST_ASSERT_EQUAL_FLOAT(62.5f, moniteur_charge_lissee(m));

    moniteur_enregistrer(m);
    TEST_ASSERT_EQUAL_FLOAT(moniteur_charge_brute(m), moniteur_charge_lissee(m));
    TEST_ASSERT_EQUAL_FLOAT(50.0f, moniteur_charge_lissee(m));
}

/* ---------- Alerte ---------- */

/**
 * @brief L'alerte se déclenche au franchissement du seuil, avec le retard
 * dû au lissage.
 *
 * - Scénario : seuil 25 %. 4 lectures à 37.5 %, puis décharge à 12.5 % et
 *              3 lectures.
 * - Trace    : lissée = 37.5 → (3*37.5 + 12.5)/4 = 31.25 → (2*37.5 + 2*12.5)/4
 *              = 25 → (37.5 + 3*12.5)/4 = 18.75.
 * - Attendu  : pas d'alerte à 31.25 (alors que la charge brute est déjà sous
 *              le seuil), pas d'alerte à exactement 25 (inférieur strict),
 *              alerte à 18.75.
 * - Détecte  : une alerte basée sur la charge brute au lieu de la lissée, ou
 *              une comparaison `<=` au lieu de `<`.
 */
void test_moniteur_alerte_franchissement(void) {
    moniteur_charger(m, 375.0f);
    enregistrer_n(m, FENETRE);
    TEST_ASSERT_EQUAL_INT(0, moniteur_alerte(m));

    moniteur_decharger(m, 250.0f);
    moniteur_enregistrer(m);
    TEST_ASSERT_EQUAL_FLOAT(31.25f, moniteur_charge_lissee(m));
    TEST_ASSERT_EQUAL_INT(0, moniteur_alerte(m));

    moniteur_enregistrer(m);
    TEST_ASSERT_EQUAL_FLOAT(SEUIL, moniteur_charge_lissee(m));
    TEST_ASSERT_EQUAL_INT(0, moniteur_alerte(m));

    moniteur_enregistrer(m);
    TEST_ASSERT_EQUAL_FLOAT(18.75f, moniteur_charge_lissee(m));
    TEST_ASSERT_EQUAL_INT(1, moniteur_alerte(m));
}

/**
 * @brief L'alerte se désactive après une recharge suffisante.
 *
 * - Scénario : 4 lectures à 12.5 % (alerte), puis recharge à 100 % et une
 *              lecture.
 * - Trace    : lissée = 12.5 → (3*12.5 + 100)/4 = 34.375.
 * - Attendu  : alerte 1, puis 0.
 */
void test_moniteur_alerte_desactivee_apres_recharge(void) {
    moniteur_charger(m, 125.0f);
    enregistrer_n(m, FENETRE);
    TEST_ASSERT_EQUAL_INT(1, moniteur_alerte(m));

    moniteur_charger(m, 875.0f);
    moniteur_enregistrer(m);
    TEST_ASSERT_EQUAL_FLOAT(34.375f, moniteur_charge_lissee(m));
    TEST_ASSERT_EQUAL_INT(0, moniteur_alerte(m));
}

/**
 * @brief Une recharge sans enregistrement ne désactive pas l'alerte.
 *
 * - Scénario : 4 lectures à 12.5 % (alerte), puis recharge à 100 % sans
 *              enregistrer.
 * - Attendu  : alerte toujours active (le filtre n'a pas changé).
 */
void test_moniteur_alerte_recharge_sans_enregistrer(void) {
    moniteur_charger(m, 125.0f);
    enregistrer_n(m, FENETRE);
    moniteur_charger(m, 875.0f);
    TEST_ASSERT_EQUAL_INT(1, moniteur_alerte(m));
}

/**
 * @brief Avant tout enregistrement, la charge lissée vaut 0 et l'alerte est
 * active.
 *
 * - Scénario : moniteur neuf, pile chargée à 100 % sans enregistrer.
 * - Attendu  : charge lissée 0, alerte 1 (comportement documenté dans
 *              moniteur.h : conséquence directe de l'énoncé).
 */
void test_moniteur_alerte_avant_enregistrement(void) {
    moniteur_charger(m, 1000.0f);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, moniteur_charge_lissee(m));
    TEST_ASSERT_EQUAL_INT(1, moniteur_alerte(m));
}

/* ---------- Simulation (tests manuels) ---------- */

/**
 * Affiche une ligne de lecture : charge brute, charge lissée et alerte.
 *
 * @param mon   pointeur non-nul vers le moniteur.
 * @param etape numéro de l'étape de la simulation.
 */
static void afficher_lecture(const moniteur_t *mon, int etape) {
    printf("Etape %2d | brute = %6.2f %% | lissee = %6.2f %% | alerte = %s\n",
           etape, moniteur_charge_brute(mon), moniteur_charge_lissee(mon),
           moniteur_alerte(mon) ? "OUI" : "non");
}

/**
 * Simulation demandée par l'énoncé : pile 5400 mAh, 11.7 V, historique de 10
 * lectures, filtre de fenêtre 5, seuil d'alerte 20 %. Charge complète, puis
 * 20 cycles de décharge de 250 mAh avec une lecture à chaque cycle.
 */
static void simulation(void) {
    moniteur_t *mon = moniteur_creer(5400.0f, 11.7f, 10, 5, 20.0f);
    if (mon == NULL) {
        printf("Erreur : creation du moniteur impossible\n");
        return;
    }

    moniteur_charger(mon, 5400.0f);
    moniteur_enregistrer(mon);
    afficher_lecture(mon, 0);

    for (int etape = 1; etape <= 20; etape++) {
        moniteur_decharger(mon, 250.0f);
        moniteur_enregistrer(mon);
        afficher_lecture(mon, etape);
    }

    printf("\n");
    moniteur_afficher(mon);
    moniteur_detruire(mon);
}

/**
 * @brief Point d'entrée : simulation de l'énoncé (tests manuels), puis
 * exécution des tests unitaires.
 *
 * La simulation (voir simulation()) affiche la charge brute, la charge lissée
 * et l'alerte à chaque étape ; elle se vérifie à l'œil.
 */
int main(void) {
    printf("** Tests Manuel : simulation **\n");
    simulation();
    printf("** Fin Tests Manuel **\n\n");

    UNITY_BEGIN();

    RUN_TEST(test_moniteur_creer_valide);
    RUN_TEST(test_moniteur_creer_pile_invalide);
    RUN_TEST(test_moniteur_creer_historique_invalide);
    RUN_TEST(test_moniteur_creer_filtre_invalide);
    RUN_TEST(test_moniteur_creer_seuil_invalide);
    RUN_TEST(test_moniteur_detruire_null);

    RUN_TEST(test_moniteur_sequence_charge_decharge);
    RUN_TEST(test_moniteur_charge_brute_saturee);
    RUN_TEST(test_moniteur_charger_ne_modifie_pas_filtre);

    RUN_TEST(test_moniteur_historique_plafonne);

    RUN_TEST(test_moniteur_lissee_converge);

    RUN_TEST(test_moniteur_alerte_franchissement);
    RUN_TEST(test_moniteur_alerte_desactivee_apres_recharge);
    RUN_TEST(test_moniteur_alerte_recharge_sans_enregistrer);
    RUN_TEST(test_moniteur_alerte_avant_enregistrement);

    return UNITY_END();
}
