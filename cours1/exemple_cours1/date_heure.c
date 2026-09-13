#include "date_heure.h"

struct date_heure creer_date_heure(uint16_t annee,
                                   uint8_t mois,
                                   uint8_t jours,
                                   uint8_t heure,
                                   uint8_t minute,
                                   uint8_t seconde) 
{
    struct date_heure d;
    d.annee = annee;
    d.mois = mois;
    d.jours = jours;
    d.heure = heure;
    d.minute = minute;
    d.seconde = seconde;
    return d;
}