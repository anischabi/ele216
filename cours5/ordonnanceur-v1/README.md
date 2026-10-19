# Ordonnanceur v1 — Round-Robin simple (sans priorité)

## Objectif de la version

Première brique de l'étude de cas : simuler l'**ordonnanceur** (scheduler) d'un
système d'exploitation. Plusieurs processus veulent le processeur ; l'OS les fait
passer **à tour de rôle** (Round-Robin), chacun pendant une **tranche de temps**
fixe (ici 20 unités). Un processus pas fini retourne à la fin de la file.

C'est l'application directe de la **File** générique du cours 4 (`../commun/file_gen`).

## Ordre de lecture suggéré

1. `processus.h` : l'interface (type opaque `processus_t`).
2. `processus.c` : la structure cachée et les fonctions.
3. `test_ordonnanceur.c` : la boucle de l'ordonnanceur (≈ 10 lignes, c'est le cœur).
4. `Makefile` : compile `vecteur_gen.c` et `file_gen.c` depuis `../commun`.

## Compiler / exécuter (Windows)

Le Makefile utilise `gcc-13` (machine de l'enseignant). Sous Windows :

```sh
mingw32-make CC=gcc run
```

ou directement, depuis `cours5/` :

```sh
gcc -Wall -Wextra -g -std=gnu11 -I commun ordonnanceur-v1/*.c commun/vecteur_gen.c commun/file_gen.c -o v1.exe
./v1.exe
```

## Le module `processus`

```c
struct processus {
    char *nom;          // pointeur vers le nom (PAS de copie)
    int duree;          // temps restant
    bool est_termine;
};
```

- `processus_execute(p, 20)` : retire 20 au temps restant. Si `duree <= 20`, met
  le temps à 0 et marque le processus terminé.
- Les accesseurs (`obtenir_nom`, `obtenir_duree`, `est_termine`) existent parce que
  la structure est **opaque** : `main` ne peut pas écrire `p->duree`.

## L'algorithme (test_ordonnanceur.c)

```c
while (!file_est_vide(taches)) {
    processus_t *suivant = file_defiler(taches);   // prend la tête
    processus_execute(suivant, 20);                 // une tranche
    processus_afficher(suivant);
    if (!processus_est_termine(suivant))
        file_enfiler(taches, suivant);              // retourne à la fin
}
```

### Trace de la sortie réelle

État initial de la file : `[p1(80), p2(40), p3(50)]`

| Tour | Défilé | Reste | File après |
|---|---|---|---|
| 1 | p1 | 60 | p2 p3 p1 |
| 2 | p2 | 20 | p3 p1 p2 |
| 3 | p3 | 30 | p1 p2 p3 |
| 4 | p1 | 40 | p2 p3 p1 |
| 5 | p2 | **0** → retiré | p3 p1 |
| 6 | p3 | 10 | p1 p3 |
| 7 | p1 | 20 | p3 p1 |
| 8 | p3 | **0** → retiré | p1 |
| 9 | p1 | **0** → retiré | (vide) |

C'est exactement ce qu'affiche le programme.

## Points à remarquer (défauts volontaires ou simplifications)

- **Le nom n'est pas copié** : `p->nom = nom;`. Ça fonctionne ici seulement parce
  que les noms sont des **littéraux** (`"programme_1"`), qui vivent toute la durée du
  programme. Ça va devenir un vrai bug en v4, quand les noms viendront d'un tampon
  de lecture → voir `../ordonnanceur-v4/README.md`.
- **Fuites mémoire** : un processus terminé n'est jamais `processus_detruire`, et la
  file n'est jamais `file_liberer`. (Un `valgrind` / `drmemory` le montrerait.)
- `main` n'a pas de `return 0;` : permis pour `main` en C99+ (retour implicite 0).
- La File de `commun` (`file_defiler` → `vecteur_retirer_premier`) **ne vérifie pas**
  si elle est vide : défiler une file vide met `taille` à -1. Ici c'est protégé par le
  `while (!file_est_vide(...))`.

## Lien avec les labs

- Même patron que le lab 5 : un **type opaque** (`processus_t` ↔ `resultat_t`) stocké
  par pointeur `void*` dans un conteneur générique du cours 4.
- La boucle « défiler → traiter → éventuellement réenfiler » ressemble à
  `sequenceur_executer_tout` (sauf que le séquenceur ne réenfile jamais).

## Questions d'auto-vérification

1. Pourquoi la file contient-elle des `void*` et pas des `processus_t*` ? Qu'est-ce
   que ça implique au moment de `file_defiler` ?
2. Si on remplace `"programme_1"` par un `char nom[32]` local rempli avec `strcpy`
   puis réutilisé pour les 3 processus, qu'affiche le programme ? Pourquoi ?
3. Où faudrait-il appeler `processus_detruire` pour éliminer la fuite ?
4. Que se passe-t-il si la tranche (20) est plus grande que toutes les durées ?
