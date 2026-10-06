#ifndef MOD_H
#define MOD_H

/* ERREUR VOLONTAIRE : definition sans extern dans le .h */
// int valeur_publique;

/* Version correcte (a utiliser pour la correction) :
 * extern int valeur_publique;
 */
int valeur_publique;
void module_a_incrementer(void);

#endif