# Labo 2 — Banque de questions de vérification (tous les problèmes)

Compilation de toutes les questions de vérification (`QUESTIONS.md` de chaque
sous-dossier de `lab2/`), banque supplémentaire incluse. But : vérifier que
l'étudiant.e comprend son code, pas juste qu'il fonctionne (voir
`consigne_charger_lab.md`). Piger 2-3 questions par étudiant.e selon le temps
disponible ; puiser dans la banque supplémentaire pour varier les questions
d'un.e étudiant.e à l'autre.

---

## Problème 1 — Tableaux 1D

### `somme_tableau`

- Que retourne la fonction si `n = 0` ? Pourquoi ?
  *(Réponse attendue : 0, car `somme` est initialisée à 0 et la boucle ne
  s'exécute jamais.)*
- Que se passerait-il si on oubliait d'initialiser `somme` à 0 avant la
  boucle ?
  *(Comportement indéfini — la variable contiendrait une valeur aléatoire
  résiduelle de la pile.)*

### `indice_max`

- Pourquoi la boucle commence-t-elle à `i = 1` et non `i = 0` ?
  *(Comparer l'élément 0 avec lui-même est inutile, on suppose déjà qu'il
  est le max au départ.)*
- Si deux éléments ont la même valeur maximale, lequel des deux indices est
  retourné ? Pourquoi ?
  *(Le premier trouvé, car la condition est `>` strict — un `>=` donnerait
  le dernier trouvé à la place.)*
- Que retournerait la fonction si `n = 0` ? Est-ce un résultat utilisable ?
  *(Retournerait 0 sans jamais vérifier `tab[0]`, ce qui est trompeur/invalide
  puisque le tableau est vide — bon point pour un.e étudiant.e à l'aise.)*

### `afficher_tableau`

- Pourquoi utilise-t-on `if (i > 0)` avant d'imprimer l'espace ?
  *(Pour ne pas mettre d'espace avant le premier élément — l'énoncé
  interdit un espace après le dernier, cette approche l'évite
  naturellement.)*
- Que produirait `printf("%d ", tab[i]);` sans condition, à la place de
  l'approche actuelle ?
  *(Un espace en trop après le dernier élément — ne respecte pas
  l'énoncé.)*
- Peux-tu décrire une autre façon d'obtenir le même résultat ?
  *(Ex. boucler jusqu'à `n - 1` avec espace après chaque élément, puis
  afficher le dernier élément séparément après la boucle.)*

### `main` / général

- Pourquoi le tableau contient-il deux fois la valeur `23` dans cette
  solution ? Qu'est-ce que ça permet d'observer ?
- Pourquoi les fonctions prennent-elles `n` en paramètre plutôt que de le
  calculer à l'intérieur (ex. avec `sizeof`) ?
  *(Un tableau passé en paramètre "dégénère" en pointeur — `sizeof` ne
  donnerait plus la taille du tableau original à l'intérieur de la
  fonction.)*

### Banque supplémentaire

- Trace à la main `indice_max` sur `{5, 3, 8, 8, 1}` : quel indice est
  retourné et pourquoi ?
  *(Indice 2 — le premier `8` trouvé, car la comparaison est `>` strict.)*
- Quelle est la complexité (grand O) de `somme_tableau` et de `indice_max` ?
  Pourrait-on faire mieux ?
  *(O(n) pour les deux — un seul passage nécessaire, on ne peut pas faire
  mieux puisqu'il faut lire chaque élément au moins une fois.)*
- Ces deux fonctions parcourent chacune tout le tableau séparément.
  Pourrait-on calculer la somme ET l'indice du max dans une seule boucle ?
  Pourquoi ce choix (séparées) est-il quand même raisonnable ici ?
  *(Oui, techniquement fusionnable en une seule boucle. Séparées, c'est
  plus lisible et chaque fonction a une seule responsabilité — un compromis
  lisibilité/performance tout à fait défendable pour un tableau de cette
  taille.)*
- Si on voulait une version de `somme_tableau` pour un tableau de `float`,
  peut-on réutiliser la même fonction telle quelle ? Pourquoi (pas) ?
  *(Non — le C n'a pas de généricité (pas de templates comme en C++), il
  faudrait réécrire une fonction séparée avec le type `float`.)*
- Que se passerait-il si on appelait `afficher_tableau(tableau, -1)` ?
  *(La condition `i < n` avec `n` négatif est fausse dès le départ, la
  boucle ne s'exécute jamais — comportement similaire à `n = 0`, mais un `n`
  négatif n'a normalement aucun sens pour une taille de tableau.)*

---

## Problème 2 — Tableaux 2D

### Signature `int mat[][5]`

- Pourquoi doit-on préciser le nombre de colonnes dans la signature
  (`int mat[][5]`) mais pas le nombre de lignes ?
  *(Un tableau 2D est stocké comme un seul bloc contigu en mémoire, rangé
  ligne par ligne. Le compilateur a besoin du nombre de colonnes pour
  calculer l'adresse de `mat[i][j]` via l'arithmétique de pointeurs. Le
  nombre de lignes n'affecte pas ce calcul, donc il peut rester un
  paramètre normal.)*
- Que se passerait-il si on essayait d'écrire `int mat[][]` sans préciser de
  nombre de colonnes ?
  *(Erreur de compilation — le nombre de colonnes est obligatoire pour tout
  sauf la première dimension.)*

### `afficher_matrice`

- Pourquoi utilise-t-on `%4d` plutôt que `%d` ou `%d ` ?
  *(Pour garder les colonnes alignées visuellement même si les nombres ont
  un nombre de chiffres différent — largeur de champ fixe.)*

### `sommes_lignes`

- Pourquoi peut-on calculer la somme d'une ligne en une seule boucle simple,
  sans initialiser `resultats[i]` à l'avance ?
  *(On accumule dans une variable locale `somme`, puis on l'assigne une
  seule fois à la fin — pas besoin d'initialiser `resultats[i]` séparément.)*

### `sommes_colonnes` (deux versions dans le code)

- Quelle est la différence entre `sommes_colonnes` et
  `sommes_colonnes_par_colonne` ? Donnent-elles le même résultat ?
  *(Ordre de parcours différent — l'une est contiguë en mémoire (ligne par
  ligne), l'autre saute de `NB_COLONNES` cases à chaque itération (colonne
  par colonne). Même résultat dans les deux cas.)*
- Pourquoi `sommes_colonnes` (version ligne par ligne) doit-elle initialiser
  `resultats[j]` à 0 avant la boucle principale, contrairement à
  `sommes_lignes` ?
  *(Parce qu'on accumule dans `resultats[j]` au fil de plusieurs passages
  (une ligne à la fois), plutôt que de calculer la somme complète d'une
  colonne avant de l'assigner en une seule fois.)*
- Une colonne est-elle contiguë en mémoire ? Pourquoi (pas) ?
  *(Non — les éléments `mat[0][j]`, `mat[1][j]`, ... sont espacés de
  `NB_COLONNES` cases dans le bloc mémoire contigu de la matrice.)*

### `main` / général

- Pourquoi `resultats_lignes` et `resultats_colonnes` n'ont-ils pas la même
  taille ?
  *(`resultats_lignes` a une case par ligne (taille `LIGNES`),
  `resultats_colonnes` a une case par colonne (taille `NB_COLONNES`) — les
  deux valeurs ne sont pas nécessairement égales.)*
- Qu'est-ce que `NB_COLONNES` et pourquoi l'a-t-on défini avec `#define`
  plutôt que d'écrire `5` directement partout dans le code ?
  *(Évite de répéter un "nombre magique" à plusieurs endroits ; un seul
  changement suffit si la taille devait changer.)*

### Banque supplémentaire

- En mémoire, à combien de `int` de distance se trouve `mat[1][0]` par
  rapport à `mat[0][0]` ? Et `mat[2][3]` par rapport à `mat[0][0]` ?
  *(`mat[1][0]` est `NB_COLONNES` (5) `int` plus loin. `mat[2][3]` est
  `2*NB_COLONNES + 3 = 13` `int` plus loin — c'est exactement le calcul que
  fait le compilateur en coulisses.)*
- Que se passerait-il si on appelait `afficher_matrice(matrice, 0)` ?
  *(La boucle externe ne s'exécute jamais, rien ne s'imprime — pas
  d'erreur.)*
- Pourrait-on écrire une seule fonction générique `somme_ligne_ou_colonne`
  qui ferait les deux à la fois ? Pourquoi les auteurs ont-ils choisi deux
  fonctions séparées ?
  *(Possible mais plus complexe à lire/utiliser ; deux fonctions séparées
  avec un nom clair sont plus simples à comprendre et à appeler
  correctement.)*
- Si `lignes` passé à `sommes_lignes` est plus grand que le nombre réel de
  lignes initialisées dans `matrice`, que se passe-t-il ?
  *(Comportement indéfini — la fonction lirait de la mémoire hors des
  limites réelles du tableau, rien ne l'en empêche puisque `lignes` n'est
  pas vérifié.)*
- Pourquoi ne peut-on pas déclarer `int mat[][]` (sans aucune dimension) et
  laisser le nombre de colonnes se déduire automatiquement à l'exécution ?
  *(Le compilateur a besoin d'un nombre de colonnes CONNU À LA COMPILATION
  pour calculer l'adresse de `mat[i][j]` par arithmétique de pointeurs ;
  rien d'équivalent à un tableau 2D "flexible" n'existe pour les paramètres
  de fonction en C standard.)*

---

## Problème 3 — Structures

### Copie profonde (le cœur du problème)

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

### `snprintf` vs `strncpy`

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

### `capteur_mise_a_jour`

- Pourquoi cette fonction ne prend-elle que la nouvelle valeur en
  paramètre, et pas le titre ou l'unité ?
  *(Seule la valeur mesurée change dans le temps ; les autres champs sont
  fixes après l'initialisation, donc pas besoin de les repasser.)*

### `main` / général

- Pourquoi passe-t-on `&capteur1` (une adresse) aux fonctions plutôt que
  `capteur1` directement ?
  *(Pour que les fonctions puissent MODIFIER le capteur original ; passer
  la struct par valeur créerait une copie locale, et les modifications ne
  seraient pas visibles à l'extérieur de la fonction.)*
- Après avoir mis à jour `capteur1`, pourquoi `capteur2` reste-t-il
  inchangé ?
  *(Chaque capteur est une struct indépendante en mémoire ; mettre à jour
  l'un ne touche pas l'autre.)*

### Banque supplémentaire

- Que se passerait-il si on appelait `capteur_init(&c, 1, NULL, 0.0f, "C", 1)`
  (titre `NULL`) ?
  *(Comportement indéfini — `snprintf("%s", NULL)` n'est pas garanti sûr par
  le standard C, même si certaines libc l'affichent comme `"(null)"".)*
- `capteur_afficher` prend `capteur_t *capteur` plutôt que `capteur_t
  capteur` par valeur, même si elle ne fait que LIRE le capteur. Est-ce
  strictement nécessaire ? Pourquoi ce choix quand même ?
  *(Pas strictement nécessaire ici (on pourrait passer par valeur), mais
  éviter de copier toute la struct à chaque appel est plus efficace —
  surtout si la struct grossit. Un `const capteur_t *` documenterait
  encore mieux l'intention de lecture seule.)*
- Si on donne un `titre` de exactement 19 caractères (donc `TITRE_TAILLE -
  1`), est-il tronqué ? Et à 20 caractères ?
  *(19 caractères tient exactement (19 + le `'\0'` = 20 = `TITRE_TAILLE`),
  pas de troncature. 20 caractères serait tronqué à 19 + `'\0'`.)*
- Qu'est-ce qui empêche `capteur2` d'être affecté quand on modifie
  `capteur1->titre` directement (hypothétiquement, sans passer par les
  fonctions) ?
  *(Rien en soi — mais chaque `capteur_t` a son propre tableau `titre`
  physiquement séparé en mémoire, donc modifier celui de `capteur1` ne peut
  pas, par construction, toucher la mémoire de `capteur2`.)*

---

## Problème 4 — Tableaux dynamiques (allocation et libération)

### Signature et allocation

- L'énoncé donne `int *creer_tableau()` sans paramètre visible. Pourquoi
  faut-il quand même un paramètre `n` ?
  *(Le tableau doit contenir `n` entiers, et `n` n'est connu qu'à
  l'exécution — sans paramètre, la fonction ne saurait pas combien de
  mémoire allouer.)*
- Pourquoi utilise-t-on `malloc(n * sizeof(int))` plutôt que
  `malloc(n)` ?
  *(`malloc` prend un nombre d'OCTETS, pas d'éléments. `sizeof(int)` donne
  la taille en octets d'un `int` ; sans ça, on n'allouerait qu'une fraction
  de la mémoire nécessaire.)*
- Que retourne `malloc` si l'allocation échoue (mémoire insuffisante) ?
  Que fait la fonction dans ce cas ?
  *(Retourne `NULL`. La fonction vérifie ce cas et retourne `NULL` à son
  tour, plutôt que d'écrire dans un pointeur invalide.)*
- Que se passerait-il si on essayait d'écrire `tableau[0] = 1;` sans
  vérifier si `tableau` est `NULL` après le `malloc` ?
  *(Comportement indéfini — déréférencer un pointeur `NULL` cause
  généralement un crash (segfault).)*

### Propriété de la mémoire / responsabilité du `free`

- Qui est responsable de libérer la mémoire retournée par
  `creer_tableau` ? Pourquoi la fonction elle-même ne le fait-elle pas ?
  *(L'appelant (ici, `main`) — la fonction retourne le pointeur précisément
  pour que l'appelant puisse l'utiliser après le retour de la fonction ; le
  libérer à l'intérieur de `creer_tableau` rendrait le pointeur retourné
  invalide.)*
- Que se passe-t-il si on oublie d'appeler `free(tableau)` dans le `main` ?
  Le programme plante-t-il immédiatement ?
  *(Non — fuite mémoire silencieuse. Le programme continue de fonctionner,
  mais la mémoire allouée n'est jamais rendue au système tant que le
  programme tourne.)*
- Que se passerait-il si on appelait `free(tableau)` deux fois de suite ?
  *(Comportement indéfini — "double free", une erreur classique de gestion
  mémoire.)*

### Réutilisation de code

- Pourquoi `main.c` inclut-il `"../probleme1/probleme1.h"` au lieu de
  réécrire une fonction d'affichage ?
  *(L'énoncé permet explicitement de réutiliser des fonctions des numéros
  précédents ; `afficher_tableau` fait déjà exactement ce qu'il faut.)*
- `afficher_tableau` a été écrite pour un tableau statique au problème 1.
  Pourquoi fonctionne-t-elle aussi bien avec le tableau alloué
  dynamiquement ici ?
  *(La fonction prend un `int tab[]` (qui dégénère en `int *`) et un `n` —
  elle ne se soucie pas de savoir si la mémoire vient de la pile ou du tas,
  seulement qu'elle pointe vers `n` entiers valides.)*

### Banque supplémentaire

- Que retourne `creer_tableau(0)` ? Est-ce forcément `NULL` ?
  *(`malloc(0)` a un comportement défini par l'implémentation : elle peut
  retourner `NULL` OU un pointeur unique non-`NULL` qu'on ne doit pas
  déréférencer. Le code gère correctement le cas `NULL`, mais un pointeur
  valide pour 0 élément est aussi un résultat légitime.)*
- Quelle est la différence entre `malloc` et `calloc` ? Est-ce que
  `calloc(n, sizeof(int))` aurait changé quoi que ce soit d'observable
  ici ?
  *(`calloc` met la mémoire à zéro avant de la retourner, `malloc` non. Ici
  ça n'aurait rien changé d'observable puisque la boucle écrase chaque case
  avec `i * 2` avant toute lecture.)*
- Pourquoi `free(tableau)` ne met-il pas automatiquement `tableau` à `NULL`
  après l'appel ? Quel danger ça représente si on continue d'utiliser
  `tableau` après le `free` ?
  *(`free()` ne fait que rendre la mémoire au système — elle ne connaît ni
  ne modifie la variable pointeur elle-même. Continuer à utiliser
  `tableau` après (le déréférencer, le relire) est un "use-after-free",
  comportement indéfini.)*
- Si `main.c` faisait `int *tableau2 = tableau;` juste avant le `free`, puis
  appelait `free(tableau2)` en plus de `free(tableau)`, quel problème
  obtiendrait-on ?
  *(Double free — les deux pointeurs visent la même mémoire, la libérer
  deux fois est un comportement indéfini classique.)*

---

## Problème 5 — Tableaux 2D dynamiques (allocation ligne par ligne)

*C'est le problème le plus important conceptuellement du labo — ne pas
hésiter à demander un dessin au tableau si l'étudiant.e bloque sur la
structure mémoire.*

### Matrice statique (problème 2) vs matrice dynamique (problème 5)

- Pourquoi ne peut-on pas réutiliser `afficher_matrice(int mat[][5], ...)`
  du problème 2 pour afficher une matrice créée par `creer_matrice` ici ?
  *(Les deux types ne sont pas compatibles : `int mat[][5]` attend un bloc
  contigu unique, alors que `int **matrice` est un tableau de pointeurs
  vers des blocs séparés. Le compilateur refuserait ou produirait un
  comportement incorrect si on essayait de les mélanger.)*
- Est-ce que `matrice[0]` et `matrice[1]` sont nécessairement côte à côte
  en mémoire dans la version dynamique ? Pourquoi ?
  *(Non — chaque ligne est allouée par un `malloc` séparé ; rien ne garantit
  que deux appels consécutifs à `malloc` retournent des adresses
  adjacentes.)*

### `creer_matrice` — allocation en cascade

- Combien d'appels à `malloc` sont nécessaires pour créer une matrice de
  5 lignes ? Pourquoi ce nombre ?
  *(6 : un pour le tableau de pointeurs, un par ligne (5 lignes).)*
- Que se passe-t-il dans cette implémentation si l'allocation de la 3e
  ligne échoue ? Que fait-on des deux premières lignes déjà allouées ?
  *(On libère les lignes déjà allouées avec succès (ici, les 2 premières)
  avant de retourner `NULL`, pour éviter une fuite mémoire partielle.)*
- Pourquoi passe-t-on `colones` à `creer_matrice` mais pas à
  `liberer_matrice` ?
  *(Chaque ligne a été allouée indépendamment avec `malloc(colones * ...)`
  — `free()` sait combien d'octets libérer sans qu'on le lui redise, cette
  information est gérée en interne par l'allocateur.)*

### `liberer_matrice` — ordre de libération

- Pourquoi doit-on libérer chaque ligne AVANT de libérer le tableau de
  pointeurs, et pas l'inverse ?
  *(Le tableau de pointeurs contient les seules adresses connues des
  lignes. Si on le libère en premier, on perd ces adresses — les lignes
  restent allouées en mémoire mais deviennent définitivement
  inaccessibles : une fuite mémoire irrécupérable pour le reste de
  l'exécution du programme.)*

### `remplir_matrice_aleatoire` et `srand`

- Où et combien de fois `srand()` est-elle appelée dans ce programme ?
  Pourquoi cet endroit et pas ailleurs ?
  *(Une seule fois, dans `main()`, avant tout appel à `rand()`. L'appeler
  plusieurs fois — par exemple à chaque itération d'une boucle — réinitialise
  le générateur à répétition et produit des valeurs beaucoup moins
  aléatoires.)*
- Que produirait le programme si on omettait complètement l'appel à
  `srand()` ?
  *(La même séquence de valeurs "aléatoires" à chaque exécution du
  programme — `rand()` se comporte alors comme si `srand(1)` avait été
  appelée.)*
- Que fait exactement `rand() % (max - min + 1) + min` ? Pourquoi ce calcul
  précis pour obtenir une valeur entre 20 et 80 ?
  *(`rand() % 61` donne une valeur entre 0 et 60 inclusivement, puis
  `+ 20` décale l'intervalle vers [20, 80].)*

### Banque supplémentaire

- Décris, étape par étape, ce que fait `matrice[i][j]` : que représente
  `matrice[i]` à lui seul, et quel type a-t-il ?
  *(`matrice` est `int **`. `matrice[i]` est `int *` — l'adresse de la
  ligne `i`, allouée séparément. `matrice[i][j]` déréférence ce pointeur de
  ligne à l'indice `j` pour obtenir un `int`.)*
- Aurait-on pu allouer TOUTE la matrice en un seul `malloc` (comme un bloc
  contigu `lignes * colones` entiers) au lieu d'un tableau de pointeurs ?
  Quel calcul d'indice faudrait-il alors faire à la main pour accéder à
  l'élément `(i, j)` ?
  *(Oui — `malloc(lignes * colones * sizeof(int))`, puis accéder via
  `bloc[i * colones + j]` au lieu de `matrice[i][j]`. Plus économe (une
  seule allocation/libération, mémoire contiguë) mais l'indexation devient
  manuelle plutôt que `[i][j]`.)*
- Si `creer_matrice(0, 8)` est appelée, que retourne la fonction ?
  *(`malloc(0 * sizeof(int*))` — même remarque que pour `malloc(0)` au
  problème 4 : peut être `NULL` ou un pointeur valide non-déréférençable ;
  la boucle d'allocation des lignes ne s'exécute jamais dans les deux cas.)*
- Dans `remplir_matrice_aleatoire`, que se passerait-il si `min > max` (ex.
  `min = 80, max = 20`) ?
  *(`max - min + 1` serait négatif ; le modulo avec un diviseur négatif a un
  comportement défini différemment en C mais donnerait presque certainement
  des valeurs incohérentes/hors de l'intervalle voulu — l'appelant est
  responsable de fournir `min <= max`.)*
- Pourquoi `liberer_matrice` ne vérifie-t-elle pas si `matrice` est `NULL`
  avant de boucler ? Est-ce un problème si `creer_matrice` a échoué et
  retourné `NULL` ?
  *(Oui potentiellement — appeler `liberer_matrice(NULL, lignes)`
  déréférencerait `NULL` dans la boucle `free(matrice[i])`. Bonne
  observation si l'étudiant.e le remarque : le code suppose que l'appelant
  vérifie le retour de `creer_matrice` avant d'appeler `liberer_matrice`.)*

---

## Problème 6 — Tableau de structures

### `point2D_t` et `distance`

- Pourquoi `distance` reçoit-elle des pointeurs (`point2D_t *a`) plutôt que
  les points directement par valeur (`point2D_t a`) ?
  *(Pour éviter de copier la struct à chaque appel — même si c'est une
  petite struct ici, c'est le bon réflexe. Passer par valeur fonctionnerait
  aussi, juste un peu moins efficace.)*
- Pourquoi peut-on (ou devrait-on) utiliser `const point2D_t *` plutôt que
  `point2D_t *` pour les paramètres de `distance` ?
  *(Parce que la fonction ne fait que LIRE les points, jamais les modifier
  — le `const` documente cette intention et permet au compilateur de
  détecter une modification accidentelle.)*
- Que calcule exactement `dx * dx + dy * dy` avant le `sqrtf` ?
  *(Le carré de la distance, via le théorème de Pythagore : la somme des
  carrés des différences en x et en y.)*
- Pourquoi utilise-t-on `sqrtf` plutôt que `sqrt` ?
  *(`sqrtf` est la version "float" de la fonction racine carrée, cohérente
  avec des champs `x`/`y` en `float` plutôt qu'en `double`.)*
- Pourquoi faut-il `#include <math.h>` et `-lm` dans le Makefile ?
  *(`<math.h>` déclare `sqrtf` ; `-lm` indique au linker d'inclure la
  bibliothèque mathématique où cette fonction est réellement implémentée.)*

### Tableau statique vs dynamique

- Pourquoi utilise-t-on un tableau statique ici, alors qu'on vient de voir
  l'allocation dynamique aux problèmes 4 et 5 ?
  *(La taille (5 points) est connue et fixe à la compilation — pas besoin
  de la flexibilité (ni de la complexité) d'un tableau alloué
  dynamiquement.)*
- Faut-il appeler `free()` quelque part dans ce programme ? Pourquoi ?
  *(Non — un tableau statique/local vit sur la pile et est automatiquement
  libéré à la fin de son scope (ici, à la fin de `main`), pas besoin de
  gestion manuelle.)*

### Boucle des distances consécutives

- Pour 5 points, combien de distances "consécutives" doit-on calculer ?
  Pourquoi la boucle s'arrête-t-elle à `NB_POINTS - 1` plutôt que
  `NB_POINTS` ?
  *(4 distances (0→1, 1→2, 2→3, 3→4). Boucler jusqu'à `NB_POINTS`
  accéderait à `points[NB_POINTS]`, qui n'existe pas — accès hors limites.)*
- Comment est calculée la distance totale ? Pourquoi ne recalcule-t-on pas
  toutes les distances une deuxième fois pour l'obtenir ?
  *(Elle est accumulée directement dans la boucle (`distance_totale += d`)
  au fur et à mesure que chaque distance est calculée — plus efficace que
  de tout recalculer séparément après coup.)*

### Banque supplémentaire

- Que se passerait-il concrètement (en mémoire) si la boucle utilisait par
  erreur `i < NB_POINTS` au lieu de `i < NB_POINTS - 1` ?
  *(À la dernière itération, `points[i + 1]` accéderait à `points[NB_POINTS]`,
  qui est hors des limites du tableau — comportement indéfini, souvent une
  valeur "poubelle" sans crash visible.)*
- Comment modifierais-tu `main` pour calculer aussi la distance entre le
  PREMIER et le DERNIER point (pas seulement les points consécutifs) ?
  *(Un appel supplémentaire : `distance(&points[0], &points[NB_POINTS - 1])`,
  en dehors de la boucle des distances consécutives.)*
- `distance` retourne un `float`. Que se passerait-il si `x`/`y` étaient des
  `int` dans `point2D_t` au lieu de `float` ? `dx * dx + dy * dy`
  changerait-il de comportement ?
  *(Avec des `int`, la soustraction et la multiplication resteraient
  entières — perte de précision pour des coordonnées non entières, et
  risque de dépassement (`overflow`) si les valeurs sont grandes ; il
  faudrait aussi utiliser `sqrt` sur un `double` calculé à partir des
  `int`.)*
- Pourquoi appelle-t-on `sqrtf` (qui coûte relativement cher en calcul) au
  lieu de comparer directement les carrés des distances (`dx*dx + dy*dy`)
  quand on veut seulement COMPARER deux distances entre elles ?
  *(Pour comparer deux distances, comparer leurs carrés suffit et évite le
  `sqrtf` — mais ici on veut la vraie distance (affichage, somme totale en
  unités réelles), donc le `sqrtf` est nécessaire.)*

---

## Mini-app Station météo (synthèse)

*Cette mini-app combine tout le labo — bon endroit pour vérifier la
compréhension d'ensemble, pas juste un détail isolé.*

### `station_t` — la structure globale

- Pourquoi `capteurs` est-il un tableau PLAT (`capteur_t *`) alors que
  `historique` est un tableau de pointeurs (`float **`) ?
  *(`capteur_t` a une taille fixe connue à la compilation, une seule
  allocation suffit (comme `creer_tableau` du problème 4). `historique`
  est une matrice 2D dont chaque ligne est allouée séparément (comme
  `creer_matrice` du problème 5).)*
- Que représente `nb_mesures` dans cette structure ?
  *(Le nombre de colonnes de `historique`, fixé une fois à la création par
  `station_creer` — pas un compteur qui change.)*

### `station_creer` — retour par valeur

- Pourquoi `station_creer` retourne-t-elle un `station_t` PAR VALEUR
  (`station_t station_creer(...)`) plutôt qu'un pointeur
  (`station_t *station_creer(...)`) ?
  *(Choix valide dans les deux cas ; ici, on suit le même patron que
  `creer_date_heure` du cours1 — la struct entière (avec ses pointeurs
  internes) est copiée dans la variable du `main`. Les pointeurs
  `capteurs`/`historique` copiés continuent de pointer vers la même
  mémoire allouée — pas de duplication du contenu.)*
- Que se passerait-il si on faisait `station_t s2 = s1;` après avoir créé
  `s1` ?
  *(Copie superficielle : `s2` et `s1` partageraient les mêmes pointeurs
  internes. Libérer l'une (`station_liberer(&s1)`) invaliderait l'autre —
  danger de double `free` ou d'utilisation après libération si on n'y
  prend pas garde.)*
- Pourquoi initialise-t-on `station.capteurs` et `station.historique` à
  `NULL` avant même de tenter les allocations ?
  *(Pour que si une allocation échoue et qu'on retourne tôt, les champs
  non encore alloués aient une valeur connue (`NULL`) plutôt
  qu'indéterminée — l'appelant peut alors vérifier facilement si la
  création a réussi.)*
- Si l'allocation de la ligne 3 (sur 5) de `historique` échoue, que fait
  le code avant de retourner ? Pourquoi cet ordre est-il important ?
  *(Libère les lignes 0 à 2 déjà allouées, puis le tableau de pointeurs
  `historique`, puis le tableau `capteurs` — dans cet ordre précis, pour
  n'oublier aucune fuite mémoire partielle.)*

### Initialisation des capteurs prédéfinis

- Pourquoi les 3 premiers capteurs sont-ils codés en dur (température,
  pression, humidité) plutôt que dans une boucle générique ?
  *(Plus simple et plus lisible pour un nombre fixe et connu de capteurs
  "spéciaux" ; on réutilise directement `capteur_init` du problème 3 pour
  chacun.)*
- Que se passe-t-il si `station_creer` est appelée avec `nb_capteurs = 1` ?
  *(Seul le capteur température est initialisé — les `if (nb_capteurs > N)`
  empêchent d'écrire dans des cases du tableau qui n'existent pas.)*

### `station_enregistrer_mesure`

- D'où vient la taille attendue du tableau `valeurs[]` passé en
  paramètre ? Comment la fonction le sait-elle ?
  *(Elle ne le "sait" pas directement — elle assume que `valeurs[]`
  contient au moins `s->nb_capteurs` éléments, dans le même ordre que
  `s->capteurs`. C'est une responsabilité de l'appelant, pas vérifiée par
  la fonction.)*
- Pourquoi appelle-t-on `capteur_mise_a_jour` en plus d'écrire dans
  `historique` ?
  *(Pour que le champ `derniere_valeur` de chaque capteur reflète toujours
  sa mesure la plus récente, même si on ne consulte que le capteur seul
  sans regarder l'historique.)*

### `station_moyenne_capteur` et `station_indice_valeur_max`

- Pourquoi ne peut-on pas réutiliser directement `indice_max` du
  problème 1 pour implémenter `station_indice_valeur_max` ?
  *(`indice_max` travaille sur un `int[]`, mais `historique` contient des
  `float`. Le C n'a pas de généricité (pas de templates) — il faut
  réécrire la même logique pour le type `float`.)*
- Que se passerait-il si on appelait `station_moyenne_capteur` avant
  d'avoir enregistré la moindre mesure ?
  *(Division par `nb_mesures`, qui vaut la capacité voulue — si aucune
  mesure n'a réellement été écrite, la moyenne inclurait des valeurs
  indéterminées (mémoire non initialisée) provenant de `historique`.)*

### `station_liberer`

- Pourquoi `historique` a-t-il besoin d'une boucle de `free` alors que
  `capteurs` n'a besoin que d'un seul `free` ?
  *(`historique` est un tableau de pointeurs vers des lignes allouées
  séparément — chacune doit être libérée individuellement, puis le
  tableau de pointeurs lui-même. `capteurs` est un bloc unique, alloué en
  une seule fois.)*
- Dans quel ordre doit-on libérer les lignes de `historique` et le
  tableau de pointeurs lui-même ? Pourquoi ?
  *(Les lignes d'abord, puis le tableau de pointeurs — l'inverse ferait
  perdre les adresses des lignes, qui resteraient allouées mais
  inaccessibles pour toujours.)*

### Banque supplémentaire

- Que retourne `station_moyenne_capteur` si `station_enregistrer_mesure`
  n'a jamais été appelée pour ce capteur ?
  *(La moyenne de valeurs non initialisées (mémoire "poubelle" retournée
  par `malloc`, jamais mise à zéro) — un résultat incohérent, pas une
  erreur visible.)*
- Que se passe-t-il si `station_enregistrer_mesure` est appelée avec
  `idx_mesure >= s->nb_mesures` (ex. `idx_mesure = 5` alors que
  `nb_mesures = 5`, donc les indices valides sont 0 à 4) ?
  *(Écriture hors limites dans `s->historique[i][idx_mesure]` —
  comportement indéfini ; rien dans le code ne vérifie `idx_mesure`.)*
- `station_liberer` ne met PAS `s->capteurs` ni `s->historique` à `NULL`
  après les avoir libérés. Quel danger cela représente-t-il si
  `station_liberer(&station)` était appelée deux fois par erreur ?
  *(Double free sur les deux — `free()` d'un pointeur déjà libéré est un
  comportement indéfini classique.)*
- Pourquoi `station_indice_valeur_max` et `station_moyenne_capteur`
  prennent-elles `s` par pointeur (`station_t *s`) alors qu'elles ne
  modifient rien dans la station ?
  *(Même raison qu'au problème 3 : éviter de copier toute la struct
  (incluant potentiellement de gros tableaux) à chaque appel — un `const
  station_t *` documenterait encore mieux l'intention de lecture seule.)*
- Si on appelait `station_creer(3, 5)` deux fois de suite dans le même
  `main` (deux stations indépendantes), les deux auraient-elles les mêmes
  adresses mémoire pour leurs capteurs/historique ? Pourquoi ce n'est pas
  grave ?
  *(Non, `malloc` retournerait très probablement des adresses différentes
  à chaque appel — ce n'est pas un problème car chaque `station_t` gère
  ses propres pointeurs indépendamment, sans lien entre les deux
  instances.)*
