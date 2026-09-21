# Questions de vérification — Problème 2 (Tableaux 2D)

But : vérifier que l'étudiant.e comprend SON code, pas juste qu'il
fonctionne. Piger 2-3 questions selon le temps disponible.

1. Que représente le `5` dans la signature `int mat[][5]` ? Pourquoi le
   compilateur en a-t-il besoin, mais pas du nombre de lignes ?
2. Comment as-tu géré l'alignement des colonnes dans `afficher_matrice`
   (ex. si les nombres n'ont pas le même nombre de chiffres) ?
3. Explique comment `sommes_lignes` calcule la somme de chaque ligne.
   As-tu besoin d'initialiser `resultats[i]` avant la boucle ? Pourquoi
   (pas) ?
4. Pour `sommes_colonnes`, dois-tu initialiser `resultats[j]` à 0 avant de
   commencer à accumuler ? Pourquoi (pas), et est-ce différent de ce que tu
   as fait pour `sommes_lignes` ?
5. Une colonne est-elle contiguë en mémoire dans un tableau 2D en C ?
   Comment cela influence-t-il l'ordre de tes boucles dans
   `sommes_colonnes` ?
6. Pourquoi les tableaux `resultats` pour les lignes et pour les colonnes
   n'ont pas nécessairement la même taille ?
7. Que se passerait-il si on appelait tes fonctions avec `lignes = 0` ?
8. Pourquoi le nombre de colonnes doit-il être connu à la compilation dans
   la signature d'une fonction recevant un tableau 2D ?
9. Si tu devais adapter ton code pour une matrice à 8 colonnes au lieu de
   5, que devrais-tu changer ?

## Points de vigilance (indépendants de l'implémentation)

- Un tableau 2D en C est stocké comme un seul bloc contigu, rangé ligne par
  ligne — le compilateur a besoin du nombre de colonnes pour calculer
  l'adresse de `mat[i][j]` par arithmétique de pointeurs.
- `resultats_lignes` a une case par ligne (taille `lignes`),
  `resultats_colonnes` une case par colonne (taille fixe, 5 ici) — ce ne
  sont pas la même taille en général.
- Une colonne n'est PAS contiguë en mémoire (éléments espacés du nombre de
  colonnes) — bon indicateur pour voir si l'étudiant.e comprend le layout
  mémoire, peu importe l'ordre de boucle qu'il/elle a choisi.
