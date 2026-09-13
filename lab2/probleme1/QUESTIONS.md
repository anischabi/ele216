# Questions de vérification — Problème 1 (Tableaux 1D)

But : vérifier que l'étudiant.e comprend son code, pas juste qu'il fonctionne.
Piger 2-3 questions selon le temps disponible.

## `somme_tableau`

- Que retourne la fonction si `n = 0` ? Pourquoi ?
  *(Réponse attendue : 0, car `somme` est initialisée à 0 et la boucle ne
  s'exécute jamais.)*
- Que se passerait-il si on oubliait d'initialiser `somme` à 0 avant la
  boucle ?
  *(Comportement indéfini — la variable contiendrait une valeur aléatoire
  résiduelle de la pile.)*

## `indice_max`

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

## `afficher_tableau`

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

## `main` / général

- Pourquoi le tableau contient-il deux fois la valeur `23` dans cette
  solution ? Qu'est-ce que ça permet d'observer ?
- Pourquoi les fonctions prennent-elles `n` en paramètre plutôt que de le
  calculer à l'intérieur (ex. avec `sizeof`) ?
  *(Un tableau passé en paramètre "dégénère" en pointeur — `sizeof` ne
  donnerait plus la taille du tableau original à l'intérieur de la
  fonction.)*
