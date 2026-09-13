# Révision — exemple_cours1

Thème du cours : **la gestion de fichiers en C** (`stdio.h`) — écriture/lecture en
mode texte, écriture/lecture en mode binaire, et utilisation de `struct` pour
représenter des données structurées.

Fichiers : `date_heure.h/.c`, `mesure.h/.c`, `exemple_fichiers.c`, `mesures.txt`,
`Makefile`.

## 1. `date_heure.h` / `date_heure.c` — une struct utilitaire

```c
struct date_heure {
    uint16_t annee;
    uint8_t mois, jours, heure, minute, seconde;
};
```

- Utilise les types à taille fixe de `<stdint.h>` (`uint16_t`, `uint8_t`) plutôt
  que `int` : taille **prévisible en mémoire** (important pour l'écriture
  binaire plus loin) et économie d'espace (1 octet vs 4 pour `int`).
- `creer_date_heure(...)` : fonction "constructeur" qui remplit une struct
  locale et la **retourne par valeur** (pas par pointeur) — pattern courant en C.

## 2. `mesure.h` / `mesure.c` — deux versions d'une même idée

```c
struct mesure {
    char *timestamp;      // horodatage en texte (string)
    int temperature;
    int humidite;
};

struct mesure_v2 {
    struct date_heure timestamp;  // horodatage structuré
    int8_t temperature;
    uint8_t humidite;
};
```

C'est le cœur pédagogique du cours : **comparer deux façons de représenter la
même donnée**.

- `mesure` (v1) : timestamp en `char*`, taille **variable** selon la longueur
  du texte.
- `mesure_v2` : timestamp en `struct date_heure` compacte, taille **fixe et
  connue** (`sizeof`), température/humidité réduites à `int8_t`/`uint8_t`.

Cette différence taille fixe vs variable est **cruciale** pour la suite
(écriture/lecture binaire).

`affiche_mesure_v2()` : simple affichage formaté avec `printf`, sert à
visualiser une struct `mesure_v2`.

## 3. `exemple_fichiers.c` — le fichier principal, 6 démonstrations progressives

| Fonction | Ce qu'elle montre |
|---|---|
| `test_fprintf()` | `fprintf` vers `stdout`, `stderr`, et vers un fichier ouvert en écriture texte (`"wt"`) |
| `test_lecture_texte()` | Lecture ligne par ligne d'un fichier texte avec `fgets` dans une boucle `while` |
| `test_ecriture_bin_v1()` | Écriture binaire (`"wb"`, `fwrite`) de `struct mesure` — écrit la chaîne `timestamp` avec `strlen()` (taille **variable**) puis les int, sans séparateur ni longueur stockée → illustre pourquoi c'est **fragile à relire** |
| `test_ecriture_bin_v2()` | Même chose avec `mesure_v2` : `fwrite(donnees, sizeof(struct mesure_v2), 6, data)` — un seul appel, struct de taille fixe d'un coup. Beaucoup plus propre. |
| `test_lecture_bin_v2()` | Relecture binaire avec `fread` en boucle, `malloc`/`free` pour l'enregistrement temporaire |
| `test_seek()` | `fseek(source, 3 * sizeof(struct mesure_v2), SEEK_SET)` — accès direct au 4e enregistrement sans tout relire, grâce à la taille fixe |

**Fil conducteur pédagogique** :
`test_ecriture_bin_v1()` (taille variable, fragile)
→ `test_ecriture_bin_v2()` / `test_lecture_bin_v2()` (taille fixe, fiable)
→ `test_seek()` (bénéfice concret : accès direct par calcul d'offset).

C'est probablement l'idée clé que les étudiant.e.s doivent retenir de ce cours.

## 4. `mesures.txt`

Fichier de données texte utilisé par `test_lecture_texte()` — correspond au
format de sortie de `affiche_mesure_v2`.

## 5. `Makefile`

Makefile standard : compile `exemple_fichiers.c + date_heure.c + mesure.c` en
objets séparés (`-MMD -MP` pour dépendances automatiques via les `.h`),
cibles `all`, `run`, `clean`, `distclean`. Pas spécifique au contenu du cours —
exemple de bonnes pratiques de compilation modulaire.

## Pistes de questions pour la vérification en labo
- Pourquoi utiliser `mesure_v2` plutôt que `mesure` pour écrire en binaire ?
- Que se passerait-il si on essayait de faire un `fseek` sur le fichier écrit
  par `test_ecriture_bin_v1` ?
- Pourquoi `date_heure` utilise-t-elle `uint8_t`/`uint16_t` plutôt que `int` ?
