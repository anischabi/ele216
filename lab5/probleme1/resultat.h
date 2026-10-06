#ifndef RESULTAT_H
#define RESULTAT_H

/**
 * @file resultat.h
 * @brief Module de résultat de test individuel (banc de test automatisé).
 *
 * Un résultat associe un identifiant de test à une valeur attendue, une
 * valeur mesurée et une tolérance en pourcentage. Le verdict (PASS ou FAIL)
 * est calculé une seule fois à la création et ne peut plus être modifié :
 * le module n'offre aucune fonction de modification.
 *
 * Verdict : PASS si la mesure se trouve dans l'intervalle
 * [attendu - |attendu| * tolérance / 100, attendu + |attendu| * tolérance / 100],
 * bornes incluses. FAIL sinon.
 */

/** Type opaque : les champs sont cachés dans resultat.c. */
typedef struct resultat resultat_t;

/**
 * Crée un résultat de test et calcule son verdict.
 *
 * L'identifiant est copié (copie profonde) : l'appelant reste propriétaire
 * de la chaîne passée et peut la modifier ou la libérer après l'appel.
 *
 * @param id identifiant du test (ex. "V_REF_3V3"). Ne doit pas être NULL.
 * @param attendu valeur nominale attendue.
 * @param mesure valeur réellement mesurée.
 * @param tolerance tolérance en pourcentage de la valeur attendue (ex. 5.0
 * pour ±5 %). Devrait être >= 0 ; une tolérance négative donne un
 * intervalle vide, donc toujours FAIL.
 * @return un pointeur vers le résultat créé. NULL si id est NULL ou en cas
 * d'erreur d'allocation mémoire.
 */
resultat_t *resultat_creer(const char *id, float attendu, float mesure,
                           float tolerance);

/**
 * Libère la mémoire d'un résultat (l'identifiant copié et la structure).
 *
 * @param r le résultat à détruire. Si le pointeur est NULL, la fonction n'a
 * pas d'effet (voir la définition de free()).
 */
void resultat_detruire(resultat_t *r);

/**
 * Retourne l'identifiant du test.
 *
 * @param r pointeur non nul vers le résultat.
 * @return la chaîne interne du résultat (lecture seule). Elle reste valide
 * jusqu'à la destruction du résultat.
 */
const char *resultat_id(const resultat_t *r);

/**
 * Retourne la valeur attendue.
 *
 * @param r pointeur non nul vers le résultat.
 * @return la valeur attendue reçue à la création.
 */
float resultat_attendu(const resultat_t *r);

/**
 * Retourne la valeur mesurée.
 *
 * @param r pointeur non nul vers le résultat.
 * @return la valeur mesurée reçue à la création.
 */
float resultat_mesure(const resultat_t *r);

/**
 * Retourne la tolérance en pourcentage.
 *
 * @param r pointeur non nul vers le résultat.
 * @return la tolérance reçue à la création (ex. 5.0 pour ±5 %).
 */
float resultat_tolerance(const resultat_t *r);

/**
 * Indique le verdict du test.
 *
 * @param r pointeur non nul vers le résultat.
 * @return 1 si le verdict est PASS, 0 si FAIL.
 */
int resultat_est_reussi(const resultat_t *r);

/**
 * Affiche tous les champs du résultat sur une ligne formatée, par exemple :
 * @code
 * V_REF_3V3    attendu =    3.300  mesure =    3.350  tol =   5.0 %  PASS
 * @endcode
 *
 * @param r pointeur non nul vers le résultat.
 */
void resultat_afficher(const resultat_t *r);

#endif // RESULTAT_H
