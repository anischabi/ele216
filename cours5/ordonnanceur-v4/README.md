# Ordonnanceur v4 — Interpréteur de commandes (strtok + fichiers)

## Objectif de la version

**C'est la version la plus importante pour le lab 6.** L'ordonnanceur n'est plus
codé en dur dans `main` : il devient un **module** (`ordonnanceur.h/.c`) piloté par
des **commandes textuelles**, tapées au clavier ou lues dans un fichier :

```
add nom duree priorite     ajoute un processus
run duree                  fait tourner l'ordonnanceur pendant <duree>
quit                       termine
```

Nouvelles notions : **tokenisation avec `strtok`**, **lecture de lignes avec
`fgets`**, `stdin` et fichiers vus comme le même `FILE*`, arguments `argc/argv`.
Comme les noms viennent maintenant d'un tampon réutilisé, il faut aussi faire une
**copie profonde** du nom dans `processus_creer`.

## Ce qui change depuis la v3

| Fichier | Changement |
|---|---|
| `ordonnanceur.h/.c` | **nouveau module** : `creer`, `interprete`, `ajouter`, `executer` |
| `main.c` | n'a plus d'algorithme ; c'est une **boucle de lecture** qui appelle `ordonnanceur_interprete` |
| `processus.c` | `processus_creer` **copie** le nom (`malloc(strlen+1)` + `strcpy`) |
| `fileprio.h/.c` | ajoute `fileprio_niveaux()` (pour valider la priorité reçue) |
| `commandes_test.txt` | fichier de commandes d'exemple |
| tabdyn / file_generique | identiques à la v3 |

## Ordre de lecture suggéré

1. `commandes_test.txt` : ce qu'on veut pouvoir exécuter.
2. `main.c` : d'où viennent les lignes (`stdin` ou un fichier).
3. `ordonnanceur.c` : `interprete` → `ajouter` / `executer` (**lire lentement**).
4. `processus.c` : la copie du nom, et **pourquoi** elle est obligatoire maintenant (voir plus bas).

## Compiler / exécuter (Windows)

```sh
gcc -Wall -Wextra -g -std=gnu11 *.c -o scheduler.exe
./scheduler.exe                          # mode interactif (prompt " >")
./scheduler.exe commandes_test.txt       # exécute le fichier
./scheduler.exe commandes_test.txt -v    # verbeux : affiche chaque ligne lue
```

(ou `cmake -S . -B build -G "MinGW Makefiles" && cmake --build build`)

## 1. `main.c` : la boucle de lecture

```c
FILE *source = stdin;                 // par défaut : le clavier
switch (argc) {
    case 1: interactif = true; break;                 // ./scheduler
    case 2: source = fopen(argv[1], "r"); break;      // ./scheduler fichier
    case 3: ... if (strcmp("-v", argv[2]) == 0) { verbeux = true; break; }
            // pas de break sinon → tombe dans default (usage)
    default: printf("Usage ..."); return 0;
}

while (!estTermine) {
    memset(tampon, 0, 128);
    if (interactif) printf(" >");
    fgets(tampon, 127, source);       // lit UNE ligne (avec son '\n')
    estTermine = ordonnanceur_interprete(simu, tampon);
}
```

**Idée clé (cours 6)** : `stdin` est un `FILE*` comme un autre. Le même code lit le
clavier ou un fichier, seule la variable `source` change. Le lab 6 exploite cette
idée (`banc_boucle_interactive` lit `stdin`, `banc_charger_fichier` lit un fichier).

Le `case 3` sans `break` est un *fallthrough* **volontaire** : si le 2ᵉ argument
n'est pas `-v`, on tombe dans `default` et on affiche l'usage.

## 2. `strtok` : comment l'interpréteur découpe la ligne

Ligne lue : `"add proc1 80 2\n"`

```c
mot = strtok(buffer, " \n");   // 1er appel : on donne la chaîne → "add"
nom = strtok(NULL, " ");       // appels suivants : NULL → "proc1"
duree = strtok(NULL, " ");     //                         → "80"
priorite = strtok(NULL, " \n");//                         → "2"
```

Ce que fait réellement `strtok` dans le tampon :

```
avant : a d d ␠ p r o c 1 ␠ 8 0 ␠ 2 \n \0
après : a d d \0 p r o c 1 \0 8 0 \0 2 \0 \0
        ^mot     ^nom         ^duree ^priorite
```

1. **Elle modifie la chaîne** : elle remplace chaque délimiteur par `'\0'`. C'est
   pourquoi le paramètre est `char *buffer` et non `const char *`. Les jetons
   retournés sont des **pointeurs dans le tampon** et non des copies.
2. **Elle garde un état caché** (une variable `static` interne) : `strtok(NULL, …)`
   « continue là où on était ». C'est ce qui permet à `ordonnanceur_ajouter` de lire
   les paramètres **sans recevoir la ligne en argument** : `interprete` a lu le
   premier mot, `ajouter` lit la suite.
3. Elle **saute les délimiteurs consécutifs** (plusieurs espaces, ou des espaces en
   début de ligne) et retourne `NULL` quand il n'y a plus de jeton. Le code s'en sert
   pour détecter un **paramètre manquant** (`if (nom == NULL) return;`).
4. Le dernier jeton utilise `" \n"` comme délimiteurs pour ne pas garder le `'\n'`
   laissé par `fgets`.

## 3. Pourquoi la copie du nom est devenue obligatoire

En v1–v3, `p->nom = nom;` pointait vers un littéral (`"proc 1"`), qui vit toujours.
En v4, `nom` pointe **dans `tampon`**, qui est effacé (`memset`) puis réécrit par
chaque `fgets`. J'ai compilé la v4 avec le `processus.c` de la v3 (sans copie).
Voici la sortie réelle :

```
Processus : 80, Temps restant : 5      ← devrait être proc3
Processus : 80, Temps restant : 0
Processus : 80, Temps restant : 60     ← devrait être proc1
Processus : 60, Temps restant : 0      ← devrait être proc6
...
```

Tous les processus affichent ce qui se trouve **à l'adresse `tampon + 4`** au moment
de l'affichage, c'est-à-dire l'argument de la dernière commande `run` (`"run 80"` →
`"80"`). C'est un très bon cas pour appliquer la méthode de déverminage de la v3
(symptôme → hypothèse « le nom pointe vers un tampon réutilisé » → breakpoint sur
`p->nom` → comparer l'adresse avec celle de `tampon`).

Le correctif de la v4 : `malloc(strlen(nom) + 1)` puis `strcpy` (la version du cours 6
utilise `strdup`, qui fait les deux d'un coup). **Le `+1` est pour le `'\0'`.**
Dans ta correction du lab 5, c'est la même raison pour laquelle `resultat_creer`
copie l'`id` et `commande_t` contient un **tableau** `id[32]` (copié avec la struct).

## 4. `ordonnanceur_executer`

```c
int duree = atoi(duree_str);
while (duree > 0) {
    if (fileprio vide) return;
    défiler → execute(UNITE_TEMPS) → afficher → réenfiler si pas fini
    duree -= UNITE_TEMPS;
}
```

Chaque tranche « coûte » 20, même si le processus n'avait besoin que de 5. `run 80`
fait donc au plus 4 tranches.

### Début de la trace avec `commandes_test.txt`

Après les 4 `add` : `1:[proc3(25)]`, `2:[proc1(80) proc2(60)]`, `3:[proc4(70)]`

`run 80` → 4 tranches : proc3→5, proc3→0, proc1→60, proc2→40.
Puis `add proc5 120 2` et `add proc6 10 0`. `run 60` → proc6→0 (niveau 0 en
premier), proc1→40, proc2→20. Et ainsi de suite. Note qu'à la fin il reste proc4
et proc7 non terminés (le dernier `run 120` n'a pas suffi).

## Points à remarquer : défauts réels (vérifiés en exécutant)

Ce sont exactement les erreurs que tes étudiants risquent de **recopier** au lab 6.

1. **Boucle infinie si le fichier ne finit pas par `quit`**. La valeur de retour de
   `fgets` est ignorée. À la fin du fichier, `fgets` retourne `NULL`, le tampon reste
   vide (grâce au `memset`), `interprete` retourne `false`, et la boucle recommence
   sans fin. Vérifié avec un fichier sans `quit` : le programme ne se termine jamais.
   **Les fichiers du lab 6 ne finissent pas par `quit`.** Il faut écrire
   `while (fgets(...) != NULL)`.
2. **Erreur « off-by-one » sur la priorité** : `if (atoi(priorite) > fileprio_niveaux(...))`
   devrait être `>=`. Avec 8 niveaux (0 à 7), `add x 10 8` passe le test puis fait
   échouer l'`assert` dans `fileprio_enfiler`. Vérifié :
   `Assertion failed: priorite < tabdyn_taille(file->listes)`, et le programme s'arrête.
3. **Priorité négative** : `atoi("-1")` vaut -1 et passe le test `>`. Converti en
   `uint8_t`, ça donne 255, et l'`assert` échoue.
4. **`atoi` ne valide rien** : `add x abc 2` donne une durée de 0, sans message.
   (`strtol` avec `endptr` permettrait de détecter l'erreur ; `atof`/`strtof` sont
   l'équivalent pour les `float` du lab 6.)
5. **Commande inconnue ignorée en silence** (`interprete` retourne `false`). Le lab 6
   exige un message d'erreur et un retour de -1.
6. **Pas de lignes vides ni de commentaires** : une ligne vide fonctionne par chance
   (`strtok` retourne NULL). `#` n'est pas géré. Au lab 6, il faut tester `mot[0] == '#'`
   sur le **premier jeton**, et non `ligne[0]`, sinon une ligne indentée qui commence
   par `#` passe à travers.
7. **`\r` (fins de ligne Windows)** : sous Windows, `fopen(…, "r")` (mode texte)
   convertit `\r\n` en `\n`, donc tout va bien. Un fichier CRLF lu **sous Linux** garde
   le `\r` : `"quit\r"` ne correspond plus à `"quit"`. La parade portable est d'utiliser
   les délimiteurs `" \t\r\n"`.
8. **Interface qui fuit l'implémentation** : `ordonnanceur_ajouter(ordo)` et
   `ordonnanceur_executer(ordo)` sont **publiques**, alors qu'elles ne fonctionnent
   que juste après un `strtok` fait par `interprete` (état caché). La version du
   cours 6 (`../../cours6/ordonnanceur-v3/ordonnanceur.c`) corrige ça en les rendant
   `static`. Au lab 6, les fonctions d'analyse des paramètres devraient aussi être `static`.
9. **Mémoire** : pas de `ordonnanceur_detruire`, pas de `fileprio_liberer`, les
   processus terminés ne sont jamais détruits, et maintenant **le nom copié fuit
   aussi** (`processus_detruire` ne fait que `free(p)`, pas `free(p->nom)`). Le fichier
   ouvert n'est jamais `fclose`. (Le cours 6 corrige `processus_detruire`.)
10. `fgets(tampon, 127, …)` avec un tampon de 128 : correct mais inutile. `fgets`
    réserve déjà la place du `'\0'`, donc `sizeof tampon` suffit. Le `memset` n'est
    nécessaire que pour cacher le défaut n° 1.

## Pont vers le lab 6

| v4 (ordonnanceur) | Lab 6 (banc) |
|---|---|
| `ordonnanceur_t { fileprio_t *taches; }` | `banc_t { journal_t*; sequenceur_t*; }` |
| `ordonnanceur_interprete(o, char *buf)` → `bool` (quit) | `banc_interpreter_ligne(b, const char *ligne)` → `1 / 0 / -1` |
| `add`, `run`, `quit` | `set`, `push`, `pop`, `measure`, `report`, `save`, `load`, `status`, `quit` |
| `main` : `source = stdin` ou `fopen` | `banc_boucle_interactive` (stdin) / `banc_charger_fichier` |
| `atoi` | `atof` / `strtof` (tensions, tolérances) |

Différences pièges du lab 6 :

- `ligne` est **`const char *`**, donc il faut **copier** la ligne dans un tampon local
  avant `strtok`.
- `load` appelle `banc_charger_fichier`, qui rappelle `banc_interpreter_ligne`, qui
  rappelle `strtok`. **L'état caché de `strtok` est écrasé.** Il faut extraire (et
  copier) le chemin **avant** de charger, et ne plus faire de `strtok(NULL, …)` sur la
  ligne d'origine après.

## Questions d'auto-vérification

1. Après `strtok` sur `"run 60\n"`, que vaut `strlen(buffer)` ? Pourquoi ?
2. Pourquoi `interprete` peut-il appeler `ajouter(ordo)` sans lui passer la ligne ?
   Qu'est-ce qui casserait si `ajouter` appelait elle-même une fonction qui utilise
   `strtok` sur une autre chaîne ?
3. Corrige la boucle de `main` pour qu'elle s'arrête à la fin du fichier **et** sur `quit`.
4. Avec 8 niveaux, quelles valeurs de priorité doivent être acceptées ? Corrige le test.
5. Que se passe-t-il avec `add  proc1   80   2` (espaces multiples) ? Avec
   `add proc1 80` (priorité manquante) ?
6. Si le séquenceur du lab 5 avait stocké `const char *id` (pointeur) au lieu de
   `char id[32]`, quel symptôme observerait-on au lab 6 ? (Même bug que la section 3.)
