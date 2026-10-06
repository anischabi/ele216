#ifndef JOURNAL_H
#define JOURNAL_H

/**
 * @file journal.h
 * @brief Module de journal qui accumule des résultats de test.
 *
 * Le journal stocke des resultat_t (problème 1) dans un Vecteur générique
 * (vecteur_gen, cours 4), car le nombre de tests n'est pas connu à
 * l'avance. Le journal est propriétaire des résultats : il les crée dans
 * journal_ajouter() et les détruit dans journal_detruire().
 */

#include "resultat.h"

/** Type opaque : les champs sont cachés dans journal.c. */
typedef struct journal journal_t;

/**
 * Crée un journal vide ainsi que son Vecteur interne.
 *
 * @return un pointeur vers le journal créé. NULL en cas d'erreur
 * d'allocation mémoire (rien n'est alors laissé alloué).
 */
journal_t *journal_creer(void);

/**
 * Libère tous les résultats stockés, le Vecteur interne et le journal.
 *
 * Après l'appel, les pointeurs obtenus par journal_obtenir() ne sont plus
 * valides.
 *
 * @param j le journal à détruire. Si le pointeur est NULL, la fonction n'a
 * pas d'effet.
 */
void journal_detruire(journal_t *j);

/**
 * Crée un resultat_t avec les paramètres reçus et l'ajoute à la fin du
 * journal. Le verdict est calculé par resultat_creer().
 *
 * Si le résultat ne peut pas être créé (id NULL, mémoire insuffisante) ou
 * ajouté au Vecteur, le journal n'est pas modifié.
 *
 * @param j pointeur non nul vers le journal.
 * @param id identifiant du test (copié par resultat_creer()).
 * @param attendu valeur nominale attendue.
 * @param mesure valeur mesurée.
 * @param tolerance tolérance en pourcentage.
 */
void journal_ajouter(journal_t *j, const char *id, float attendu,
                     float mesure, float tolerance);

/**
 * Retourne le nombre total de résultats dans le journal.
 *
 * @param j pointeur non nul vers le journal.
 * @return le nombre de résultats (PASS et FAIL).
 */
int journal_nb_resultats(const journal_t *j);

/**
 * Retourne le nombre de résultats dont le verdict est PASS.
 *
 * @param j pointeur non nul vers le journal.
 * @return le nombre de résultats réussis.
 */
int journal_nb_reussis(const journal_t *j);

/**
 * Retourne le nombre de résultats dont le verdict est FAIL.
 *
 * @param j pointeur non nul vers le journal.
 * @return le nombre de résultats échoués.
 */
int journal_nb_echoues(const journal_t *j);

/**
 * Retourne le résultat à la position donnée (0 = premier ajouté).
 *
 * Le pointeur retourné est const : l'appelant peut lire le résultat mais ne
 * doit ni le modifier ni le détruire (le journal en reste propriétaire).
 *
 * @param j pointeur non nul vers le journal.
 * @param index position du résultat, entre 0 et journal_nb_resultats() - 1.
 * @return un pointeur vers le résultat, ou NULL si l'index est invalide.
 */
const resultat_t *journal_obtenir(const journal_t *j, int index);

/**
 * Affiche à l'écran un rapport tabulaire de tous les résultats, suivi d'un
 * résumé (total, réussis, échoués).
 *
 * @param j pointeur non nul vers le journal.
 */
void journal_afficher(const journal_t *j);

/**
 * Écrit le même rapport que journal_afficher() dans un fichier texte.
 * Le fichier est créé ou écrasé. Si le fichier ne peut pas être ouvert, un
 * message d'erreur est affiché sur stderr et rien n'est écrit.
 *
 * @param j pointeur non nul vers le journal.
 * @param chemin chemin du fichier à écrire. Ne doit pas être NULL.
 */
void journal_sauvegarder(const journal_t *j, const char *chemin);

#endif // JOURNAL_H
