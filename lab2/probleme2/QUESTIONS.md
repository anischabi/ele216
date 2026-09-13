# Questions de vérification — Problème 2 (Tableaux 2D)

But : vérifier que l'étudiant.e comprend son code, pas juste qu'il fonctionne.
Piger 2-3 questions selon le temps disponible.

## Signature `int mat[][5]`

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

## `afficher_matrice`

- Pourquoi utilise-t-on `%4d` plutôt que `%d` ou `%d ` ?
  *(Pour garder les colonnes alignées visuellement même si les nombres ont
  un nombre de chiffres différent — largeur de champ fixe.)*

## `sommes_lignes`

- Pourquoi peut-on calculer la somme d'une ligne en une seule boucle simple,
  sans initialiser `resultats[i]` à l'avance ?
  *(On accumule dans une variable locale `somme`, puis on l'assigne une
  seule fois à la fin — pas besoin d'initialiser `resultats[i]` séparément.)*

## `sommes_colonnes` (deux versions dans le code — voir le tableau comparatif dans `probleme2.c`)

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

## `main` / général

- Pourquoi `resultats_lignes` et `resultats_colonnes` n'ont-ils pas la même
  taille ?
  *(`resultats_lignes` a une case par ligne (taille `LIGNES`),
  `resultats_colonnes` a une case par colonne (taille `NB_COLONNES`) — les
  deux valeurs ne sont pas nécessairement égales.)*
- Qu'est-ce que `NB_COLONNES` et pourquoi l'a-t-on défini avec `#define`
  plutôt que d'écrire `5` directement partout dans le code ?
  *(Évite de répéter un "nombre magique" à plusieurs endroits ; un seul
  changement suffit si la taille devait changer.)*
