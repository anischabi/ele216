# Questions de vérification — Problème 4 (Tableaux dynamiques : allocation et libération)

But : vérifier que l'étudiant.e comprend son code, pas juste qu'il fonctionne.
Piger 2-3 questions selon le temps disponible.

## Signature et allocation

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

## Propriété de la mémoire / responsabilité du `free`

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

## Réutilisation de code

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
