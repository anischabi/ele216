# Questions de vérification — Problème 4 (Tableaux dynamiques : allocation et libération)

But : vérifier que l'étudiant.e comprend SON code, pas juste qu'il
fonctionne. Piger 2-3 questions selon le temps disponible.

1. Pourquoi utilises-tu `malloc(n * sizeof(int))` plutôt que `malloc(n)` ?
2. Que fais-tu si `malloc` échoue (retourne `NULL`) ? Pourquoi est-ce
   important de vérifier ?
3. Qui est responsable d'appeler `free` sur le tableau retourné ? Pourquoi
   pas la fonction `creer_tableau` elle-même ?
4. Que se passe-t-il si on oublie d'appeler `free` dans le `main` ? Le
   programme plante-t-il ?
5. Que se passerait-il si on appelait `free` deux fois de suite sur le
   même pointeur ?
6. Pourquoi ta fonction a-t-elle besoin d'un paramètre `n`, même si
   l'énoncé montre `creer_tableau()` sans paramètre visible dans sa
   signature simplifiée ?
7. As-tu réutilisé une fonction d'affichage d'un problème précédent ?
   Pourquoi est-ce possible ici (l'énoncé le permet) ?

## Points de vigilance (indépendants de l'implémentation)

- `malloc` prend un nombre d'OCTETS, pas d'éléments — `sizeof(int)` est
  nécessaire.
- Vérifier le retour de `malloc` avant de déréférencer le pointeur est une
  bonne pratique, peu importe si l'étudiant.e l'a fait avec un `if` ou
  autrement.
- La responsabilité du `free` revient à l'appelant, pas à la fonction qui
  alloue — sinon le pointeur retourné serait invalide dès son retour.
