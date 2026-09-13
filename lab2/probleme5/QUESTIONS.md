# Questions de vérification — Problème 5 (Tableaux 2D dynamiques : allocation ligne par ligne)

But : vérifier que l'étudiant.e comprend son code, pas juste qu'il fonctionne.
Piger 2-3 questions selon le temps disponible. C'est le problème le plus
important conceptuellement du labo — ne pas hésiter à demander un dessin au
tableau si l'étudiant.e bloque sur la structure mémoire.

## Matrice statique (problème 2) vs matrice dynamique (problème 5)

C'est la comparaison la plus importante à faire faire à l'étudiant.e — les
deux se manipulent avec la même syntaxe (`matrice[i][j]`) mais ont des
structures mémoire complètement différentes.

|  | `int mat[][5]` (problème 2) | `int **matrice` (problème 5) |
|---|---|---|
| Ce que c'est | Un seul bloc contigu | Un tableau de pointeurs, chacun menant à une ligne allouée séparément |
| Lignes contiguës en mémoire ? | Oui, toujours | Non — chaque ligne peut être n'importe où en RAM |
| Nombre de colonnes | Fixé à la compilation (dans la signature) | Choisi à l'exécution (paramètre de `creer_matrice`) |
| Nombre d'allocations `malloc` | Aucune (tableau statique/pile) ou 1 seule si alloué dynamiquement en bloc | `lignes + 1` (1 pour le tableau de pointeurs, 1 par ligne) |
| Libération | Automatique (pile) ou 1 seul `free` | Chaque ligne, PUIS le tableau de pointeurs (ordre important) |
| Fonctions compatibles | `afficher_matrice` du problème 2 | `afficher_matrice_dyn` de ce problème (PAS interchangeables) |

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

## `creer_matrice` — allocation en cascade

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

## `liberer_matrice` — ordre de libération

- Pourquoi doit-on libérer chaque ligne AVANT de libérer le tableau de
  pointeurs, et pas l'inverse ?
  *(Le tableau de pointeurs contient les seules adresses connues des
  lignes. Si on le libère en premier, on perd ces adresses — les lignes
  restent allouées en mémoire mais deviennent définitivement
  inaccessibles : une fuite mémoire irrécupérable pour le reste de
  l'exécution du programme.)*

## `remplir_matrice_aleatoire` et `srand`

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
