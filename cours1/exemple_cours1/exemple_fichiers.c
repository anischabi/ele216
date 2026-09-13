#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mesure.h"

void test_fprintf(void) {
    fprintf(stdout, "Hello, World!\n");
    fprintf(stderr, "Erreur! À l'aide!\nJe suis pris dans l'ordi!\n");

    FILE *fichier_test;
    char *chemin = "test.txt";

    fichier_test = fopen(chemin, "wt");
    fprintf(fichier_test, "Ceci est le contenu\nd'un fichier texte.\n");
    fclose(fichier_test);
}

void test_lecture_texte(void) {
    // ouverture du fichier en mode lecture, texte
    FILE *mesures = fopen("mesures.txt", "rt");
    if (mesures == NULL) { return; }

    char tampon[80];
    while (fgets(tampon, 80, mesures)) {
        printf("%s", tampon);
    }
    fclose(mesures);
}

void test_ecriture_bin_v1(void) {
    struct mesure donnees[] = {
        {.timestamp = "2026-04-01 14:28:04", .temperature = 22, .humidite = 71},
        {.timestamp = "2026-04-01 14:28:14", .temperature = 23, .humidite = 71},
        {.timestamp = "2026-04-01 14:28:24", .temperature = 23, .humidite = 72},
        {.timestamp = "2026-04-01 14:28:34", .temperature = 23, .humidite = 73},
        {.timestamp = "2026-04-01 14:28:44", .temperature = 22, .humidite = 74},
        {.timestamp = "2026-04-01 14:28:54", .temperature = 21, .humidite = 69},
    };
    FILE *data = fopen("mesures.bin", "wb");
    if (data == NULL) { return; }
    for (int i = 0; i < 6; i++) {
        fwrite(donnees[i].timestamp, sizeof(char),
            strlen(donnees[i].timestamp), data);

        fwrite(&donnees[i].temperature, sizeof(int), 1, data);
        fwrite(&donnees[i].humidite, sizeof(int), 1, data);
    }
    fclose(data);
}

void test_ecriture_bin_v2(void) {
    struct mesure_v2 donnees[6];
    donnees[0].timestamp = creer_date_heure(2026,4,1,14,28,4);
    donnees[0].temperature = 22;
    donnees[0].humidite = 71;

    donnees[1].timestamp = creer_date_heure(2026,4,1,14,28,14);
    donnees[1].temperature = 23;
    donnees[1].humidite = 71;

    donnees[2].timestamp = creer_date_heure(2026,4,1,14,28,24);
    donnees[2].temperature = 23;
    donnees[2].humidite = 72;

    donnees[3].timestamp = creer_date_heure(2026,4,1,14,28,34);
    donnees[3].temperature = 23;
    donnees[3].humidite = 73;

    donnees[4].timestamp = creer_date_heure(2026,4,1,14,28,44);
    donnees[4].temperature = 22;
    donnees[4].humidite = 74;

    donnees[5].timestamp = creer_date_heure(2026,4,1,14,28,54);
    donnees[5].temperature = 21;
    donnees[5].humidite = 69;

    FILE *data = fopen("mesures_v2.bin", "wb");
    if (data == NULL) { return; }
    fwrite(donnees, sizeof(struct mesure_v2), 6, data);
    fclose(data);
}

void test_lecture_bin_v2(void) {
    FILE *source = fopen("mesures_v2.bin", "rb");
    if (source == NULL) { return; }
    struct mesure_v2 *item = malloc(sizeof(struct mesure_v2));

    while (fread(item, sizeof(struct mesure_v2), 1, source)) {
        affiche_mesure_v2(item);
    }
    fclose(source);
    free(item);
}

void test_seek(void) {
    FILE *source = fopen("mesures_v2.bin", "rb");
    if (source == NULL) { return; }
    struct mesure_v2 *item = malloc(sizeof(struct mesure_v2));

    // fseek permet d'avancer le curseur dans le fichier binaire
    //  ici : positionner pour lire le 4e enregistrement (donc après le 3e...)
    fseek(source, 3 * sizeof(struct mesure_v2), SEEK_SET);
    fread(item, sizeof(struct mesure_v2), 1, source);
    affiche_mesure_v2(item);
    free(item);
    fclose(source);
}

int main(void) {
    test_fprintf();
    test_lecture_texte();
    test_ecriture_bin_v1();
    test_ecriture_bin_v2();
    test_lecture_bin_v2();
    printf("\n\n");
    test_seek();

    return 0;
}
