#include <stdio.h>
#include <stdlib.h>

#include "station.h"

station_t station_creer(int nb_capteurs, int nb_mesures) {
    // "station" est une variable LOCALE (sur la pile), mais elle contient
    // des POINTEURS (capteurs, historique) qui, eux, vont pointer vers de
    // la mémoire allouée sur le tas avec malloc(). On la retourne ensuite
    // PAR VALEUR : la struct elle-même est copiée dans la variable du
    // main, mais les pointeurs qu'elle contient continuent de pointer vers
    // la même mémoire allouée (une copie de pointeur reste le même
    // pointeur, elle ne duplique pas ce qu'il y a "au bout").
    station_t station;

    station.nb_capteurs = nb_capteurs;
    station.nb_mesures = nb_mesures;
    // On initialise les deux pointeurs à NULL dès le départ. Comme ça, si
    // une allocation échoue plus loin et qu'on doit retourner tôt, les
    // champs non encore alloués valent NULL plutôt qu'une valeur
    // indéterminée — plus sûr, et ça permet à l'appelant de vérifier
    // facilement si la création a réussi (voir main.c).
    station.capteurs = NULL;
    station.historique = NULL;

    // --- Étape 1 : allouer le tableau de capteurs ---
    // Tableau PLAT de capteur_t : une seule allocation, comme
    // creer_tableau du problème 4. On alloue nb_capteurs "cases", chacune
    // de la taille d'un capteur_t (sizeof(capteur_t)). Contrairement à la
    // matrice plus bas, on n'a PAS besoin d'un tableau de pointeurs ici,
    // car chaque capteur_t a une taille fixe connue à la compilation.
    station.capteurs = malloc(nb_capteurs * sizeof(capteur_t));
    if (station.capteurs == NULL) {
        // malloc a échoué (mémoire insuffisante) : on retourne tout de
        // suite. station.historique est déjà NULL, donc l'appelant pourra
        // détecter l'échec en vérifiant capteurs == NULL.
        return station;
    }

    // --- Étape 2 : initialiser chaque capteur avec des valeurs prédéfinies ---
    // L'énoncé demande au moins 3 capteurs prédéfinis : température,
    // pression, humidité. On appelle capteur_init du problème 3 pour
    // chacun — elle s'occupe déjà de la copie profonde des chaînes de
    // caractères (titre, unite), donc on n'a pas à s'en soucier ici.
    // Les "if" protègent contre le cas où on demanderait MOINS de 3
    // capteurs (ex. station_creer(1, ...) ne doit pas essayer d'écrire
    // dans station.capteurs[1] ou [2], qui n'existeraient pas).
    if (nb_capteurs > 0) {
        capteur_init(&station.capteurs[0], 0, "temperature", 0.0f, "C", 1);
    }
    if (nb_capteurs > 1) {
        capteur_init(&station.capteurs[1], 1, "pression", 0.0f, "kPa", 1);
    }
    if (nb_capteurs > 2) {
        capteur_init(&station.capteurs[2], 2, "humidite", 0.0f, "%", 1);
    }
    // Si on demande PLUS de 3 capteurs, les capteurs supplémentaires (à
    // partir de l'indice 3) reçoivent un nom générique "capteur" — l'énoncé
    // exige seulement "au moins 3 capteurs" prédéfinis, pas que tous le
    // soient.
    for (int i = 3; i < nb_capteurs; i++) {
        capteur_init(&station.capteurs[i], i, "capteur", 0.0f, "?", 1);
    }

    // --- Étape 3 : allouer la matrice d'historique, LIGNE PAR LIGNE ---
    // On veut : lignes = capteurs, colonnes = mesures. Comme au problème 5
    // (creer_matrice), une matrice dynamique en C se construit en 2 temps :
    //   1) un tableau de POINTEURS (un par ligne, donc un par capteur)
    //   2) chaque ligne allouée séparément, pouvant se trouver n'importe
    //      où en mémoire (pas nécessairement les unes à côté des autres).
    // Ici on fait la même chose, mais avec des float plutôt que des int.
    station.historique = malloc(nb_capteurs * sizeof(float *));
    if (station.historique == NULL) {
        // Le tableau de capteurs, lui, a réussi à s'allouer plus haut :
        // il faut donc le libérer avant de retourner, sinon fuite mémoire.
        free(station.capteurs);
        station.capteurs = NULL;
        return station;
    }

    for (int i = 0; i < nb_capteurs; i++) {
        // Chaque ligne i contient nb_mesures float : c'est l'espace pour
        // toutes les mesures du capteur i.
        station.historique[i] = malloc(nb_mesures * sizeof(float));
        if (station.historique[i] == NULL) {
            // Une ligne a échoué en cours de route (ex. la ligne 3 sur 5).
            // Il faut libérer TOUT ce qui a déjà été alloué avec succès
            // avant de retourner, sinon fuite mémoire partielle :
            //   - les lignes 0 à i-1 de historique (déjà allouées)
            //   - le tableau de pointeurs historique lui-même
            //   - le tableau de capteurs (alloué à l'étape 1)
            for (int j = 0; j < i; j++) {
                free(station.historique[j]);
                // On remet le pointeur à NULL après l'avoir libéré : bonne
                // pratique pour éviter un pointeur "dangling" (qui pointe
                // vers de la mémoire déjà libérée), même si ici on
                // retourne immédiatement après.
                station.historique[j] = NULL;
            }
            free(station.historique);
            free(station.capteurs);
            station.historique = NULL;
            station.capteurs = NULL;
            return station;
        }
    }

    // Tout a réussi : on retourne la station complète (par valeur — voir
    // le commentaire tout en haut de la fonction).
    return station;
}

void station_enregistrer_mesure(station_t *s, int idx_mesure, float valeurs[]) {
    // valeurs[] contient une valeur par capteur (même ordre que
    // s->capteurs). Pour chaque capteur i :
    //   1) on écrit sa nouvelle mesure dans la colonne idx_mesure de sa
    //      ligne d'historique
    //   2) on met à jour son champ "derniere_valeur" via capteur_mise_a_jour
    //      (du problème 3), pour qu'un affichage du capteur seul montre
    //      toujours sa valeur la plus récente.
    for (int i = 0; i < s->nb_capteurs; i++) {
        s->historique[i][idx_mesure] = valeurs[i];
        capteur_mise_a_jour(&s->capteurs[i], valeurs[i]);
    }
}

float station_moyenne_capteur(station_t *s, int idx_capteur) {
    // s->historique[idx_capteur] est LA LIGNE (un float*) contenant
    // toutes les mesures de ce capteur. On additionne les nb_mesures
    // valeurs de cette ligne
    float somme = 0.0f;
    for (int j = 0; j < s->nb_mesures; j++) {
        somme += s->historique[idx_capteur][j];
    }
    // puis on divise par le nombre de mesures pour obtenir la moyenne.
    // (On assume ici que toutes les nb_mesures colonnes ont bien été
    // remplies par station_enregistrer_mesure avant cet appel.)
    return somme / s->nb_mesures;
}

int station_indice_valeur_max(station_t *s, int idx_capteur) {
    // Exactement la même logique que indice_max du problème 1
    // (on suppose l'élément 0 maximal, puis on compare avec le reste),
    // mais appliquée à s->historique[idx_capteur] (un float*) plutôt qu'à
    // un int[] passé en paramètre. Le C n'a pas de généricité (pas de
    // templates comme en C++), donc on ne peut pas réutiliser TEL QUEL le
    // indice_max du problème 1 ici — il faut réécrire la même logique
    // pour le type float.
    int indice = 0;
    for (int j = 1; j < s->nb_mesures; j++) {
        if (s->historique[idx_capteur][j] > s->historique[idx_capteur][indice]) {
            indice = j;
        }
    }
    return indice;
}

void station_afficher_historique(station_t *s) {
    printf("Historique (%d mesures) :\n", s->nb_mesures);
    // Une ligne par capteur...
    for (int i = 0; i < s->nb_capteurs; i++) {
        // %-12s : affiche le titre du capteur aligné à gauche sur 12
        // caractères (comme %4d au problème 2, mais pour du texte).
        printf("%-12s: ", s->capteurs[i].titre);
        // ...et sur cette ligne, une colonne par mesure.
        for (int j = 0; j < s->nb_mesures; j++) {
            printf("%8.2f", s->historique[i][j]);
        }
        printf("\n");
    }
}

void station_afficher_stats(station_t *s) {
    for (int i = 0; i < s->nb_capteurs; i++) {
        // On réutilise les deux fonctions écrites plus haut plutôt que de
        // recalculer la logique ici.
        float moyenne = station_moyenne_capteur(s, i);
        int idx_max = station_indice_valeur_max(s, i);
        float valeur_max = s->historique[i][idx_max];

        printf("%-12s : moyenne = %6.2f %-3s, max = %6.2f %-3s (mesure #%d)\n",
            s->capteurs[i].titre, moyenne, s->capteurs[i].unite,
            valeur_max, s->capteurs[i].unite, idx_max + 1);
    }
}

void station_liberer(station_t *s) {
    // On vérifie != NULL au cas où station_creer aurait échoué avant même
    // d'allouer historique (dans ce cas, il n'y a rien à faire ici).
    if (s->historique != NULL) {
        // Chaque ligne d'abord (elles ont été allouées séparément par
        // malloc dans station_creer)...
        for (int i = 0; i < s->nb_capteurs; i++) {
            free(s->historique[i]);
        }
        // ...PUIS le tableau de pointeurs lui-même. Exactement le même
        // ordre que liberer_matrice du problème 5, et pour la même
        // raison : libérer le tableau de pointeurs en premier ferait
        // perdre les adresses des lignes, qui resteraient allouées mais
        // inaccessibles pour toujours.
        free(s->historique);
    }
    // Un seul free ici (pas de boucle) : contrairement à historique,
    // capteurs est un bloc UNIQUE (tableau plat, une seule allocation à
    // l'étape 1 de station_creer), pas un tableau de pointeurs vers
    // plusieurs blocs séparés.
    free(s->capteurs);
}
