#include <stdio.h>
#include <stdint.h>
#include "processus.h"
#include "file_generique.h"
#include "fileprio.h"
#include "ordonnanceur.h"
#include <string.h>

int main(int argc, char* argv[]) {
    char tampon[128];
    bool estTermine = false;
    int commande = 0;
    FILE *source = stdin;
    bool interactif = false;
    bool verbeux = false;

    ordonnanceur_t *simu = ordonnanceur_creer();
    if (simu == NULL) {return 1; }

    switch(argc) {
        case 1:
             printf("*** Simulation d'ordonnanceur Round-Robin avec priorités ***\n");
            printf("COMMANDES : \n");
            printf("  add NAME duration priority\n");
            printf("  run duration\n");
            printf("  quit\n");
            interactif = true;
            break;

        case 2:
            source = fopen(argv[1], "r");
            if (source == NULL) { return -1; }
            break;

        case 3:
            source = fopen(argv[1], "r");
            if (source == NULL) { return -1; }
            if (strcmp("-v", argv[2]) == 0) {
                verbeux = true;
                break;
            }
        
        default :
            printf("Usage : scheduler [fichierCommande] [-v]\n");
            return 0;
    }

    while(! estTermine) {
        // vider le tampon avant la saisie
        memset(tampon, 0, 128);

        if (interactif) { printf(" >"); }
        fgets(tampon, 127, source);
        if (verbeux) { printf("=> %s", tampon); }
        estTermine = ordonnanceur_interprete(simu, tampon);
    }
    return 0;
}
