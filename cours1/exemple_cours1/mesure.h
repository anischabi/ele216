#ifndef MESURE_H
#define MESURE_H
#include "date_heure.h"

struct mesure {
    char *timestamp;
    int temperature;
    int humidite;
};

struct mesure_v2 {
    struct date_heure timestamp;
    int8_t temperature;
    uint8_t humidite;
};

void affiche_mesure_v2(struct mesure_v2 *mesure);

#endif //MESURE_H