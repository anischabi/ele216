#include <stdio.h>

#include "capteur.h"

int main(void) {
    capteur_t capteur1;
    capteur_t capteur2;

    capteur_init(&capteur1, 1, "temperature", 21.5f, "C", 1);
    capteur_init(&capteur2, 2, "pression", 101.3f, "kPa", 1);

    printf("Etat initial :\n");
    capteur_afficher(&capteur1);
    capteur_afficher(&capteur2);

    // On met à jour uniquement le capteur1 ; capteur2 ne doit pas changer.
    capteur_mise_a_jour(&capteur1, 23.1f);

    printf("\nApres mise a jour du capteur 1 :\n");
    capteur_afficher(&capteur1);
    capteur_afficher(&capteur2);

    return 0;
}
