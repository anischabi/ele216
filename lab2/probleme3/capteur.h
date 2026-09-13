#ifndef CAPTEUR_H
#define CAPTEUR_H

// Tailles fixes imposées par l'énoncé pour les champs texte de capteur_t.
#define TITRE_TAILLE 20
#define UNITE_TAILLE 8

/**
 * Modélise un capteur IoT (ex. un capteur de température, de pression, etc.).
 *
 * titre et unite sont des tableaux de caractères de TAILLE FIXE, pas des
 * char*. Ça signifie que chaque capteur_t POSSÈDE sa propre copie de ces
 * chaînes en mémoire (contrairement à un char* qui ne ferait que pointer
 * vers une chaîne appartenant à quelqu'un d'autre).
 */
typedef struct {
    int id;
    char titre[TITRE_TAILLE];
    float derniere_valeur;
    char unite[UNITE_TAILLE];
    int actif; // booléen : 0 = inactif, non-zéro = actif
} capteur_t;

/**
 * Initialise un capteur avec les valeurs fournies. Fait une COPIE PROFONDE
 * de titre et unite : leur contenu est copié caractère par caractère dans
 * les tableaux internes du capteur, pas seulement leur adresse.
 *
 * @param capteur       pointeur non-nul vers le capteur à initialiser.
 * @param id            identificateur du capteur.
 * @param titre         nom du capteur (ex. "temperature"), copié dans
 *                      capteur->titre (tronqué si trop long pour
 *                      TITRE_TAILLE - 1 caractères + le '\0' final).
 * @param valeur        dernière valeur mesurée.
 * @param unite         unité de mesure (ex. "N/m", "mV", "kWh"), copiée
 *                      dans capteur->unite (mêmes règles que titre).
 * @param actif         état d'activation (0 = inactif, non-zéro = actif).
 */
void capteur_init(capteur_t *capteur, int id, const char *titre,
                   float valeur, const char *unite, int actif);

/**
 * Met à jour la dernière valeur mesurée d'un capteur. Les autres champs
 * (id, titre, unite, actif) ne sont pas modifiés.
 *
 * @param capteur       pointeur non-nul vers le capteur à mettre à jour.
 * @param nouvelle_valeur la nouvelle valeur mesurée.
 */
void capteur_mise_a_jour(capteur_t *capteur, float nouvelle_valeur);

/**
 * Affiche tous les champs du capteur dans un format utile au débogage.
 * @param capteur pointeur non-nul vers le capteur à afficher.
 */
void capteur_afficher(capteur_t *capteur);

#endif // CAPTEUR_H
