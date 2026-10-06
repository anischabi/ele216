#ifndef SEQUENCEUR_H
#define SEQUENCEUR_H

/**
 * @file sequenceur.h
 * @brief Module de séquenceur qui exécute des commandes de banc de test.
 *
 * Le séquenceur garde une File (file_gen, cours 4) de commandes à exécuter
 * dans l'ordre d'arrivée, et une Pile (pile_gen, cours 4) de contextes de
 * configuration sauvegardés. Le contexte courant est conservé à part : la
 * Pile ne contient que les copies empilées par CMD_PUSH, elle est donc vide
 * à la création.
 *
 * Les mesures (CMD_MESURER) sont ajoutées à un journal_t (problème 2) reçu
 * en paramètre ; le séquenceur n'en est pas propriétaire.
 */

#include "journal.h"

/** Taille du tableau d'identifiant d'une commande, '\0' final compris. */
#define COMMANDE_ID_MAX 32

/** Contexte de configuration du banc de test. */
typedef struct {
    float tension;    /**< Tension d'alimentation, en volts. */
    float courant;    /**< Courant limite, en ampères. */
    float tolerance;  /**< Tolérance par défaut, en pourcentage (5.0 = 5 %). */
} config_t;

/** Types de commandes reconnus par le séquenceur. */
typedef enum {
    CMD_SET_TENSION,    /**< Modifie la tension du contexte courant. */
    CMD_SET_COURANT,    /**< Modifie le courant limite du contexte courant. */
    CMD_SET_TOLERANCE,  /**< Modifie la tolérance du contexte courant. */
    CMD_MESURER,        /**< Simule une mesure et l'ajoute au journal. */
    CMD_PUSH,           /**< Empile une copie du contexte courant. */
    CMD_POP,            /**< Restaure le dernier contexte empilé. */
    CMD_RAPPORT         /**< Affiche le journal. */
} type_commande_t;

/**
 * Commande à exécuter. Les champs utilisés dépendent du type :
 * - CMD_SET_* : valeur (nouvelle valeur du champ du contexte) ;
 * - CMD_MESURER : id (identifiant du test) et valeur (valeur nominale) ;
 * - CMD_PUSH, CMD_POP, CMD_RAPPORT : aucun champ.
 */
typedef struct {
    type_commande_t type;        /**< Type de la commande. */
    char id[COMMANDE_ID_MAX];    /**< Identifiant du test (ex. "V_REF_3V3"). */
    float valeur;                /**< Valeur numérique associée. */
} commande_t;

/** Type opaque : les champs sont cachés dans sequenceur.c. */
typedef struct sequenceur sequenceur_t;

/**
 * Crée un séquenceur avec une File de commandes vide, une Pile de contextes
 * vide et un contexte courant par défaut (tension = 0 V, courant = 0 A,
 * tolérance = 5 %).
 *
 * @return un pointeur vers le séquenceur créé. NULL en cas d'erreur
 * d'allocation mémoire (rien n'est alors laissé alloué).
 */
sequenceur_t *sequenceur_creer(void);

/**
 * Libère les commandes encore en attente, les contextes encore empilés, la
 * File, la Pile et le séquenceur. Le journal n'est pas touché.
 *
 * @param s le séquenceur à détruire. Si le pointeur est NULL, la fonction
 * n'a pas d'effet.
 */
void sequenceur_detruire(sequenceur_t *s);

/**
 * Ajoute une copie de la commande à la fin de la file d'attente. La
 * commande n'est pas exécutée tout de suite.
 *
 * L'identifiant est toujours terminé par '\0' dans la copie (tronqué à
 * COMMANDE_ID_MAX - 1 caractères au besoin). En cas d'erreur d'allocation,
 * la commande est ignorée avec un message sur stderr.
 *
 * @param s pointeur non nul vers le séquenceur.
 * @param cmd la commande à enfiler (passée par valeur).
 */
void sequenceur_enfiler(sequenceur_t *s, commande_t cmd);

/**
 * @param s pointeur non nul vers le séquenceur.
 * @return le nombre de commandes en attente d'exécution.
 */
int sequenceur_nb_en_attente(const sequenceur_t *s);

/**
 * Défile la plus ancienne commande en attente et l'exécute.
 *
 * Une commande invalide dans le contexte actuel (CMD_POP sur une pile vide,
 * CMD_MESURER ou CMD_RAPPORT sans journal, type inconnu) est ignorée avec
 * un avertissement sur stderr, mais elle compte comme exécutée.
 *
 * CMD_MESURER génère une mesure aléatoire avec rand() : appelez srand()
 * avant pour obtenir des résultats reproductibles.
 *
 * @param s pointeur non nul vers le séquenceur.
 * @param j journal où ajouter les mesures et à afficher pour CMD_RAPPORT.
 * Peut être NULL si aucune commande en attente n'en a besoin.
 * @return 1 si une commande a été défilée et exécutée, 0 si la file était
 * vide.
 */
int sequenceur_executer_prochaine(sequenceur_t *s, journal_t *j);

/**
 * Exécute toutes les commandes en attente, dans l'ordre d'arrivée, jusqu'à
 * ce que la file soit vide.
 *
 * @param s pointeur non nul vers le séquenceur.
 * @param j journal passé à chaque exécution (voir
 * sequenceur_executer_prochaine()).
 */
void sequenceur_executer_tout(sequenceur_t *s, journal_t *j);

/**
 * @param s pointeur non nul vers le séquenceur.
 * @return un pointeur en lecture seule vers le contexte courant. Le
 * pointeur reste valide jusqu'à sequenceur_detruire() ; les valeurs
 * pointées changent quand des commandes sont exécutées.
 */
const config_t *sequenceur_contexte(const sequenceur_t *s);

/**
 * Affiche sur stdout les commandes en attente, de la prochaine à la
 * dernière. La file n'est pas modifiée (même contenu, même ordre).
 *
 * @param s pointeur non nul vers le séquenceur.
 */
void sequenceur_afficher_file(const sequenceur_t *s);

#endif
