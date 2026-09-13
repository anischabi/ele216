#include <stdio.h>

#include "mesure.h"

void affiche_mesure_v2(struct mesure_v2 *mesure) {
    printf("%04d-%02d-%02d  %02d:%02d:%02d Temperature : %d  Humidite %d\n",
        mesure->timestamp.annee,
        mesure->timestamp.mois, mesure->timestamp.jours,
        mesure->timestamp.heure, mesure->timestamp.minute,
        mesure->timestamp.seconde, mesure->temperature, mesure->humidite);
}
