#ifndef MONITEUR_H
#define MONITEUR_H

/**
 * @file moniteur.h
 * @brief Module de moniteur de batterie (mini-app du laboratoire 4).
 *
 * Le moniteur surveille une pile au fil du temps. Il réutilise les modules du
 * laboratoire 3 :
 *  - une pile (pile.h) : le composant surveillé ;
 *  - un buffer FIFO (buffer.h) : l'historique des derniers pourcentages
 *    mesurés, arrondis à l'entier ;
 *  - un filtre moyenne mobile (filtre.h) : lisse les pourcentages mesurés.
 *
 * Le seuil d'alerte est fixé à la création et ne peut plus changer.
 * Charger ou décharger la pile ne modifie ni l'historique ni le filtre :
 * seule moniteur_enregistrer prend une nouvelle mesure.
 */

/** Type opaque : les champs sont cachés dans moniteur.c. */
typedef struct moniteur moniteur_t;

/**
 * Crée un moniteur ET tous ses sous-modules. La pile est initialement
 * déchargée, l'historique et le filtre sont vides.
 *
 * @param capacite          capacité de la pile, en mAh (> 0).
 * @param tension           tension nominale de la pile, en V (> 0).
 * @param taille_historique nombre de lectures conservées dans l'historique (> 0).
 * @param taille_filtre     taille de la fenêtre du filtre (> 0).
 * @param seuil_alerte      seuil d'alerte, en pourcentage, entre 0 et 100.
 * @return un pointeur vers le moniteur créé. NULL si un paramètre est
 * invalide ou si l'un des sous-modules échoue à s'allouer ; dans ce cas, ce
 * qui avait déjà été alloué est libéré.
 */
moniteur_t *moniteur_creer(float capacite, float tension, int taille_historique,
                           int taille_filtre, float seuil_alerte);

/**
 * Libère le moniteur ET tous ses sous-modules.
 *
 * @param m le moniteur à détruire. Si le pointeur est NULL, la fonction n'a
 * pas d'effet. Après l'appel, le pointeur de l'appelant n'est plus valide.
 */
void moniteur_detruire(moniteur_t *m);

/**
 * Applique une charge à la pile interne (saturée à la capacité). Ne modifie
 * ni l'historique ni le filtre. Une valeur négative ou nulle est ignorée.
 *
 * @param m   pointeur non-nul vers le moniteur.
 * @param mAh quantité de charge à ajouter, en mAh.
 */
void moniteur_charger(moniteur_t *m, float mAh);

/**
 * Applique une décharge à la pile interne (la charge ne descend pas sous 0).
 * Ne modifie ni l'historique ni le filtre. Une valeur négative ou nulle est
 * ignorée.
 *
 * @param m   pointeur non-nul vers le moniteur.
 * @param mAh quantité de charge à retirer, en mAh.
 */
void moniteur_decharger(moniteur_t *m, float mAh);

/**
 * Lit le pourcentage courant de la pile et l'ajoute à la fois à l'historique
 * (arrondi à l'entier le plus proche) et au filtre (valeur exacte). Si
 * l'historique est plein, sa lecture la plus ancienne est retirée pour faire
 * de la place : il contient toujours les dernières lectures.
 *
 * @param m pointeur non-nul vers le moniteur.
 */
void moniteur_enregistrer(moniteur_t *m);

/**
 * @param m pointeur non-nul vers le moniteur.
 * @return le pourcentage instantané de la pile (lecture directe), entre 0 et 100.
 */
float moniteur_charge_brute(const moniteur_t *m);

/**
 * @param m pointeur non-nul vers le moniteur.
 * @return le pourcentage filtré (moyenne des dernières lectures enregistrées).
 * 0 si aucune lecture n'a encore été enregistrée.
 */
float moniteur_charge_lissee(const moniteur_t *m);

/**
 * Indique si la charge lissée est sous le seuil d'alerte. Comme la charge
 * lissée vaut 0 tant qu'aucune lecture n'a été enregistrée, l'alerte est
 * active avant le premier enregistrement.
 *
 * @param m pointeur non-nul vers le moniteur.
 * @return 1 si la charge lissée est strictement inférieure au seuil, 0 sinon.
 */
int moniteur_alerte(const moniteur_t *m);

/**
 * @param m pointeur non-nul vers le moniteur.
 * @return le seuil d'alerte, en pourcentage. (Accesseur ajouté pour les tests.)
 */
float moniteur_seuil_alerte(const moniteur_t *m);

/**
 * @param m pointeur non-nul vers le moniteur.
 * @return le nombre de lectures dans l'historique, au plus taille_historique.
 * (Accesseur ajouté pour les tests.)
 */
int moniteur_nb_lectures(const moniteur_t *m);

/**
 * Affiche l'état de la pile, le contenu de l'historique (du plus ancien au
 * plus récent), la charge lissée et l'état de l'alerte.
 *
 * @param m pointeur non-nul vers le moniteur.
 */
void moniteur_afficher(const moniteur_t *m);

#endif // MONITEUR_H
