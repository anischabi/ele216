#include <stdio.h>
#include "processus.h"
#include "file_gen.h"
#include "fileprio.h"


int main(int argc, char* argv[]) {
    printf("Simulation d'ordonnanceur Round-Robin\n\n");

    fileprio_t *taches = fileprio_creer(4);

    fileprio_enfiler(taches, processus_creer("programme_1", 80, 2), 2);
    fileprio_enfiler(taches, processus_creer("programme_2", 40, 0), 0);
    fileprio_enfiler(taches, processus_creer("programme_3", 50, 1), 1);
    fileprio_enfiler(taches, processus_creer("programme_4", 70, 1), 1);
    fileprio_enfiler(taches, processus_creer("programme_5", 30, 1), 1);
    fileprio_enfiler(taches, processus_creer("programme_6", 40, 1), 1);

    // Simulation d'un ordonanceur "à tour de rôle" sans priorité
    // avec des tranches de temps fixes de 20 microsecondes

    while(! fileprio_est_vide(taches)) {
        processus_t *suivant = fileprio_defiler(taches);
        processus_execute(suivant, 20);
        processus_afficher(suivant);
        if (!processus_est_termine(suivant)) {
            fileprio_enfiler(taches, suivant, processus_obtenir_priorite(suivant));
        }
    }

    printf("Simulation terminée\n");
}
