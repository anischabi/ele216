# Questions de vérification — Problème 1 (Tableaux 1D)

But : vérifier que l'étudiant.e comprend SON code (peu importe comment il/elle
l'a écrit), pas juste qu'il fonctionne. Ces questions sont formulées pour
s'appliquer à n'importe quelle implémentation correcte — demande à
l'étudiant.e d'expliquer avec SES propres variables et SA propre logique.
Piger 2-3 questions selon le temps disponible.

1. Que retourne ta fonction `somme_tableau` si `n = 0` ? Est-ce cohérent
   avec ce qu'on attendrait ?
2. Comment as-tu géré l'initialisation de l'accumulateur de somme, et
   pourquoi c'est nécessaire ?
3. Si plusieurs éléments du tableau partagent la valeur maximale, quel
   indice ton `indice_max` retourne-t-il ? Pourquoi (dépend de ta condition
   de comparaison, `>` ou `>=`) ?
4. Que retournerait ton `indice_max` si `n = 0` ? Ce résultat est-il
   valide/utilisable pour l'appelant ?
5. Comment as-tu évité d'avoir un espace après le dernier élément dans
   `afficher_tableau` ? Explique ta technique.
6. Pourquoi tes fonctions prennent-elles `n` en paramètre plutôt que de
   calculer la taille du tableau avec `sizeof` à l'intérieur ?
7. Quelle est la complexité (nombre d'opérations en fonction de `n`) de
   `somme_tableau` et `indice_max` ?
8. As-tu testé `indice_max` ou `somme_tableau` avec un `n` négatif ou une
   valeur invalide ? Que se passe-t-il dans ton code ?
9. Pourquoi le `main` initialise-t-il le tableau directement dans le code
   plutôt que de le lire depuis l'entrée standard ?
10. Peux-tu expliquer, ligne par ligne, ce que fait ta boucle dans
    `indice_max` ?

## Points de vigilance (indépendants de l'implémentation)

- L'accumulateur de somme doit être initialisé à 0 — sinon comportement
  indéfini (valeur résiduelle de la pile).
- `indice_max` doit gérer correctement le cas d'égalité et documenter (même
  implicitement, via le code) quel indice est retourné.
- Un tableau passé en paramètre à une fonction "dégénère" en pointeur — un
  `sizeof` à l'intérieur de la fonction ne donnerait pas la taille du
  tableau original, d'où la nécessité de `n`.
