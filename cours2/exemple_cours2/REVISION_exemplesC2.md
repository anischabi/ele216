# Révision — exemple_cours2

Thème du cours : **l'allocation dynamique de mémoire en C** — construction d'un
tableau dynamique (équivalent maison d'un `std::vector` / `ArrayList`) avec
`malloc`/`realloc`/`free`.

Fichiers : `tabdyn.h`, `tabdyn.c`, `tabdyn_test.c`, `Makefile`.

## 1. `tabdyn.h` — l'interface publique

```c
typedef struct tabdyn {
    int capacite;   // L'espace de stockage actuel
    int taille;     // L'espace utilisé
    int *contenu;   // Pointeur vers les données
} tabdyn_t;
```

Distinction centrale à faire comprendre aux étudiant.e.s :
- **`capacite`** = combien de cases sont réservées en mémoire (allouées).
- **`taille`** = combien de cases sont réellement utilisées (éléments ajoutés).
- `capacite >= taille` toujours. Le tableau peut avoir de la place libre en
  réserve.

Fonctions déclarées (documentées en Doxygen-style dans le `.h`) :
- `tabdyn_creer(taille_initiale)` — alloue et initialise, retourne `NULL` si
  l'allocation échoue.
- `tabdyn_ajouter(tabdyn, valeur)` — ajoute un élément, agrandit
  automatiquement si nécessaire. Retourne le nombre d'éléments après l'ajout,
  ou `0` si l'agrandissement a échoué.
- `tabdyn_liberer(tabdyn)` — libère toute la mémoire (comportement `NULL`-safe
  comme `free()`).
- `tabdyn_afficher(tabdyn)` — affichage de débogage.

## 2. `tabdyn.c` — l'implémentation

### `tabdyn_creer`
```c
tabdyn_t *tabdyn = malloc(sizeof(tabdyn_t));
int* contenu_initial = malloc(taille_initiale * sizeof(int));
```
Deux allocations séparées : une pour la structure elle-même, une pour le
tableau de contenu qu'elle pointe. **Gestion d'erreur en cascade** : si la
2e allocation échoue, on `free()` la 1ère avant de retourner `NULL` — sinon
fuite mémoire.

### `tabdyn_ajouter`
```c
if (tabdyn->taille == tabdyn->capacite) {
    if (agrandir(tabdyn) == false) { return 0; }
}
tabdyn->contenu[tabdyn->taille] = valeur;
tabdyn->taille++;
```
N'agrandit **que si nécessaire** (quand `taille` atteint `capacite`). Sinon,
insertion directe en O(1).

### `agrandir` (fonction `static`, privée au fichier)
```c
int *nouveau = realloc(tabdyn->contenu, tabdyn->capacite * sizeof(int) * 2);
if (nouveau == NULL) { return false; }
tabdyn->contenu = nouveau;
tabdyn->capacite *= 2;
```
**Point pédagogique clé : stratégie de doublement de capacité.**
- On **double** la capacité à chaque agrandissement plutôt que de l'augmenter
  de 1 à chaque fois. C'est la stratégie classique pour obtenir un coût
  **amorti O(1)** par insertion (au lieu de O(n) si on réallouait à chaque
  ajout).
- `static` devant `agrandir` : fonction visible seulement dans `tabdyn.c`,
  pas exposée dans le `.h` — c'est un détail d'implémentation, pas une partie
  de l'API publique.
- Motif de sécurité avec `realloc` : on stocke le résultat dans une variable
  **temporaire** (`nouveau`) avant de l'assigner à `tabdyn->contenu`. Si
  `realloc` échoue, il retourne `NULL` mais **le bloc original reste valide**
  — écraser directement `tabdyn->contenu` par `NULL` perdrait le pointeur et
  causerait une fuite mémoire. C'est une erreur classique à surveiller dans le
  code des étudiant.e.s.

### `tabdyn_liberer`
```c
free(tabdyn->contenu);
free(tabdyn);
```
Libère dans l'ordre : d'abord le contenu pointé, ensuite la structure elle-même.
(Contrairement à ce que documente le `.h`, cette version ne vérifie pas
explicitement `tabdyn == NULL` avant de déréférencer — à noter si un.e
étudiant.e pose la question, `free(NULL)` est sans effet mais
`tabdyn->contenu` sur un pointeur `NULL` crasherait.)

### `tabdyn_afficher`
Affiche `capacite`, `taille`, puis le contenu entre accolades — utile pour
visualiser visuellement les agrandissements pendant les tests.

## 3. `tabdyn_test.c` — démonstration

```c
tabdyn_t *tab_test = tabdyn_creer(2);
tabdyn_ajouter(tab_test, 1);
tabdyn_ajouter(tab_test, 2);
tabdyn_ajouter(tab_test, 3);
tabdyn_ajouter(tab_test, 4);
tabdyn_ajouter(tab_test, 5);
tabdyn_afficher(tab_test);
tabdyn_liberer(tab_test);
```

Créé avec une capacité initiale de **2**, puis 5 éléments sont ajoutés :
- Ajout de 1, 2 → tableau plein (`taille == capacite == 2`).
- Ajout de 3 → déclenche `agrandir()` : capacité 2 → 4.
- Ajout de 4 → tableau plein à nouveau (`taille == capacite == 4`).
- Ajout de 5 → déclenche un 2e `agrandir()` : capacité 4 → 8.

Résultat affiché : `capacite = 8`, `taille = 5`, contenu `{ 1, 2, 3, 4, 5, }`.
C'est un bon exemple à tracer à la main pour voir la progression des
doublements.

## 4. `Makefile`

Même structure que celui du cours1 : compile `tabdyn_test.c + tabdyn.c` en
objets séparés, dépendances automatiques via `-MMD -MP`, cibles `all`, `run`,
`clean`, `distclean`.

## Fil conducteur pédagogique

Ce cours enseigne le patron classique du **tableau à capacité croissante** :
séparer `taille` (occupé) de `capacite` (réservé), agrandir par
**doublement** plutôt qu'unité par unité, et manipuler `malloc`/`realloc`/`free`
de façon sécuritaire (variable temporaire pour `realloc`, libération en
cascade en cas d'échec).

## Pistes de questions pour la vérification en labo

- Pourquoi doubler la capacité plutôt que l'augmenter de 1 à chaque ajout ?
- Que se passe-t-il si `realloc` échoue dans `agrandir` ? Pourquoi utilise-t-on
  une variable temporaire (`nouveau`) plutôt que d'assigner directement à
  `tabdyn->contenu` ?
- Quelle est la différence entre `capacite` et `taille` ? Peuvent-ils être
  égaux ? Différents ?
- Pourquoi `agrandir` est-elle déclarée `static` ?
- Dans `tabdyn_test.c`, à quel moment exactement le tableau est-il agrandi,
  et à combien de capacité se retrouve-t-il après les 5 ajouts ?
