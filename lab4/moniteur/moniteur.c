#include "moniteur.h"
#include "pile.h"
#include "buffer.h"
#include "filtre.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Définition de la structure dans le .c : les champs sont invisibles de
// l'extérieur du module. Le moniteur est le propriétaire de ses trois
// sous-modules : il les crée dans moniteur_creer et les détruit dans
// moniteur_detruire. Aucune fonction ne modifie seuil_alerte après la
// création, ce champ est donc immuable.
struct moniteur {
    pile_t *pile;           // composant surveillé
    buffer_t *historique;   // dernières lectures, en % arrondi à l'entier
    filtre_t *filtre;       // moyenne mobile des lectures, en %
    float seuil_alerte;     // en %, fixé à la création
};

moniteur_t *moniteur_creer(float capacite, float tension, int taille_historique,
                           int taille_filtre, float seuil_alerte) {
    // Les autres paramètres sont validés par les sous-modules. Écrit avec une
    // négation pour refuser aussi NaN (toute comparaison avec NaN est fausse).
    if (!(seuil_alerte >= 0.0f && seuil_alerte <= 100.0f)) { return NULL; }

    moniteur_t *m = malloc(sizeof(moniteur_t));
    if (m == NULL) { return NULL; }

    m->pile = pile_creer(capacite, tension);
    m->historique = buffer_creer(taille_historique);
    m->filtre = filtre_creer(taille_filtre);
    m->seuil_alerte = seuil_alerte;

    // Si un sous-module a échoué, on libère tout ce qui a été alloué.
    // moniteur_detruire convient, car chaque *_detruire est sans effet sur
    // NULL : les sous-modules qui ont échoué sont simplement ignorés.
    if (m->pile == NULL || m->historique == NULL || m->filtre == NULL) {
        moniteur_detruire(m);
        return NULL;
    }
    return m;
}

void moniteur_detruire(moniteur_t *m) {
    if (m == NULL) { return; }
    // D'abord les sous-modules (m est encore lisible)...
    pile_detruire(m->pile);
    buffer_detruire(m->historique);
    filtre_detruire(m->filtre);
    free(m);                // ...puis la structure qui les référence
}

void moniteur_charger(moniteur_t *m, float mAh) {
    assert(m != NULL);
    pile_charger(m->pile, mAh);
}

void moniteur_decharger(moniteur_t *m, float mAh) {
    assert(m != NULL);
    pile_decharger(m->pile, mAh);
}

void moniteur_enregistrer(moniteur_t *m) {
    assert(m != NULL);
    float pourcentage = pile_pourcentage(m->pile);

    // Le buffer refuse les ajouts quand il est plein : on retire d'abord la
    // lecture la plus ancienne pour garder les plus récentes.
    if (buffer_est_plein(m->historique)) {
        int ancienne;
        buffer_defiler(m->historique, &ancienne);
    }
    buffer_enfiler(m->historique, (int)lroundf(pourcentage));

    filtre_ajouter(m->filtre, pourcentage);
}

float moniteur_charge_brute(const moniteur_t *m) {
    assert(m != NULL);
    return pile_pourcentage(m->pile);
}

float moniteur_charge_lissee(const moniteur_t *m) {
    assert(m != NULL);
    return filtre_valeur(m->filtre);
}

int moniteur_alerte(const moniteur_t *m) {
    assert(m != NULL);
    return moniteur_charge_lissee(m) < m->seuil_alerte;
}

float moniteur_seuil_alerte(const moniteur_t *m) {
    assert(m != NULL);
    return m->seuil_alerte;
}

int moniteur_nb_lectures(const moniteur_t *m) {
    assert(m != NULL);
    return buffer_taille(m->historique);
}

void moniteur_afficher(const moniteur_t *m) {
    assert(m != NULL);
    printf("=== Moniteur de batterie ===\n");
    pile_afficher(m->pile);
    printf("Charge brute  : %.2f %%\n", moniteur_charge_brute(m));
    printf("Historique    : ");
    buffer_afficher(m->historique);
    printf("Charge lissee : %.2f %% (fenetre %d/%d)\n", moniteur_charge_lissee(m),
           filtre_nb_echantillons(m->filtre), filtre_taille_fenetre(m->filtre));
    printf("Alerte        : %s (seuil %.1f %%)\n",
           moniteur_alerte(m) ? "OUI" : "non", m->seuil_alerte);
}
