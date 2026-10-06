#include "journal.h"
#include "vecteur_gen.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/** Capacité initiale du Vecteur : il double automatiquement au besoin. */
#define JOURNAL_CAPACITE_INITIALE 4

// Définition de la structure dans le .c (type opaque).
// Le Vecteur contient des void* qui pointent tous vers des resultat_t
// alloués par resultat_creer() : le journal en est propriétaire.
struct journal {
    vecteur_t *resultats;
};

static int compter_verdicts(const journal_t *j, int verdict);
static void ecrire_rapport(const journal_t *j, FILE *flux);

journal_t *journal_creer(void) {
    journal_t *j = malloc(sizeof(journal_t));
    if (j == NULL) { return NULL; }

    j->resultats = vecteur_creer(JOURNAL_CAPACITE_INITIALE);
    if (j->resultats == NULL) {
        free(j);  // Sinon la structure du journal fuit.
        return NULL;
    }
    return j;
}

void journal_detruire(journal_t *j) {
    if (j == NULL) { return; }

    // Le Vecteur ne libère pas ses éléments (voir vecteur_liberer) :
    // c'est au journal de détruire chaque résultat avant de libérer le
    // Vecteur, sinon tous les resultat_t fuient.
    int n = vecteur_taille(j->resultats);
    for (int i = 0; i < n; i++) {
        resultat_detruire(vecteur_obtenir(j->resultats, i));
    }
    vecteur_liberer(j->resultats);
    free(j);
}

void journal_ajouter(journal_t *j, const char *id, float attendu,
                     float mesure, float tolerance) {
    assert(j != NULL);

    resultat_t *r = resultat_creer(id, attendu, mesure, tolerance);
    // vecteur_ajouter refuse un pointeur NULL (assert) : on vérifie avant.
    if (r == NULL) { return; }

    if (vecteur_ajouter(j->resultats, r) == 0) {
        resultat_detruire(r);  // Agrandissement impossible : pas de fuite.
    }
}

int journal_nb_resultats(const journal_t *j) {
    assert(j != NULL);
    return vecteur_taille(j->resultats);
}

int journal_nb_reussis(const journal_t *j) {
    return compter_verdicts(j, 1);
}

int journal_nb_echoues(const journal_t *j) {
    return compter_verdicts(j, 0);
}

/**
 * Compte les résultats dont resultat_est_reussi() vaut verdict.
 * On recompte à chaque appel plutôt que de garder des compteurs dans la
 * structure : aucun risque qu'ils deviennent incohérents avec le Vecteur.
 *
 * @param j pointeur non nul vers le journal.
 * @param verdict 1 pour compter les PASS, 0 pour les FAIL.
 * @return le nombre de résultats ayant ce verdict.
 */
static int compter_verdicts(const journal_t *j, int verdict) {
    assert(j != NULL);
    int compte = 0;
    int n = vecteur_taille(j->resultats);
    for (int i = 0; i < n; i++) {
        const resultat_t *r = vecteur_obtenir(j->resultats, i);
        if (resultat_est_reussi(r) == verdict) {
            compte++;
        }
    }
    return compte;
}

const resultat_t *journal_obtenir(const journal_t *j, int index) {
    assert(j != NULL);
    // vecteur_obtenir fait un assert sur la position : on valide avant
    // pour retourner NULL au lieu d'arrêter le programme.
    if (index < 0 || index >= vecteur_taille(j->resultats)) {
        return NULL;
    }
    return vecteur_obtenir(j->resultats, index);
}

/**
 * Écrit le rapport tabulaire et le résumé dans un flux. Partagée par
 * journal_afficher (stdout) et journal_sauvegarder (fichier), pour que
 * l'écran et le fichier aient exactement le même contenu.
 *
 * @param j pointeur non nul vers le journal.
 * @param flux flux de sortie ouvert en écriture.
 */
static void ecrire_rapport(const journal_t *j, FILE *flux) {
    assert(j != NULL);
    assert(flux != NULL);

    fprintf(flux, "%-4s %-16s %10s %10s %8s  %s\n",
            "#", "ID", "ATTENDU", "MESURE", "TOL (%)", "VERDICT");
    fprintf(flux, "------------------------------------------------------------\n");

    int n = journal_nb_resultats(j);
    for (int i = 0; i < n; i++) {
        const resultat_t *r = journal_obtenir(j, i);
        fprintf(flux, "%-4d %-16s %10.3f %10.3f %8.1f  %s\n",
                i, resultat_id(r), resultat_attendu(r), resultat_mesure(r),
                resultat_tolerance(r),
                resultat_est_reussi(r) ? "PASS" : "FAIL");
    }

    fprintf(flux, "------------------------------------------------------------\n");
    fprintf(flux, "Total : %d   Reussis : %d   Echoues : %d\n",
            n, journal_nb_reussis(j), journal_nb_echoues(j));
}

void journal_afficher(const journal_t *j) {
    ecrire_rapport(j, stdout);
}

void journal_sauvegarder(const journal_t *j, const char *chemin) {
    assert(j != NULL);
    assert(chemin != NULL);

    FILE *fichier = fopen(chemin, "w");
    if (fichier == NULL) {
        fprintf(stderr, "journal_sauvegarder : impossible d'ouvrir '%s'\n",
                chemin);
        return;
    }
    ecrire_rapport(j, fichier);
    fclose(fichier);
}
