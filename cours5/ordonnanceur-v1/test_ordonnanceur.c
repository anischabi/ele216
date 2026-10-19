#include <stdio.h>
#include "processus.h"
#include "file_gen.h"


int main(int argc, char* argv[]) {
    printf("Simulation d'ordonnanceur Round-Robin\n\n");

    file_t *taches = file_creer();

    file_enfiler(taches, processus_creer("programme_1", 80));
    file_enfiler(taches, processus_creer("programme_2", 40));
    file_enfiler(taches, processus_creer("programme_3", 50));

    // Simulation d'un ordonanceur "à tour de rôle" sans priorité
    // avec des tranches de temps fixes de 20 microsecondes

    while(! file_est_vide(taches)) {
        processus_t *suivant = file_defiler(taches);
        processus_execute(suivant, 20);
        processus_afficher(suivant);
        if (!processus_est_termine(suivant)) {
            file_enfiler(taches, suivant);
        }
    }

    printf("Simulation terminée\n");
}
