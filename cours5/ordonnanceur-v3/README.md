# Ordonnanceur v3 — Conteneurs autonomes + méthode de déverminage

## Objectif de la version

Même algorithme que la v2 (Round-Robin avec priorités), mais le projet est maintenant
**autonome** : il n'utilise plus `../commun`, il a ses propres conteneurs plus
complets et plus rigoureux (`tabdyn_generique`, `file_generique`), et il se compile
avec **CMake** au lieu d'un Makefile.

Le `readme.md` du cours associe cette version à la **méthode de déverminage** :

1. Obtenir un mode de défaillance (symptôme).
2. Émettre une hypothèse sur la cause.
3. Concevoir un test minimal qui vérifie l'hypothèse.
4. Exécuter le test et noter le résultat.
5. Placer un *breakpoint* au bon endroit et comparer la valeur attendue à la valeur obtenue.
6. Corriger, puis relancer toute la suite de tests pour trouver le défaut suivant.

## Ce qui change depuis la v2

| v2 (`../commun`) | v3 (local) |
|---|---|
| `vecteur_gen` (`int` partout, pas de `const`) | `tabdyn_generique` : `size_t`, `const` sur les lectures, `bool` en retour |
| `file_gen` | `file_generique` : `file_enfiler` retourne `bool`, `file_regarder_tete`, `file_vider`, `file_afficher`, et `file_defiler` **sûr sur une file vide** (retourne NULL) |
| `fileprio_enfiler` → `void` | → `bool` (propage l'échec de `file_enfiler`) |
| `fileprio_afficher` commentée | **implémentée** (outil de débogage) |
| `Makefile` | `CMakeLists.txt` |
| `processus.prio` | renommé `priorite` (+ doc des fonctions) |

L'algorithme de `main.c` est inchangé. Seuls les processus de départ diffèrent
(8 niveaux).

## Ordre de lecture suggéré

1. `tabdyn_generique.h` puis `.c` : compare avec `../commun/vecteur_gen.c`. Regarde
   surtout `redimensionner` (avec `realloc`), `ajouter` (doublement), `supprimer`
   (`memmove` + réduction de la capacité).
2. `file_generique.c` : une file n'est qu'un `tabdyn` où on ajoute à la fin et retire
   à l'indice 0.
3. `fileprio.c` : identique à la v2, sauf les types.
4. `main.c`.

## Compiler / exécuter (Windows)

Avec CMake :

```sh
cmake -S . -B build -G "MinGW Makefiles" && cmake --build build && ./build/scheduler.exe
```

ou plus simplement :

```sh
gcc -Wall -Wextra -g -std=gnu11 *.c -o scheduler.exe && ./scheduler.exe
```

## Détails intéressants de `tabdyn_generique.c`

- **`redimensionner`** : `realloc` peut déplacer le bloc. On assigne le résultat à une
  **variable temporaire** (`nouveau`) avant d'écraser `tableau->contenu`. Si
  `realloc` échoue, l'ancien bloc reste valide et n'est pas perdu.
  (`p = realloc(p, n)` directement est un bug classique : fuite en cas d'échec.)
- **`ajouter`** : si plein, la capacité double (ou passe à 8 si elle était à 0). Le
  coût est amorti en O(1).
- **`supprimer(i)`** : `memmove` (et non `memcpy`, car les zones se chevauchent)
  décale les éléments vers la gauche. Si `taille < capacite/4`, on divise la
  capacité par 2.
- **`file_defiler` = `tabdyn_supprimer(tab, 0)`** : O(n) à cause du décalage. C'est
  simple, pas optimal (un tampon circulaire serait O(1)).

### Trace de la sortie réelle

Niveaux : `2:[p2(50) p3(90)]`, `3:[p1(80) p4(30) p6(70)]`, `4:[p5(10)]`

1. Round-Robin p2/p3 au niveau 2 : p2 finit après 3 tranches, p3 continue seul jusqu'à 0.
2. Round-Robin p1/p4/p6 au niveau 3 : p4 finit, puis p1, puis p6.
3. p5 (niveau 4) s'exécute en dernier, en une tranche.

## Points à remarquer

- Les défauts de `fileprio.c` de la v2 sont **toujours là** : `malloc(sizeof(fileprio_t*))`,
  la fuite sur le chemin d'erreur de `creer`, et `regarder_tete`/`liberer`
  non implémentées.
- **Avertissements `-Wsign-compare`** : `for (int i = 0; i < tabdyn_taille(...); ...)`
  compare un `int` à un `size_t`. C'est inoffensif ici, mais c'est le genre
  d'avertissement à faire corriger (`size_t i`).
- `main` ne libère toujours rien (processus, fileprio).
- `fileprio_afficher` n'affiche que des **adresses** (`%p`) : un conteneur générique
  ne sait pas afficher ses éléments. La solution habituelle, c'est un pointeur de
  fonction : `file_generique.h` déclare déjà `afficher_element_fn` et
  `liberer_element_fn`, mais ne s'en sert pas encore.

## Exercice de déverminage à faire soi-même

Pour pratiquer la méthode, injecte un défaut et applique les 6 étapes. Par exemple,
dans `fileprio_defiler`, remplace `i = 0` par `i = 1` :

1. **Symptôme** : un processus n'est jamais exécuté et la boucle de `main` ne se
   termine jamais.
2. **Hypothèse** : un niveau n'est jamais défilé.
3. **Test minimal** : une fileprio à 2 niveaux, un seul élément au niveau 0, puis
   `defiler`. On attend l'élément, on obtient NULL.
4. **Breakpoint** dans `fileprio_defiler` : observe `i` et `file_est_vide(...)` à
   chaque itération.
5. Corrige, puis relance **tous** les tests (régression).

C'est exactement ce que demande l'exercice du cours 5 : *« Ajoutez une suite de tests
Unity pour chacun des modules du scheduler v3 ou v4 »*.

## Questions d'auto-vérification

1. Pourquoi `redimensionner` teste-t-il `!nouveau && nouvelle_capacite > 0` et pas
   seulement `!nouveau` ?
2. Que retourne `file_defiler` sur une file vide en v3 ? Et en v2 (`../commun`) ?
3. Avec 8 niveaux, quelle est la capacité du `tabdyn` `listes` après `fileprio_creer(8)` ?
   Y a-t-il eu un `realloc` ?
4. Écris les tests Unity minimaux de `fileprio` : ordre FIFO dans un niveau,
   priorité entre niveaux, `defiler` sur vide → NULL, `taille`.
