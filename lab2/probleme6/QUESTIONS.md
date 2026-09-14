# Questions de vérification — Problème 6 (Tableau de structures)

But : vérifier que l'étudiant.e comprend son code, pas juste qu'il fonctionne.
Piger 2-3 questions selon le temps disponible.

## `point2D_t` et `distance`

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

## Tableau statique vs dynamique

- Pourquoi utilise-t-on un tableau statique ici, alors qu'on vient de voir
  l'allocation dynamique aux problèmes 4 et 5 ?
  *(La taille (5 points) est connue et fixe à la compilation — pas besoin
  de la flexibilité (ni de la complexité) d'un tableau alloué
  dynamiquement.)*
- Faut-il appeler `free()` quelque part dans ce programme ? Pourquoi ?
  *(Non — un tableau statique/local vit sur la pile et est automatiquement
  libéré à la fin de son scope (ici, à la fin de `main`), pas besoin de
  gestion manuelle.)*

## Boucle des distances consécutives

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
