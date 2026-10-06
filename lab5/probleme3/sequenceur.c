#include "sequenceur.h"
#include "file_gen.h"
#include "pile_gen.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/** Valeurs du contexte par défaut (énoncé). */
#define TENSION_DEFAUT   0.0f
#define COURANT_DEFAUT   0.0f
#define TOLERANCE_DEFAUT 5.0f

/** Amplitude du bruit de CMD_MESURER : ±10 % de la valeur nominale. */
#define BRUIT_RELATIF 0.10f

// Définition de la structure dans le .c (type opaque).
// La File contient des void* vers des commande_t et la Pile des void* vers
// des config_t, tous alloués par le séquenceur : il en est propriétaire.
// Le contexte courant n'est PAS dans la Pile : il est stocké par valeur,
// la Pile ne garde que les copies sauvegardées par CMD_PUSH.
struct sequenceur {
    file_gen_t *commandes;
    pile_gen_t *contextes;
    config_t courant;
};

static void executer(sequenceur_t *s, const commande_t *c, journal_t *j);
static void empiler_contexte(sequenceur_t *s);
static void depiler_contexte(sequenceur_t *s);
static void mesurer(const sequenceur_t *s, const commande_t *c, journal_t *j);
static const char *nom_commande(type_commande_t type);

sequenceur_t *sequenceur_creer(void) {
    sequenceur_t *s = malloc(sizeof(sequenceur_t));
    if (s == NULL) { return NULL; }

    s->commandes = file_gen_creer();
    s->contextes = pile_gen_creer();
    if (s->commandes == NULL || s->contextes == NULL) {
        // Une seule des deux a pu réussir : on libère ce qui existe.
        if (s->commandes != NULL) { file_gen_liberer(s->commandes); }
        if (s->contextes != NULL) { pile_gen_liberer(s->contextes); }
        free(s);
        return NULL;
    }

    s->courant.tension = TENSION_DEFAUT;
    s->courant.courant = COURANT_DEFAUT;
    s->courant.tolerance = TOLERANCE_DEFAUT;
    return s;
}

void sequenceur_detruire(sequenceur_t *s) {
    if (s == NULL) { return; }

    // La File et la Pile ne libèrent pas leurs éléments (voir
    // vecteur_liberer) : on les vide d'abord, sinon les commandes en
    // attente et les contextes empilés fuient.
    while (!file_gen_est_vide(s->commandes)) {
        free(file_gen_defiler(s->commandes));
    }
    while (!pile_gen_est_vide(s->contextes)) {
        free(pile_gen_depiler(s->contextes));
    }
    file_gen_liberer(s->commandes);
    pile_gen_liberer(s->contextes);
    free(s);
}

void sequenceur_enfiler(sequenceur_t *s, commande_t cmd) {
    assert(s != NULL);

    // cmd est une variable locale (passage par valeur) : stocker &cmd
    // laisserait dans la File un pointeur vers une zone libérée au retour.
    // On enfile donc une copie allouée sur le tas.
    commande_t *copie = malloc(sizeof(commande_t));
    if (copie == NULL) {
        fprintf(stderr, "Erreur : memoire insuffisante, commande ignoree\n");
        return;
    }
    *copie = cmd;
    // Garantit une chaîne valide même si l'appelant a rempli tout le tableau.
    copie->id[COMMANDE_ID_MAX - 1] = '\0';

    // file_gen_enfiler ignore l'échec de vecteur_ajouter : on le détecte
    // par la taille, pour ne pas perdre la copie.
    int avant = file_gen_taille(s->commandes);
    file_gen_enfiler(s->commandes, copie);
    if (file_gen_taille(s->commandes) == avant) {
        fprintf(stderr, "Erreur : memoire insuffisante, commande ignoree\n");
        free(copie);
    }
}

int sequenceur_nb_en_attente(const sequenceur_t *s) {
    assert(s != NULL);
    return file_gen_taille(s->commandes);
}

int sequenceur_executer_prochaine(sequenceur_t *s, journal_t *j) {
    assert(s != NULL);
    // file_gen_defiler sur une file vide lit hors du tableau (pas d'assert
    // dans vecteur_retirer_premier) : on vérifie avant.
    if (file_gen_est_vide(s->commandes)) {
        return 0;
    }

    commande_t *c = file_gen_defiler(s->commandes);
    executer(s, c, j);
    free(c);
    return 1;
}

void sequenceur_executer_tout(sequenceur_t *s, journal_t *j) {
    while (sequenceur_executer_prochaine(s, j)) {
        // Le travail est fait dans la condition.
    }
}

const config_t *sequenceur_contexte(const sequenceur_t *s) {
    assert(s != NULL);
    return &s->courant;
}

void sequenceur_afficher_file(const sequenceur_t *s) {
    assert(s != NULL);

    int n = file_gen_taille(s->commandes);
    printf("Commandes en attente : %d\n", n);

    // La File du cours ne permet de voir que sa tête. On la fait tourner
    // n fois (défiler puis réenfiler) : après un tour complet, elle a
    // retrouvé le même contenu dans le même ordre. Le réenfilage ne peut
    // pas échouer, car la taille ne dépasse jamais n (capacité suffisante).
    // C'est permis malgré le const : s->commandes est un pointeur constant
    // vers une File qui, elle, ne l'est pas.
    for (int i = 0; i < n; i++) {
        commande_t *c = file_gen_defiler(s->commandes);
        printf("  %-3d %-18s %-16s %10.3f\n", i, nom_commande(c->type),
               c->id[0] != '\0' ? c->id : "-", c->valeur);
        file_gen_enfiler(s->commandes, c);
    }
}

/**
 * Applique l'effet d'une commande sur le séquenceur ou le journal.
 *
 * @param s pointeur non nul vers le séquenceur.
 * @param c commande à exécuter (non nulle).
 * @param j journal, peut être NULL.
 */
static void executer(sequenceur_t *s, const commande_t *c, journal_t *j) {
    switch (c->type) {
    case CMD_SET_TENSION:
        s->courant.tension = c->valeur;
        break;
    case CMD_SET_COURANT:
        s->courant.courant = c->valeur;
        break;
    case CMD_SET_TOLERANCE:
        s->courant.tolerance = c->valeur;
        break;
    case CMD_MESURER:
        mesurer(s, c, j);
        break;
    case CMD_PUSH:
        empiler_contexte(s);
        break;
    case CMD_POP:
        depiler_contexte(s);
        break;
    case CMD_RAPPORT:
        if (j == NULL) {
            fprintf(stderr, "Avertissement : CMD_RAPPORT ignoree, aucun journal\n");
        } else {
            journal_afficher(j);
        }
        break;
    default:
        fprintf(stderr, "Avertissement : type de commande inconnu (%d), ignoree\n",
                (int)c->type);
        break;
    }
}

/**
 * CMD_PUSH : empile une copie du contexte courant. Le contexte courant
 * reste inchangé ; les commandes suivantes modifient le courant, pas la
 * copie.
 *
 * @param s pointeur non nul vers le séquenceur.
 */
static void empiler_contexte(sequenceur_t *s) {
    config_t *copie = malloc(sizeof(config_t));
    if (copie == NULL) {
        fprintf(stderr, "Erreur : memoire insuffisante, CMD_PUSH ignoree\n");
        return;
    }
    *copie = s->courant;

    // Même vérification que dans sequenceur_enfiler : pile_gen_empiler
    // ignore l'échec de vecteur_ajouter.
    int avant = pile_gen_taille(s->contextes);
    pile_gen_empiler(s->contextes, copie);
    if (pile_gen_taille(s->contextes) == avant) {
        fprintf(stderr, "Erreur : memoire insuffisante, CMD_PUSH ignoree\n");
        free(copie);
    }
}

/**
 * CMD_POP : remplace le contexte courant par le dernier contexte empilé.
 * Si la pile est vide, le contexte courant est conservé et un
 * avertissement est affiché.
 *
 * @param s pointeur non nul vers le séquenceur.
 */
static void depiler_contexte(sequenceur_t *s) {
    // pile_gen_depiler sur une pile vide lit hors du tableau (pas d'assert
    // dans vecteur_retirer_dernier) : on vérifie avant.
    if (pile_gen_est_vide(s->contextes)) {
        fprintf(stderr, "Avertissement : CMD_POP ignoree, pile de contextes vide\n");
        return;
    }
    config_t *sauvegarde = pile_gen_depiler(s->contextes);
    s->courant = *sauvegarde;
    free(sauvegarde);  // Le contexte a été copié : la sauvegarde ne sert plus.
}

/**
 * CMD_MESURER : simule une mesure valeur * (1 + bruit), avec un bruit
 * uniforme dans [-10 %, +10 %], et l'ajoute au journal avec la tolérance
 * du contexte courant.
 *
 * @param s pointeur non nul vers le séquenceur.
 * @param c commande CMD_MESURER (id et valeur nominale).
 * @param j journal où ajouter le résultat, peut être NULL.
 */
static void mesurer(const sequenceur_t *s, const commande_t *c, journal_t *j) {
    if (j == NULL) {
        fprintf(stderr, "Avertissement : CMD_MESURER %s ignoree, aucun journal\n",
                c->id);
        return;
    }
    // rand() / RAND_MAX est dans [0, 1] : on le ramène dans [-1, 1], puis
    // dans [-BRUIT_RELATIF, +BRUIT_RELATIF].
    float aleatoire = (float)rand() / (float)RAND_MAX;
    float bruit = (2.0f * aleatoire - 1.0f) * BRUIT_RELATIF;
    float mesure = c->valeur * (1.0f + bruit);

    journal_ajouter(j, c->id, c->valeur, mesure, s->courant.tolerance);
}

/**
 * @param type type de commande.
 * @return le nom de la constante (ex. "CMD_PUSH"), pour l'affichage.
 */
static const char *nom_commande(type_commande_t type) {
    switch (type) {
    case CMD_SET_TENSION:   return "CMD_SET_TENSION";
    case CMD_SET_COURANT:   return "CMD_SET_COURANT";
    case CMD_SET_TOLERANCE: return "CMD_SET_TOLERANCE";
    case CMD_MESURER:       return "CMD_MESURER";
    case CMD_PUSH:          return "CMD_PUSH";
    case CMD_POP:           return "CMD_POP";
    case CMD_RAPPORT:       return "CMD_RAPPORT";
    default:                return "INCONNUE";
    }
}
