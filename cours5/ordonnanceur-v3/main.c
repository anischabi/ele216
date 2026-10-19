#include <stdio.h>
#include <stdint.h>
#include "processus.h"
#include "file_generique.h"
#include "fileprio.h"


int main(int argc, char* argv[]) {
    printf("Simulation de scheduler Round-Robin avec priorités\n\n");

    fileprio_t *taches = fileprio_creer(8);
    fileprio_enfiler(taches, processus_creer("proc 1", 80, 3), 3);
    fileprio_enfiler(taches, processus_creer("proc 2", 50, 2), 2);
    fileprio_enfiler(taches, processus_creer("proc 3", 90, 2), 2);
    fileprio_enfiler(taches, processus_creer("proc 4", 30, 3), 3);
    fileprio_enfiler(taches, processus_creer("proc 5", 10, 4), 4);
    fileprio_enfiler(taches, processus_creer("proc 6", 70, 3), 3);

    // Simulation d'un ordonanceur "à tour de rôle" avec priorité
    // avec des tranches de temps fixes de 20 microsecondes

    while(! fileprio_est_vide(taches)) {
        processus_t *suivant = fileprio_defiler(taches);
        processus_execute(suivant, 20);
        processus_afficher(suivant);
        uint8_t prio = processus_obtenir_priorite(suivant);
        if (!processus_est_termine(suivant)) {
            fileprio_enfiler(taches, suivant, prio);
        }
    }

    printf("Simulation terminée\n");
    return 0;
}
