#ifndef STATION_H
#define STATION_H

// Réutilisation directe de capteur_t du problème 3 (pas de copie, on
// inclut le .h depuis le dossier voisin, comme au problème 4).
#include "../probleme3/capteur.h"

/**
 * Une station météo gère un réseau de capteurs et l'historique de leurs
 * mesures.
 *
 * nb_mesures est fixé UNE FOIS à la création (par station_creer) : c'est
 * le nombre de colonnes de la matrice historique. Le programme est censé
 * enregistrer exactement nb_mesures séries de valeurs avant d'afficher les
 * statistiques.
 */
typedef struct {
    capteur_t *capteurs; // tableau PLAT de capteur_t (une seule allocation)
    int nb_capteurs;
    int nb_mesures;       // nombre de colonnes de historique (fixé à la création)
    float **historique;   // lignes = capteurs, colonnes = nb_mesures
} station_t;

/**
 * Crée une station météo : alloue le tableau de capteurs et la matrice
 * d'historique, puis initialise les capteurs avec des valeurs prédéfinies
 * (température, pression, humidité, dans cet ordre pour les 3 premiers).
 *
 * @param nb_capteurs nombre de capteurs à créer.
 * @param nb_mesures  nombre de colonnes à allouer dans l'historique
 *                    (nombre de relevés que la station pourra contenir).
 * @return la station créée. En cas d'échec d'allocation, capteurs et/ou
 *         historique valent NULL — à vérifier avant utilisation.
 */
station_t station_creer(int nb_capteurs, int nb_mesures);

/**
 * Enregistre une nouvelle série de mesures (une valeur par capteur) dans
 * la colonne idx_mesure de l'historique, et met à jour la valeur courante
 * de chaque capteur.
 *
 * @param s          pointeur non-nul vers la station.
 * @param idx_mesure indice de la colonne à remplir (0 à nb_mesures - 1).
 * @param valeurs    tableau d'au moins nb_capteurs valeurs, une par capteur,
 *                   dans le même ordre que le tableau capteurs.
 */
void station_enregistrer_mesure(station_t *s, int idx_mesure, float valeurs[]);

/**
 * @param s           pointeur non-nul vers la station.
 * @param idx_capteur indice du capteur.
 * @return la moyenne de toutes les mesures enregistrées pour ce capteur.
 */
float station_moyenne_capteur(station_t *s, int idx_capteur);

/**
 * @param s           pointeur non-nul vers la station.
 * @param idx_capteur indice du capteur.
 * @return l'indice (dans l'historique) de la mesure maximale de ce capteur.
 */
int station_indice_valeur_max(station_t *s, int idx_capteur);

/**
 * Affiche la matrice d'historique sous forme de grille : capteurs en
 * lignes (précédés de leur titre), relevés en colonnes.
 * @param s pointeur non-nul vers la station.
 */
void station_afficher_historique(station_t *s);

/**
 * Pour chaque capteur, affiche son nom, sa moyenne, ainsi que la valeur et
 * l'indice de sa mesure maximale.
 * @param s pointeur non-nul vers la station.
 */
void station_afficher_stats(station_t *s);

/**
 * Libère le tableau de capteurs et la matrice d'historique.
 * @param s pointeur non-nul vers la station à libérer.
 */
void station_liberer(station_t *s);

#endif // STATION_H
