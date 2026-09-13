# Questions de vérification — Problème 3 (Structures)

But : vérifier que l'étudiant.e comprend son code, pas juste qu'il fonctionne.
Piger 2-3 questions selon le temps disponible.

## Copie profonde (le cœur du problème)

- Qu'est-ce qu'une "copie profonde" ? Pourquoi est-ce nécessaire ici ?
  *(Copier le CONTENU d'une chaîne caractère par caractère dans le tableau
  interne du capteur, plutôt que de copier une adresse/pointeur. Nécessaire
  car `titre`/`unite` sont des `char[N]` internes à la struct — le capteur
  doit posséder sa propre copie indépendante.)*
- Que se passerait-il si `titre` était déclaré `char *titre` au lieu de
  `char titre[20]` dans la struct, et qu'on faisait simplement
  `capteur->titre = titre_recu;` ?
  *(Le capteur ne ferait que pointer vers la mémoire du paramètre reçu —
  pas de copie. Si cette mémoire est libérée ou modifiée ailleurs (ex. une
  variable locale d'une autre fonction), le capteur se retrouverait avec un
  pointeur invalide ou une valeur inattendue.)*
- Pourquoi ne peut-on pas simplement écrire
  `capteur->titre = titre;` même avec `char titre[20]` dans la struct ?
  *(En C, on ne peut pas assigner un tableau avec `=` — il faut copier son
  contenu élément par élément, via une fonction comme `strncpy` ou
  `snprintf`.)*

## `snprintf` vs `strncpy`

- Pourquoi a-t-on choisi `snprintf` plutôt que `strncpy` pour copier
  `titre` et `unite` ?
  *(`snprintf` garantit toujours une chaîne terminée par `'\0'`, alors que
  `strncpy` ne le garantit PAS si la source est aussi longue ou plus longue
  que la taille de destination.)*
- Si on utilisait `strncpy` à la place (voir le code en commentaire dans
  `capteur.c`), quelle étape supplémentaire faut-il absolument ajouter, et
  pourquoi ?
  *(Forcer manuellement `capteur->titre[TITRE_TAILLE - 1] = '\0';` après
  l'appel, sinon la chaîne pourrait ne pas être terminée correctement, ce
  qui causerait une lecture hors limites lors d'un affichage ultérieur.)*
- Que se passe-t-il si le `titre` fourni fait plus de 19 caractères ?
  *(Il est tronqué à 19 caractères + le `'\0'` final, pour respecter la
  taille de `TITRE_TAILLE` (20).)*

## `capteur_mise_a_jour`

- Pourquoi cette fonction ne prend-elle que la nouvelle valeur en
  paramètre, et pas le titre ou l'unité ?
  *(Seule la valeur mesurée change dans le temps ; les autres champs sont
  fixes après l'initialisation, donc pas besoin de les repasser.)*

## `main` / général

- Pourquoi passe-t-on `&capteur1` (une adresse) aux fonctions plutôt que
  `capteur1` directement ?
  *(Pour que les fonctions puissent MODIFIER le capteur original ; passer
  la struct par valeur créerait une copie locale, et les modifications ne
  seraient pas visibles à l'extérieur de la fonction.)*
- Après avoir mis à jour `capteur1`, pourquoi `capteur2` reste-t-il
  inchangé ?
  *(Chaque capteur est une struct indépendante en mémoire ; mettre à jour
  l'un ne touche pas l'autre.)*
