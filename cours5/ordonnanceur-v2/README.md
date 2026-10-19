# Ordonnanceur v2 — Round-Robin avec priorités (File Prioritaire)

## Objectif de la version

Les vrais OS donnent plus de temps processeur aux tâches importantes. On ajoute une
**priorité** à chaque processus et on remplace la File par une **File Prioritaire**
(`fileprio`), construite en **composant** les modules du cours 4 :

> une `fileprio` = un **vecteur** dont chaque case est une **file** (une par niveau).

C'est le premier exemple du cours où un module est fait d'autres modules génériques,
le même principe que le séquenceur du lab 5 (File + Pile) et que le banc du lab 6
(journal + séquenceur).

## Ce qui change depuis la v1

| Fichier | Changement |
|---|---|
| `processus.h/.c` | nouveau champ `uint8_t prio`, paramètre `prio` dans `processus_creer`, nouvel accesseur `processus_obtenir_priorite` |
| `fileprio.h/.c` | **nouveau module** |
| `test_ordonnanceur.c` | utilise `fileprio_*` au lieu de `file_*` |
| `Makefile` | ajoute `fileprio.c` à `SRC` (et `CC = gcc`) |

## Ordre de lecture suggéré

1. `fileprio.h` (l'interface)
2. `fileprio.c` : `struct file_prioritaire`, puis `creer`, `enfiler`, `defiler`, `taille`
3. `test_ordonnanceur.c` (presque identique à la v1)
4. `processus.c` : seulement la différence avec la v1 (`prio`)

## Compiler / exécuter (Windows)

```sh
mingw32-make run
```

ou depuis `cours5/` :

```sh
gcc -Wall -Wextra -g -std=gnu11 -I commun ordonnanceur-v2/*.c commun/vecteur_gen.c commun/file_gen.c -o v2.exe
```

## La File Prioritaire

```c
struct file_prioritaire {
    vecteur_t *listes;   // listes[0] = file prio 0 (la plus haute), listes[1] = prio 1, ...
};
```

```
listes ─► [0] ─► file : [p2]
          [1] ─► file : [p3, p4, p5, p6]
          [2] ─► file : [p1]
          [3] ─► file : []
```

- **creer(n)** : crée le vecteur, puis `n` files vides qu'on y ajoute.
- **enfiler(fp, e, prio)** : `file_enfiler(vecteur_obtenir(listes, prio), e)`, donc à la
  fin de la file de **son** niveau.
- **defiler(fp)** : parcourt les niveaux de 0 vers n-1 et défile dans la
  **première file non vide**.
- **taille(fp)** : somme des tailles des files.

Le `main` ne change presque pas : on réenfile le processus avec **sa** priorité
(`processus_obtenir_priorite`). Tant qu'une file de niveau 0 n'est pas vide, aucun
niveau inférieur ne s'exécute.

### Trace de la sortie réelle

Niveaux initiaux : `0:[p2(40)]`, `1:[p3(50) p4(70) p5(30) p6(40)]`, `2:[p1(80)]`

1. p2 tourne seul jusqu'à 0 (2 tranches), car le niveau 0 passe avant tout.
2. Round-Robin entre p3, p4, p5, p6 au niveau 1 jusqu'à ce que le niveau soit vide
   (p5 et p6 finissent au 2ᵉ tour, p3 au 3ᵉ, p4 au 4ᵉ).
3. p1 (niveau 2) s'exécute enfin seul : 60, 40, 20, 0.

## Points à remarquer

- **Famine (starvation)** : p1 attend que **tous** les processus plus prioritaires
  aient fini. Si des tâches de priorité 0 arrivaient sans arrêt, p1 ne s'exécuterait
  jamais. C'est une vraie notion d'OS (remède classique : le *vieillissement*,
  « aging »).
- **`malloc(sizeof(fileprio_t*))`** dans `fileprio_creer` : c'est la taille d'un
  **pointeur**, pas celle de la structure. Ça « marche » par chance, car la structure
  ne contient qu'un pointeur (même taille). Si on ajoutait un champ, ce serait un
  dépassement de tampon. La bonne forme est `malloc(sizeof(fileprio_t))` ou
  `malloc(sizeof *file)`. **Bug classique à reconnaître chez les étudiants.**
- **Chemin d'erreur incomplet** dans `creer` : si la 3ᵉ `file_creer` échoue,
  `vecteur_liberer` libère le vecteur mais **pas** les 2 files déjà créées, ce qui
  laisse une fuite.
- `fileprio_regarder_tete` et `fileprio_liberer` sont **déclarées dans le .h mais
  pas implémentées** (commentées dans le .c). Ça compile tant que personne ne les
  appelle ; un appel donnerait une erreur à l'**édition de liens** (undefined
  reference), pas à la compilation.
- `return file_enfiler(...)` dans une fonction `void` : `file_enfiler` est aussi
  `void`, donc gcc l'accepte, mais c'est interdit en ISO C (avertissement avec
  `-pedantic`).
- **`const` superficiel** : `fileprio_taille(const fileprio_t *file)` passe
  `file->listes` à `vecteur_taille(vecteur_t*)` (non const) **sans avertissement**.
  Le `const` protège le champ `listes` (le pointeur), pas le vecteur pointé. C'est le
  même phénomène que dans ta correction du lab 5 (`afficher_file` qui fait tourner la
  File).
- Toujours aucune libération des processus terminés (fuite, comme en v1).

## Questions d'auto-vérification

1. Dessine l'état des 4 files après le 7ᵉ affichage.
2. Pourquoi `fileprio_defiler` vérifie-t-il `fileprio_est_vide` **avant** la boucle ?
   (Indice : regarde `vecteur_retirer_premier` dans `../commun`.)
3. Que se passe-t-il si on fait `fileprio_enfiler(taches, p, 4)` avec `fileprio_creer(4)` ?
4. Écris `fileprio_liberer`. Doit-elle libérer les éléments (`processus_t`) ? Qui
   en est propriétaire ?
