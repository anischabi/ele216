#include <stdio.h>

#include "capteur.h"

void capteur_init(capteur_t *capteur, int id, const char *titre,
                   float valeur, const char *unite, int actif) {
    capteur->id = id;

    // Copie profonde : snprintf copie le contenu de titre/unite
    // caractère par caractère dans les tableaux internes du capteur, et
    // garantit toujours un '\0' final (contrairement à strncpy, qui ne
    // termine pas forcément la chaîne si la source est trop longue).
    snprintf(capteur->titre, TITRE_TAILLE, "%s", titre);
    snprintf(capteur->unite, UNITE_TAILLE, "%s", unite);

    /*
     * Équivalent avec strncpy (NON utilisé ici, gardé en commentaire à
     * titre de comparaison) :
     *
     *     strncpy(capteur->titre, titre, TITRE_TAILLE - 1);
     *     capteur->titre[TITRE_TAILLE - 1] = '\0';
     *
     *     strncpy(capteur->unite, unite, UNITE_TAILLE - 1);
     *     capteur->unite[UNITE_TAILLE - 1] = '\0';
     *
     * Pourquoi "- 1" et la ligne '\0' supplémentaire ?
     * strncpy(dest, src, n) copie au plus n caractères de src dans dest,
     * MAIS s'arrête d'écrire un '\0' si src fait n caractères ou plus
     * (aucune garantie de terminaison). On limite donc la copie à
     * TAILLE - 1 caractères, puis on force nous-mêmes le dernier octet du
     * tableau à '\0' pour garantir une chaîne valide dans tous les cas.
     * Oublier cette ligne est une source classique de bug (lecture hors
     * limites lors d'un printf("%s", ...) subséquent).
     * snprintf gère ce détail automatiquement, ce qui est pourquoi on l'a
     * préférée ici.
     */

    capteur->derniere_valeur = valeur;
    capteur->actif = actif;
}

void capteur_mise_a_jour(capteur_t *capteur, float nouvelle_valeur) {
    capteur->derniere_valeur = nouvelle_valeur;
}

void capteur_afficher(capteur_t *capteur) {
    printf("capteur #%d [%s] : %.2f %s (actif: %s)\n",
        capteur->id, capteur->titre, capteur->derniere_valeur,
        capteur->unite, capteur->actif ? "oui" : "non");
}
