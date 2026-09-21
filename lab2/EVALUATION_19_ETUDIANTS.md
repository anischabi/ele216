# Labo 2 — Grille de questions pour 19 étudiant.e.s

Une question par problème (7 questions au total : Problèmes 1 à 6 +
Station météo) par étudiant.e, piochées dans `QUESTIONS_LAB2.md` avec un
décalage différent par problème pour éviter que deux étudiant.e.s voisin.e.s
reçoivent exactement les mêmes questions dans le même ordre. Quelques
répétitions existent (c'est voulu — les questions du même problème sont en
nombre limité), mais chaque question n'est réutilisée qu'une ou deux fois sur
les 19.

Remplace "Étudiant 1", "Étudiant 2", ... par les noms réels. Rappel du
guide : 2-3 questions suffisent normalement par étudiant.e selon le temps —
ceci te donne 7 questions **au choix**, pas 7 questions obligatoires à poser.

---

## Étudiant 1

- **P1** — Que retourne la fonction si `n = 0` ? Pourquoi ?
  *(0, car `somme` est initialisée à 0 et la boucle ne s'exécute jamais.)*
- **P2** — Pourquoi `sommes_colonnes` (version ligne par ligne) doit-elle
  initialiser `resultats[j]` à 0 avant la boucle principale ?
  *(On accumule dans `resultats[j]` au fil de plusieurs lignes.)*
- **P3** — Que se passerait-il si on appelait `capteur_init(&c, 1, NULL,
  0.0f, "C", 1)` (titre `NULL`) ?
  *(Comportement indéfini — `snprintf("%s", NULL)` n'est pas garanti sûr.)*
- **P4** — Pourquoi `malloc(n * sizeof(int))` plutôt que `malloc(n)` ?
  *(`malloc` prend un nombre d'octets, pas d'éléments.)*
- **P5** — Décris, étape par étape, ce que fait `matrice[i][j]`.
  *(`matrice[i]` est `int *` (adresse de la ligne), déréférencé à l'indice
  `j` pour obtenir un `int`.)*
- **P6** — Pourquoi utilise-t-on un tableau statique ici, alors qu'on vient
  de voir l'allocation dynamique aux problèmes 4 et 5 ?
  *(Taille (5 points) connue et fixe à la compilation.)*
- **Station** — Pourquoi `capteurs` est un tableau PLAT alors que
  `historique` est un tableau de pointeurs ?
  *(`capteur_t` a une taille fixe ; `historique` est une matrice 2D dont
  chaque ligne est allouée séparément.)*

## Étudiant 2

- **P1** — Que se passerait-il si on oubliait d'initialiser `somme` à 0
  avant la boucle ?
  *(Comportement indéfini — valeur résiduelle de la pile.)*
- **P2** — Pourquoi `sommes_colonnes` et `sommes_colonnes_par_colonne`
  donnent-elles le même résultat malgré un ordre de parcours différent ?
  *(L'addition est commutative — peu importe l'ordre, la somme totale est
  la même.)*
- **P3** — `capteur_afficher` prend `capteur_t *capteur` plutôt que par
  valeur, même si elle ne fait que LIRE le capteur. Est-ce strictement
  nécessaire ?
  *(Non, mais évite de copier toute la struct à chaque appel.)*
- **P4** — Que retourne `malloc` si l'allocation échoue ? Que fait la
  fonction dans ce cas ?
  *(Retourne `NULL`. La fonction vérifie et retourne `NULL` à son tour.)*
- **P5** — Aurait-on pu allouer TOUTE la matrice en un seul `malloc`
  (bloc contigu) ? Quel calcul d'indice faudrait-il faire à la main ?
  *(`bloc[i * colones + j]` au lieu de `matrice[i][j]`.)*
- **P6** — Faut-il appeler `free()` quelque part dans ce programme ?
  Pourquoi ?
  *(Non — tableau statique/local sur la pile, libéré automatiquement.)*
- **Station** — Que représente `nb_mesures` dans la structure `station_t` ?
  *(Le nombre de colonnes de `historique`, fixé une fois à la création.)*

## Étudiant 3

- **P1** — Pourquoi la boucle de `indice_max` commence-t-elle à `i = 1` et
  non `i = 0` ?
  *(Comparer l'élément 0 avec lui-même est inutile.)*
- **P2** — Une colonne est-elle contiguë en mémoire ? Pourquoi (pas) ?
  *(Non — les éléments sont espacés de `NB_COLONNES` cases.)*
- **P3** — Si on donne un `titre` de exactement 19 caractères, est-il
  tronqué ? Et à 20 caractères ?
  *(19 tient exactement (19 + `'\0'` = 20). 20 serait tronqué.)*
- **P4** — Que se passerait-il si on écrivait `tableau[0] = 1;` sans
  vérifier si `tableau` est `NULL` après le `malloc` ?
  *(Comportement indéfini — crash généralement.)*
- **P5** — Que se passe-t-il si l'allocation de la 3e ligne échoue ? Que
  fait-on des deux premières lignes déjà allouées ?
  *(On les libère avant de retourner `NULL`, pour éviter une fuite.)*
- **P6** — Pour 5 points, combien de distances "consécutives" calcule-t-on,
  et pourquoi la boucle s'arrête-t-elle à `NB_POINTS - 1` ?
  *(4 distances — sinon accès hors limites à `points[NB_POINTS]`.)*
- **Station** — Pourquoi `station_creer` retourne-t-elle un `station_t` PAR
  VALEUR plutôt que par pointeur ?
  *(Choix valide ; les pointeurs internes copiés continuent de pointer vers
  la même mémoire allouée.)*

## Étudiant 4

- **P1** — Si deux éléments ont la même valeur maximale, lequel des deux
  indices `indice_max` retourne-t-elle ? Pourquoi ?
  *(Le premier trouvé, car la condition est `>` strict.)*
- **P2** — Pourquoi `resultats_lignes` et `resultats_colonnes` n'ont pas la
  même taille ?
  *(Une case par ligne (`LIGNES`) vs une case par colonne
  (`NB_COLONNES`).)*
- **P3** — Qu'est-ce qui empêche `capteur2` d'être affecté quand on modifie
  `capteur1->titre` directement ?
  *(Chaque `capteur_t` a son propre tableau `titre` physiquement séparé en
  mémoire.)*
- **P4** — Que se passe-t-il si on oublie d'appeler `free(tableau)` dans le
  `main` ? Le programme plante-t-il immédiatement ?
  *(Non — fuite mémoire silencieuse, le programme continue de tourner.)*
- **P5** — Pourquoi doit-on libérer chaque ligne AVANT le tableau de
  pointeurs, et pas l'inverse ?
  *(Sinon on perd les adresses des lignes — fuite mémoire irrécupérable.)*
- **P6** — Pourquoi `distance` reçoit-elle des pointeurs plutôt que les
  points par valeur ?
  *(Pour éviter de copier la struct à chaque appel.)*
- **Station** — Que se passerait-il si on faisait `station_t s2 = s1;`
  après avoir créé `s1` ?
  *(Copie superficielle — danger de double `free`.)*

## Étudiant 5

- **P1** — Que retournerait `indice_max` si `n = 0` ? Est-ce un résultat
  utilisable ?
  *(Retournerait 0 sans jamais vérifier `tab[0]` — trompeur/invalide.)*
- **P2** — Qu'est-ce que `NB_COLONNES` et pourquoi l'a-t-on défini avec
  `#define` plutôt que d'écrire `5` partout ?
  *(Évite de répéter un nombre magique — un seul changement suffit.)*
- **P3** — Pourquoi cette fonction (`capteur_mise_a_jour`) ne prend-elle
  que la nouvelle valeur en paramètre ?
  *(Seule la valeur mesurée change dans le temps.)*
- **P4** — Que se passe-t-il si on oublie d'appeler `free(tableau)` dans le
  `main` ? Le programme plante-t-il immédiatement ?
  *(Non — fuite mémoire silencieuse, le programme continue de tourner.)*
- **P5** — Pourquoi passe-t-on `colones` à `creer_matrice` mais pas à
  `liberer_matrice` ?
  *(Chaque ligne a été allouée indépendamment, `free()` sait combien
  libérer sans qu'on le lui redise.)*
- **P6** — Comment est calculée la distance totale ? Pourquoi ne
  recalcule-t-on pas toutes les distances une deuxième fois ?
  *(Accumulée directement dans la boucle (`distance_totale += d`).)*
- **Station** — Pourquoi initialise-t-on `station.capteurs` et
  `station.historique` à `NULL` avant même de tenter les allocations ?
  *(Pour que l'appelant puisse détecter un échec facilement.)*

## Étudiant 6

- **P1** — Pourquoi utilise-t-on `if (i > 0)` avant d'imprimer l'espace
  dans `afficher_tableau` ?
  *(Pour ne pas mettre d'espace avant le premier élément.)*
- **P2** — En mémoire, à combien de `int` de distance se trouve `mat[1][0]`
  par rapport à `mat[0][0]` ?
  *(`NB_COLONNES` (5) `int` plus loin.)*
- **P3** — Que se passe-t-il si le `titre` fourni fait plus de 19
  caractères ?
  *(Il est tronqué à 19 caractères + le `'\0'` final.)*
- **P4** — Que se passerait-il si on appelait `free(tableau)` deux fois de
  suite ?
  *(Comportement indéfini — "double free".)*
- **P5** — Combien d'appels à `malloc` sont nécessaires pour créer une
  matrice de 5 lignes ? Pourquoi ce nombre ?
  *(6 : un pour le tableau de pointeurs, un par ligne.)*
- **P6** — Pourquoi peut-on (ou devrait-on) utiliser `const point2D_t *`
  plutôt que `point2D_t *` pour les paramètres de `distance` ?
  *(La fonction ne fait que LIRE les points — documente l'intention.)*
- **Station** — Si l'allocation de la ligne 3 (sur 5) de `historique`
  échoue, que fait le code avant de retourner ?
  *(Libère les lignes 0-2, puis `historique`, puis `capteurs`, dans cet
  ordre précis.)*

## Étudiant 7

- **P1** — Que produirait `printf("%d ", tab[i]);` sans condition, à la
  place de l'approche actuelle ?
  *(Un espace en trop après le dernier élément.)*
- **P2** — Pourrait-on écrire une seule fonction générique pour
  `sommes_lignes` et `sommes_colonnes` ? Pourquoi deux fonctions séparées ?
  *(Possible mais moins clair — deux fonctions nommées sont plus simples à
  utiliser correctement.)*
- **P3** — Qu'est-ce qu'une "copie profonde" ? Pourquoi est-ce nécessaire
  ici ?
  *(Copier le CONTENU d'une chaîne, pas seulement son adresse — le capteur
  doit posséder sa propre copie.)*
- **P4** — Pourquoi `main.c` inclut-il `"../probleme1/probleme1.h"` au lieu
  de réécrire une fonction d'affichage ?
  *(L'énoncé permet explicitement de réutiliser des fonctions des numéros
  précédents.)*
- **P5** — Est-ce que `matrice[0]` et `matrice[1]` sont nécessairement côte
  à côte en mémoire ? Pourquoi ?
  *(Non — chaque ligne est allouée par un `malloc` séparé.)*
- **P6** — Que calcule exactement `dx * dx + dy * dy` avant le `sqrtf` ?
  *(Le carré de la distance, via Pythagore.)*
- **Station** — Pourquoi les 3 premiers capteurs sont-ils codés en dur
  plutôt que dans une boucle générique ?
  *(Plus simple et lisible pour un nombre fixe et connu de capteurs
  "spéciaux".)*

## Étudiant 8

- **P1** — Peux-tu décrire une autre façon d'obtenir le même résultat pour
  `afficher_tableau` ?
  *(Ex. boucler jusqu'à `n-1` puis afficher le dernier séparément.)*
- **P2** — Que se passerait-il si on appelait `afficher_matrice(matrice,
  0)` ?
  *(La boucle externe ne s'exécute jamais, rien ne s'imprime.)*
- **P3** — Que se passerait-il si `titre` était déclaré `char *titre` et
  qu'on faisait `capteur->titre = titre_recu;` ?
  *(Le capteur ne ferait que pointer vers la mémoire du paramètre reçu —
  danger si cette mémoire change ou est libérée.)*
- **P4** — Que retourne `creer_tableau(0)` ? Est-ce forcément `NULL` ?
  *(`malloc(0)` peut retourner `NULL` ou un pointeur unique
  non-déréférençable — comportement défini par l'implémentation.)*
- **P5** — Pourquoi ne peut-on pas réutiliser `afficher_matrice(int
  mat[][5], ...)` du problème 2 pour afficher une matrice créée par
  `creer_matrice` ?
  *(Types incompatibles — bloc contigu vs tableau de pointeurs.)*
- **P6** — Pourquoi utilise-t-on `sqrtf` plutôt que `sqrt` ?
  *(Cohérent avec des champs `x`/`y` en `float`.)*
- **Station** — Que se passe-t-il si `station_creer` est appelée avec
  `nb_capteurs = 1` ?
  *(Seul le capteur température est initialisé.)*

## Étudiant 9

- **P1** — Pourquoi le tableau contient-il deux fois la valeur `23` dans
  cette solution ?
  *(Pour observer le comportement de `indice_max` en cas d'égalité.)*
- **P2** — Pourquoi ne peut-on pas déclarer `int mat[][]` sans dimension et
  laisser le nombre de colonnes se déduire à l'exécution ?
  *(Le compilateur a besoin d'un nombre de colonnes connu À LA
  COMPILATION.)*
- **P3** — Pourquoi passe-t-on `&capteur1` (une adresse) aux fonctions
  plutôt que `capteur1` directement ?
  *(Pour que les fonctions puissent MODIFIER le capteur original.)*
- **P4** — Quelle est la différence entre `malloc` et `calloc` ? Est-ce que
  `calloc` aurait changé quelque chose ici ?
  *(`calloc` met la mémoire à zéro — aucune différence observable ici.)*
- **P5** — Pourquoi passe-t-on `colones` à `creer_matrice` mais pas à
  `liberer_matrice` ?
  *(`free()` sait combien d'octets libérer sans qu'on le lui redise.)*
- **P6** — Pourquoi `distance` reçoit-elle des pointeurs plutôt que les
  points par valeur ?
  *(Pour éviter de copier la struct à chaque appel.)*
- **Station** — D'où vient la taille attendue du tableau `valeurs[]` passé
  à `station_enregistrer_mesure` ?
  *(Responsabilité de l'appelant — pas vérifiée par la fonction.)*

## Étudiant 10

- **P1** — Pourquoi les fonctions prennent-elles `n` en paramètre plutôt
  que de le calculer avec `sizeof` ?
  *(Un tableau passé en paramètre dégénère en pointeur.)*
- **P2** — Pourquoi utilise-t-on `%4d` plutôt que `%d` dans
  `afficher_matrice` ?
  *(Pour garder les colonnes alignées visuellement.)*
- **P3** — Que se passerait-il si on appelait `capteur_init(&c, 1, NULL,
  0.0f, "C", 1)` ?
  *(Comportement indéfini.)*
- **P4** — Pourquoi `free(tableau)` ne met-il pas automatiquement
  `tableau` à `NULL` après l'appel ?
  *(`free()` ne modifie pas la variable pointeur elle-même — danger de
  use-after-free si on continue de l'utiliser.)*
- **P5** — Que fait exactement `rand() % (max - min + 1) + min` ?
  *(Décale l'intervalle `[0, max-min]` vers `[min, max]`.)*
- **P6** — Que se passerait-il concrètement si la boucle utilisait par
  erreur `i < NB_POINTS` au lieu de `i < NB_POINTS - 1` ?
  *(Accès hors limites à `points[NB_POINTS]`.)*
- **Station** — Pourquoi appelle-t-on `capteur_mise_a_jour` en plus
  d'écrire dans `historique` ?
  *(Pour que `derniere_valeur` reflète toujours la mesure la plus
  récente.)*

## Étudiant 11

- **P1** — Trace `indice_max` sur `{5, 3, 8, 8, 1}` : quel indice est
  retourné et pourquoi ?
  *(Indice 2 — le premier `8` trouvé.)*
- **P2** — Pourquoi doit-on préciser le nombre de colonnes dans la
  signature mais pas le nombre de lignes ?
  *(Le compilateur a besoin des colonnes pour calculer l'adresse de
  `mat[i][j]`.)*
- **P3** — Pourquoi a-t-on choisi `snprintf` plutôt que `strncpy` pour
  copier `titre` et `unite` ?
  *(`snprintf` garantit toujours une chaîne terminée par `'\0'`.)*
- **P4** — Si `main.c` faisait `int *tableau2 = tableau;` puis `free` sur
  les deux pointeurs, quel problème obtiendrait-on ?
  *(Double free — les deux pointeurs visent la même mémoire.)*
- **P5** — Où et combien de fois `srand()` est-elle appelée ? Pourquoi cet
  endroit précis ?
  *(Une seule fois, dans `main()`, avant tout appel à `rand()`.)*
- **P6** — Comment modifierais-tu `main` pour calculer aussi la distance
  entre le PREMIER et le DERNIER point ?
  *(Appel supplémentaire `distance(&points[0], &points[NB_POINTS-1])`.)*
- **Station** — Pourquoi ne peut-on pas réutiliser directement `indice_max`
  du problème 1 pour `station_indice_valeur_max` ?
  *(`indice_max` travaille sur `int[]`, `historique` contient des `float` —
  pas de généricité en C.)*

## Étudiant 12

- **P1** — Quelle est la complexité (grand O) de `somme_tableau` et de
  `indice_max` ?
  *(O(n) pour les deux.)*
- **P2** — Pourquoi doit-on préciser le nombre de colonnes mais pas le
  nombre de lignes ?
  *(Colonnes nécessaires pour calculer l'adresse de `mat[i][j]`.)*
- **P3** — Pourquoi `capteur_mise_a_jour` ne prend-elle que la nouvelle
  valeur en paramètre ?
  *(Les autres champs sont fixes après l'initialisation.)*
- **P4** — L'énoncé donne `int *creer_tableau()` sans paramètre visible.
  Pourquoi faut-il quand même un paramètre `n` ?
  *(`n` n'est connu qu'à l'exécution.)*
- **P5** — Que se passe-t-il dans cette implémentation si l'allocation de
  la 3e ligne échoue ?
  *(On libère les 2 premières lignes déjà allouées avant de retourner
  `NULL`.)*
- **P6** — Que calcule exactement `dx * dx + dy * dy` avant le `sqrtf` ?
  *(Le carré de la distance, via Pythagore.)*
- **Station** — Que se passerait-il si on appelait
  `station_moyenne_capteur` avant d'avoir enregistré la moindre mesure ?
  *(La moyenne inclurait des valeurs non initialisées — résultat
  incohérent.)*

## Étudiant 13

- **P1** — Ces deux fonctions (`somme_tableau`, `indice_max`) parcourent
  chacune tout le tableau séparément. Pourrait-on les fusionner en une
  seule boucle ?
  *(Oui, techniquement — mais séparées, c'est plus lisible.)*
- **P2** — Pourquoi `sommes_lignes` n'a pas besoin d'initialiser
  `resultats[i]` à l'avance ?
  *(On accumule dans une variable locale, puis on assigne une seule fois à
  la fin.)*
- **P3** — Pourquoi cette fonction ne prend-elle que la nouvelle valeur en
  paramètre (`capteur_mise_a_jour`) ?
  *(Seule la valeur mesurée change dans le temps.)*
- **P4** — Que retourne `malloc` si l'allocation échoue ? Que fait la
  fonction dans ce cas ?
  *(`NULL` — la fonction propage `NULL` à son tour.)*
- **P5** — Pourquoi doit-on libérer chaque ligne AVANT de libérer le
  tableau de pointeurs, et pas l'inverse ?
  *(Sinon on perd les adresses des lignes — fuite irrécupérable.)*
- **P6** — Pourquoi faut-il `#include <math.h>` et `-lm` dans le Makefile ?
  *(`<math.h>` déclare `sqrtf` ; `-lm` lie la bibliothèque mathématique.)*
- **Station** — Pourquoi `historique` a besoin d'une boucle de `free` alors
  que `capteurs` n'a besoin que d'un seul `free` ?
  *(`historique` est un tableau de pointeurs vers des lignes séparées ;
  `capteurs` est un bloc unique.)*

## Étudiant 14

- **P1** — Peut-on réutiliser `somme_tableau` telle quelle pour un tableau
  de `float` ?
  *(Non — le C n'a pas de généricité, il faudrait une fonction séparée.)*
- **P2** — Pourquoi `sommes_colonnes` doit-elle initialiser `resultats[j]`
  à 0 avant la boucle principale ?
  *(On accumule au fil de plusieurs lignes, contrairement à
  `sommes_lignes`.)*
- **P3** — Que se passerait-il si `titre` était déclaré `char *titre` et
  qu'on faisait `capteur->titre = titre_recu;` ?
  *(Danger — le capteur pointerait vers de la mémoire externe.)*
- **P4** — Pourquoi utilise-t-on `malloc(n * sizeof(int))` plutôt que
  `malloc(n)` ?
  *(`malloc` prend des octets, pas des éléments.)*
- **P5** — Pourquoi ne peut-on pas réutiliser `afficher_matrice` du
  problème 2 pour une matrice créée par `creer_matrice` ?
  *(Types incompatibles.)*
- **P6** — Pourquoi peut-on (ou devrait-on) utiliser `const point2D_t *`
  plutôt que `point2D_t *` ?
  *(La fonction ne fait que LIRE les points.)*
- **Station** — Pourquoi `capteurs` est un tableau plat alors que
  `historique` est un tableau de pointeurs ?
  *(Tailles fixes vs matrice 2D allouée ligne par ligne.)*

## Étudiant 15

- **P1** — Que se passerait-il si on appelait `afficher_tableau(tableau,
  -1)` ?
  *(La boucle ne s'exécute jamais — comme `n = 0`.)*
- **P2** — Qu'est-ce que `NB_COLONNES` et pourquoi `#define` plutôt que `5`
  partout ?
  *(Évite de répéter un nombre magique.)*
- **P3** — Après avoir mis à jour `capteur1`, pourquoi `capteur2` reste-t-il
  inchangé ?
  *(Structs indépendantes en mémoire.)*
- **P4** — Pourquoi `main.c` inclut-il `probleme1.h` plutôt que de
  réécrire l'affichage ?
  *(Réutilisation explicitement permise par l'énoncé.)*
- **P5** — Décris, étape par étape, ce que fait `matrice[i][j]`.
  *(`matrice[i]` est `int *`, déréférencé à `j` pour un `int`.)*
- **P6** — `distance` retourne un `float`. Que se passerait-il si `x`/`y`
  étaient des `int` dans `point2D_t` ?
  *(Perte de précision, risque d'overflow.)*
- **Station** — Pourquoi `station_creer` retourne-t-elle un `station_t` PAR
  VALEUR plutôt que par pointeur ?
  *(Choix valide — les pointeurs internes restent partagés.)*

## Étudiant 16

- **P1** — Que retourne la fonction si `n = 0` (`somme_tableau`) ?
  *(0.)*
- **P2** — En mémoire, à combien de `int` de distance se trouve `mat[1][0]`
  par rapport à `mat[0][0]` ?
  *(`NB_COLONNES` (5) `int` plus loin.)*
- **P3** — Que se passe-t-il si le `titre` fourni fait plus de 19
  caractères ?
  *(Tronqué à 19 + `'\0'`.)*
- **P4** — Qui est responsable de libérer la mémoire retournée par
  `creer_tableau` ?
  *(L'appelant.)*
- **P5** — Est-ce que `matrice[0]` et `matrice[1]` sont nécessairement côte
  à côte en mémoire ?
  *(Non.)*
- **P6** — Pourquoi utilise-t-on un tableau statique ici plutôt que
  dynamique ?
  *(Taille connue et fixe à la compilation.)*
- **Station** — Que se passe-t-il si `station_enregistrer_mesure` est
  appelée avec `idx_mesure >= s->nb_mesures` ?
  *(Écriture hors limites — comportement indéfini.)*

## Étudiant 17

- **P1** — Que se passerait-il si on oubliait d'initialiser `somme` à 0 ?
  *(Comportement indéfini.)*
- **P2** — Pourquoi `sommes_colonnes` et `sommes_colonnes_par_colonne`
  donnent-elles le même résultat ?
  *(Ordre de parcours différent, résultat identique.)*
- **P3** — `capteur_afficher` prend un pointeur même si elle ne fait que
  LIRE — strictement nécessaire ?
  *(Non, mais plus efficace.)*
- **P4** — Que se passe-t-il si on oublie `free(tableau)` ?
  *(Fuite mémoire silencieuse, pas de crash immédiat.)*
- **P5** — Combien d'appels à `malloc` pour une matrice de 5 lignes ?
  *(6.)*
- **P6** — Faut-il appeler `free()` quelque part dans ce programme ?
  *(Non — tableau statique/local.)*
- **Station** — `station_liberer` ne met pas les pointeurs à `NULL` après
  libération — quel danger si elle était appelée deux fois ?
  *(Double free.)*

## Étudiant 18

- **P1** — Pourquoi la boucle de `indice_max` commence-t-elle à `i = 1` ?
  *(Comparer l'élément 0 à lui-même est inutile.)*
- **P2** — Une colonne est-elle contiguë en mémoire ?
  *(Non.)*
- **P3** — Si on donne un `titre` de exactement 19 caractères, est-il
  tronqué ?
  *(Non, il tient exactement.)*
- **P4** — Que se passerait-il en écrivant `tableau[0] = 1;` sans vérifier
  `NULL` ?
  *(Comportement indéfini, crash probable.)*
- **P5** — Pourquoi passe-t-on `colones` à `creer_matrice` mais pas à
  `liberer_matrice` ?
  *(`free()` n'a pas besoin de cette information.)*
- **P6** — Que calcule `dx * dx + dy * dy` ?
  *(Le carré de la distance.)*
- **Station** — Deux stations créées indépendamment auraient-elles les
  mêmes adresses mémoire pour capteurs/historique ?
  *(Non — `malloc` retourne des adresses différentes en général.)*

## Étudiant 19

- **P1** — Si deux éléments ont la même valeur maximale, lequel des deux
  indices `indice_max` retourne-t-elle ?
  *(Le premier trouvé.)*
- **P2** — Pourquoi `sommes_lignes` n'a pas besoin d'initialiser
  `resultats[i]` à l'avance ?
  *(Accumulation locale puis assignation unique.)*
- **P3** — Pourquoi passe-t-on `&capteur1` plutôt que `capteur1` ?
  *(Pour permettre la modification du capteur original.)*
- **P4** — Que se passerait-il avec `free(tableau)` appelé deux fois de
  suite ?
  *(Double free — comportement indéfini.)*
- **P5** — Que produirait le programme sans `srand()` ?
  *(Même séquence de valeurs à chaque exécution.)*
- **P6** — Pourquoi appelle-t-on `sqrtf` plutôt que de comparer directement
  les carrés des distances, quand on veut seulement COMPARER deux
  distances ?
  *(Comparer les carrés suffirait — mais ici on veut la vraie distance
  (affichage, somme totale).)*
- **Station** — Pourquoi `station_indice_valeur_max` et
  `station_moyenne_capteur` prennent-elles `s` par pointeur alors qu'elles
  ne modifient rien ?
  *(Éviter de copier toute la struct à chaque appel.)*

---

*Généré à partir de la banque complète dans `QUESTIONS_LAB2.md`. Pour une
question de secours si un.e étudiant.e a manifestement discuté avec un.e
voisin.e déjà passé.e, pige n'importe quelle autre question du même
problème dans `QUESTIONS_LAB2.md`.*
