# Traces des tests — Problème 2 (Buffer circulaire FIFO)

Une trace par test de `buffer_test.c`, dans l'ordre du fichier (sauf les 3
tests sans `tete`/`queue`, regroupés à la fin). Toutes ces traces sont faites
à la main à partir de `buffer.c`.

**Convention**

- Capacité 3, sauf indication contraire (c'est le buffer créé par `setUp`).
- `tete` : case du plus ancien élément (`defiler` lit là, puis avance).
- `queue` : case où on écrit le prochain élément (`enfiler` écrit là, puis avance).
- « Avancer » = `(index + 1) % capacite`.
- `n` : nombre d'éléments présents.
- **Tableau** : le contenu brut de la mémoire, cases 0, 1, 2. `_` = jamais
  écrite. Une valeur déjà défilée **reste en mémoire** (elle n'est pas
  effacée), elle est simplement ignorée.
- **File (FIFO)** : les éléments réellement présents, du plus ancien au plus
  récent. C'est ce qu'on lit en partant de `tete`.

Le tableau brut et la file FIFO ne sont pas dans le même ordre dès qu'il y a
un wrap-around : c'est le point à bien comprendre.

Les tests `creer_capacite_nulle`, `creer_capacite_negative` et
`detruire_null` ne créent aucun buffer utilisable : il n'y a pas de `tete` ni
de `queue` à suivre (voir la fin du document).

---

## 1. `test_buffer_creer_valide` (capacité 5)

| Opération | Tableau (cases 0 à 4) | tete | queue | n | File (FIFO) |
|---|---|---|---|---|---|
| `creer(5)` | `_ _ _ _ _` | 0 | 0 | 0 | vide |

Vérifie : capacité 5, taille 0, `est_vide` = 1, `est_plein` = 0.

## 2. `test_buffer_creer_capacite_un` (capacité 1)

| Opération | Tableau (case 0) | tete | queue | n | File (FIFO) |
|---|---|---|---|---|---|
| `creer(1)` | `_` | 0 | 0 | 0 | vide |

Vérifie : le buffer est créé (non NULL) et sa capacité vaut 1.

## 3. `test_buffer_enfiler_simple`

| Opération | Tableau | tete | queue | n | File (FIFO) |
|---|---|---|---|---|---|
| départ | `_ _ _` | 0 | 0 | 0 | vide |
| `enfiler(10)` → 1 | `10 _ _` | 0 | 1 | 1 | 10 |

Vérifie : taille 1, `est_vide` = 0, `est_plein` = 0.

## 4. `test_buffer_ordre_fifo`

| Opération | Tableau | tete | queue | n | File (FIFO) |
|---|---|---|---|---|---|
| départ | `_ _ _` | 0 | 0 | 0 | vide |
| `enfiler(10)` | `10 _ _` | 0 | 1 | 1 | 10 |
| `enfiler(20)` | `10 20 _` | 0 | 2 | 2 | 10, 20 |
| `enfiler(30)` | `10 20 30` | 0 | **0** | 3 | 10, 20, 30 |
| `defiler` → **10** | `10 20 30` | 1 | 0 | 2 | 20, 30 |
| `defiler` → **20** | `10 20 30` | 2 | 0 | 1 | 30 |
| `defiler` → **30** | `10 20 30` | **0** | 0 | 0 | vide |

L'ordre de sortie (10, 20, 30) est l'ordre d'entrée : c'est le FIFO. Ici,
`queue` revient à 0 au 3e enfilage et `tete` revient à 0 au 3e défilage : le
`%` sert dans les deux sens. Vérifie aussi `est_vide` = 1 à la fin.

## 5. `test_buffer_defiler_diminue_la_taille`

| Opération | Tableau | tete | queue | n | File (FIFO) |
|---|---|---|---|---|---|
| départ | `_ _ _` | 0 | 0 | 0 | vide |
| `enfiler(10)` | `10 _ _` | 0 | 1 | 1 | 10 |
| `enfiler(20)` | `10 20 _` | 0 | 2 | 2 | 10, 20 |
| `defiler` → **10** | `10 20 _` | 1 | 2 | 1 | 20 |

Vérifie : taille 1 (elle a diminué de 1), `est_vide` = 0.

## 6. `test_buffer_defiler_vide_retourne_0`

| Opération | Tableau | tete | queue | n | File (FIFO) |
|---|---|---|---|---|---|
| départ | `_ _ _` | 0 | 0 | 0 | vide |
| `defiler` → **0** | `_ _ _` | 0 | 0 | 0 | vide |

Rien ne change, et la variable de l'appelant vaut toujours -999 : `defiler`
ne doit pas la toucher quand le buffer est vide.

## 7. `test_buffer_defiler_vide_apres_avoir_tout_retire`

| Opération | Tableau | tete | queue | n | File (FIFO) |
|---|---|---|---|---|---|
| départ | `_ _ _` | 0 | 0 | 0 | vide |
| `enfiler(1)` | `1 _ _` | 0 | 1 | 1 | 1 |
| `enfiler(2)` | `1 2 _` | 0 | 2 | 2 | 1, 2 |
| `defiler` → 1 | `1 2 _` | 1 | 2 | 1 | 2 |
| `defiler` → 2 | `1 2 _` | **2** | **2** | 0 | vide |
| `defiler` → **0** | `1 2 _` | 2 | 2 | 0 | vide |

Ici le buffer est vide avec `tete == queue == 2` (pas 0). Le tableau contient
encore 1 et 2, mais `n = 0` : c'est `n` qui dit que le buffer est vide, pas le
contenu du tableau. Le dernier `defiler` retourne 0 et la variable de
l'appelant reste à -999.

## 8. `test_buffer_enfiler_plein_retourne_0`

| Opération | Tableau | tete | queue | n | File (FIFO) |
|---|---|---|---|---|---|
| départ | `_ _ _` | 0 | 0 | 0 | vide |
| `enfiler(1)` | `1 _ _` | 0 | 1 | 1 | 1 |
| `enfiler(2)` | `1 2 _` | 0 | 2 | 2 | 1, 2 |
| `enfiler(3)` | `1 2 3` | 0 | **0** | 3 | 1, 2, 3 |
| `enfiler(4)` → **0** | `1 2 3` | 0 | 0 | 3 | 1, 2, 3 |

Ici `tete == queue == 0` alors que le buffer est **plein** (contrairement au
cas vide). Sans le test « plein », le 4 s'écrirait à `queue` = case 0, donc
**par-dessus le 1** (le plus ancien). Vérifie : le retour vaut 0, taille 3,
`est_plein` = 1.

## 9. `test_buffer_enfiler_plein_ne_modifie_pas_le_contenu`

| Opération | Tableau | tete | queue | n | File (FIFO) |
|---|---|---|---|---|---|
| (3 enfilages : 1, 2, 3) | `1 2 3` | 0 | 0 | 3 | 1, 2, 3 |
| `enfiler(4)` → **0** | `1 2 3` | 0 | 0 | 3 | 1, 2, 3 |
| `defiler` → **1** | `1 2 3` | 1 | 0 | 2 | 2, 3 |
| `defiler` → **2** | `1 2 3` | 2 | 0 | 1 | 3 |
| `defiler` → **3** | `1 2 3` | 0 | 0 | 0 | vide |

On défile tout pour vérifier qu'on retrouve exactement 1, 2, 3 : le 4 n'a
pas remplacé le 1 et n'est pas apparu à la fin.

## 10. `test_buffer_plein_puis_vide_puis_plein`

| Opération | Tableau | tete | queue | n | File (FIFO) | Vérifie |
|---|---|---|---|---|---|---|
| (3 enfilages : 1, 2, 3) | `1 2 3` | 0 | 0 | 3 | 1, 2, 3 | plein = 1, vide = 0 |
| `defiler` → 1 | `1 2 3` | 1 | 0 | 2 | 2, 3 | plein = 0 |
| `defiler` → 2 | `1 2 3` | 2 | 0 | 1 | 3 | |
| `defiler` → 3 | `1 2 3` | 0 | 0 | 0 | vide | vide = 1, plein = 0 |
| `enfiler(4)` | `4 2 3` | 0 | 1 | 1 | 4 | |
| `enfiler(5)` | `4 5 3` | 0 | 2 | 2 | 4, 5 | |
| `enfiler(6)` | `4 5 6` | 0 | **0** | 3 | 4, 5, 6 | plein = 1 |

Ce test suit surtout les indicateurs `est_plein` et `est_vide` à chaque
transition : plein -> pas plein dès le 1er défilage, vide au 3e, plein de
nouveau après 3 enfilages. On réutilise les mêmes cases, dont les anciennes
valeurs sont écrasées.

## 11. `test_buffer_wrap_around_avec_decalage`

| Opération | Tableau | tete | queue | n | File (FIFO) |
|---|---|---|---|---|---|
| départ | `_ _ _` | 0 | 0 | 0 | vide |
| `enfiler(1)` | `1 _ _` | 0 | 1 | 1 | 1 |
| `enfiler(2)` | `1 2 _` | 0 | 2 | 2 | 1, 2 |
| `enfiler(3)` | `1 2 3` | 0 | **0** | 3 | 1, 2, 3 |
| `defiler` → 1 | `1 2 3` | 1 | 0 | 2 | 2, 3 |
| `defiler` → 2 | `1 2 3` | **2** | 0 | 1 | 3 |
| `enfiler(4)` | `4 2 3` | 2 | 1 | 2 | 3, 4 |
| `enfiler(5)` | `4 5 3` | 2 | 2 | 3 | 3, 4, 5 |
| `enfiler(99)` → **0** | `4 5 3` | 2 | 2 | 3 | 3, 4, 5 |
| `defiler` → **3** | `4 5 3` | **0** | 2 | 2 | 4, 5 |
| `defiler` → **4** | `4 5 3` | 1 | 2 | 1 | 5 |
| `defiler` → **5** | `4 5 3` | 2 | 2 | 0 | vide |

C'est le test le plus important. Points clés :

- `enfiler(4)` écrit à la case 0 alors que `tete` est à la case 2 : c'est le
  wrap-around **avec décalage**.
- Le tableau brut est `[4, 5, 3]` mais l'ordre FIFO est 3, 4, 5.
- Plein avec `tete == queue == 2` : `enfiler(99)` doit retourner 0.
- Au 1er `defiler` de la fin, `tete` passe de 2 à `(2 + 1) % 3 = 0` : la tête
  aussi boucle.

## 12. `test_buffer_plusieurs_cycles_remplir_vider`

Une boucle de 5 cycles (`cycle` = 0 à 4). Le cycle 0 utilise les valeurs 1,
2, 3 ; le cycle 1 utilise 11, 12, 13 ; le cycle 2 utilise 21, 22, 23 ; etc.

Cycle 0 (`base = 0`) :

| Opération | Tableau | tete | queue | n | File (FIFO) |
|---|---|---|---|---|---|
| départ | `_ _ _` | 0 | 0 | 0 | vide |
| `enfiler(1)` | `1 _ _` | 0 | 1 | 1 | 1 |
| `enfiler(2)` | `1 2 _` | 0 | 2 | 2 | 1, 2 |
| `enfiler(3)` | `1 2 3` | 0 | **0** | 3 | 1, 2, 3 |
| `enfiler(-1)` → **0** | `1 2 3` | 0 | 0 | 3 | 1, 2, 3 |
| `defiler` → 1 | `1 2 3` | 1 | 0 | 2 | 2, 3 |
| `defiler` → 2 | `1 2 3` | 2 | 0 | 1 | 3 |
| `defiler` → 3 | `1 2 3` | **0** | 0 | 0 | vide |

Cycle 1 (`base = 10`) : on repart du même état (`tete = 0`, `queue = 0`,
`n = 0`), les valeurs 11, 12, 13 écrasent 1, 2, 3 dans le tableau, et on
passe par les mêmes cases dans le même ordre. Il en va de même aux cycles 2,
3 et 4.

Ce test vérifie que l'état revient bien à l'identique après chaque cycle
complet. Par contre, comme `tete` et `queue` valent toujours 0 au début d'un
cycle, il ne détecte pas les bugs qui n'apparaissent qu'avec un **décalage**.
C'est le rôle des tests 11 et 13.

## 13. `test_buffer_entrelace_plusieurs_tours`

Pour `i` de 0 à 19 : `enfiler(i)`, puis `defiler` doit retourner `i`. Le
buffer contient 0 ou 1 élément, mais `tete` et `queue` avancent ensemble.
L'état est donné **après** chaque paire enfiler + défiler (n vaut alors 0) :

| i | Case écrite (`queue` avant) | Tableau après | tete | queue | n |
|---|---|---|---|---|---|
| départ | | `_ _ _` | 0 | 0 | 0 |
| 0 | 0 | `0 _ _` | 1 | 1 | 0 |
| 1 | 1 | `0 1 _` | 2 | 2 | 0 |
| 2 | 2 | `0 1 2` | **0** | **0** | 0 |
| 3 | 0 | `3 1 2` | 1 | 1 | 0 |
| 4 | 1 | `3 4 2` | 2 | 2 | 0 |
| 5 | 2 | `3 4 5` | **0** | **0** | 0 |
| 6 | 0 | `6 4 5` | 1 | 1 | 0 |
| 7 | 1 | `6 7 5` | 2 | 2 | 0 |
| ... | | | | | |
| 19 | 1 | `18 19 17` | 2 | 2 | 0 |

À chaque `i`, la valeur est écrite à la case `i % 3` et relue à la même case.
Sur 20 itérations, `tete` et `queue` font environ 6 tours du tableau et
passent par toutes les positions possibles (0, 1 et 2) comme point de départ.
Le wrap-around est donc exercé de manière répétée, pas seulement à un endroit.
Dans cet état, `tete == queue` et `n = 0` : c'est le cas « vide » décalé.

## 14. `test_buffer_capacite_un_enfiler_defiler` (capacité 1)

| Opération | Tableau (case 0) | tete | queue | n | File (FIFO) |
|---|---|---|---|---|---|
| `creer(1)` | `_` | 0 | 0 | 0 | vide |
| `enfiler(7)` → 1 | `7` | 0 | 0 | 1 | 7 |
| `enfiler(8)` → **0** | `7` | 0 | 0 | 1 | 7 |
| `defiler` → **7** | `7` | 0 | 0 | 0 | vide |
| `enfiler(9)` → 1 | `9` | 0 | 0 | 1 | 9 |
| `defiler` → **9** | `9` | 0 | 0 | 0 | vide |

Avec une seule case, `(index + 1) % 1` donne toujours 0 : `tete` et `queue`
ne quittent jamais la case 0. Le buffer est soit vide (`n = 0`), soit plein
(`n = 1`), avec `tete == queue` dans les deux cas. C'est le cas limite le plus
extrême, et il montre encore que seul `n` distingue plein de vide.

## 15. `test_buffer_capacite_immuable`

| Opération | Tableau | tete | queue | n | File (FIFO) |
|---|---|---|---|---|---|
| (3 enfilages : 1, 2, 3) | `1 2 3` | 0 | 0 | 3 | 1, 2, 3 |
| `enfiler(4)` → **0** | `1 2 3` | 0 | 0 | 3 | 1, 2, 3 |
| `defiler` → 1 | `1 2 3` | 1 | 0 | 2 | 2, 3 |

Vérifie que `capacite` vaut toujours 3 : ni l'enfilage refusé ni le défilage
ne la modifient. Le buffer ne grandit pas, contrairement au `vecteur` du
cours.

---

## Tests sans `tete` ni `queue` à suivre

| Test | Ce qu'il vérifie |
|---|---|
| `test_buffer_creer_capacite_nulle` | `creer(0)` retourne NULL : aucun buffer créé. |
| `test_buffer_creer_capacite_negative` | `creer(-4)` retourne NULL : aucun buffer créé. |
| `test_buffer_detruire_null` | `detruire(NULL)` ne plante pas. Le test réussit s'il arrive au bout. |

Les tests 1 à 15 ci-dessus plus ces 3 tests font bien les 18 tests de
`buffer_test.c`.
