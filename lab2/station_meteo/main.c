#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "station.h"

#define NB_CAPTEURS 3
#define NB_MESURES 5

int main(void) {
    srand((unsigned int) time(NULL));

    station_t station = station_creer(NB_CAPTEURS, NB_MESURES);
    if (station.capteurs == NULL || station.historique == NULL) {
        fprintf(stderr, "Erreur : allocation echouee.\n");
        return 1;
    }

    // Plages réalistes par capteur, dans l'ordre défini dans station.c :
    // [0] température (C), [1] pression (kPa), [2] humidité (%).
    float valeur_min[NB_CAPTEURS] = {15.0f, 98.0f, 30.0f};
    float valeur_max[NB_CAPTEURS] = {30.0f, 103.0f, 90.0f};

    for (int mesure = 0; mesure < NB_MESURES; mesure++) {
        float valeurs[NB_CAPTEURS];
        for (int i = 0; i < NB_CAPTEURS; i++) {
            // ratio dans [0, 1], puis mis à l'échelle dans [min, max] du
            // capteur i. (rand() % ... suffirait aussi, mais ici on veut
            // une valeur flottante, pas juste un entier.)
            float ratio = (float) rand() / (float) RAND_MAX;
            valeurs[i] = valeur_min[i] + ratio * (valeur_max[i] - valeur_min[i]);
        }
        station_enregistrer_mesure(&station, mesure, valeurs);
    }

    printf("\n");
    station_afficher_historique(&station);

    printf("\nStatistiques :\n");
    station_afficher_stats(&station);

    station_liberer(&station);

    return 0;
}
