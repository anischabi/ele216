#include <stdio.h>
#include "mod.h"

// int valeur_publique = 0;
int valeur_publique = 20;

int main(void) {
    
    module_a_incrementer();
    printf("valeur_publique = %d\n", valeur_publique);  /* attendu : 11 */
    return 0;
}