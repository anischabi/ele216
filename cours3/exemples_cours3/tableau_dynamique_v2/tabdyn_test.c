#include <stdio.h>

#include "tabdyn.h"

int main(void){
    tabdyn_t *tab_test = tabdyn_creer(2);
    tabdyn_ajouter(tab_test, 1);
    tabdyn_ajouter(tab_test, 2);
    tabdyn_ajouter(tab_test, 3);
    tabdyn_ajouter(tab_test, 4);
    tabdyn_ajouter(tab_test, 5);
    tabdyn_afficher(tab_test);
    tabdyn_liberer(tab_test);

    return 0;
}
