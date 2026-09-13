#ifndef DATE_HEURE_H
#define DATE_HEURE_H

#include <stdint.h>

struct date_heure {
    uint16_t annee;
    uint8_t mois;
    uint8_t jours;
    uint8_t heure;
    uint8_t minute;
    uint8_t seconde;
};

/**
 * Crée et retourne une structure date_heure initialisée.
 * @param annee   l'annee
 * @param mois    le mois
 * @param jours   le jours
 * @param heure   l'heure
 * @param minute  la minute
 * @param seconde la seconde
 * @return la structure correctement initialisée.
 */
struct date_heure creer_date_heure(uint16_t annee,
                                    uint8_t mois,
                                    uint8_t jours,
                                    uint8_t heure,
                                    uint8_t minute,
                                    uint8_t seconde);

#endif //DATE_HEURE_H