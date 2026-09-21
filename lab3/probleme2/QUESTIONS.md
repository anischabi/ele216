# Questions de vérification — Problème 2 (Buffer circulaire FIFO)

But : vérifier que l'étudiant.e comprend SON code (peu importe comment il/elle
l'a écrit), pas juste qu'il fonctionne. Ces questions sont formulées pour
s'appliquer à n'importe quelle implémentation correcte — demande à
l'étudiant.e d'expliquer avec SES propres variables et SA propre logique.
Piger 2-3 questions selon le temps disponible.

Chaque question est suivie de la **réponse attendue** (pour le ou la chargé.e
de labo). Une formulation différente est acceptable si l'idée est juste.

Astuce : demander à l'étudiant.e de **dessiner le tableau** avec `tete` et
`queue` au tableau/sur papier pendant qu'il/elle répond. Ceux et celles qui ont
compris le wrap-around le font sans hésiter.

## Aide-mémoire : trace d'un wrap-around (capacité 3)

Scénario du test `test_buffer_wrap_around_avec_decalage`. `n` = nombre
d'éléments, `_` = case sans valeur utile.

- `queue` : case où on écrit le prochain élément (`enfiler` écrit là, puis avance).
- `tete` : case du plus ancien élément (`defiler` lit là, puis avance).
- « Avancer » = `(index + 1) % capacite` : après la dernière case, on revient à 0.

| Opération | Tableau (cases 0, 1, 2) | tete | queue | n |
|---|---|---|---|---|
| départ | `_ _ _` | 0 | 0 | 0 |
| enfiler(1) | `1 _ _` | 0 | 1 | 1 |
| enfiler(2) | `1 2 _` | 0 | 2 | 2 |
| enfiler(3) | `1 2 3` | 0 | **0** | 3 |
| défiler → 1 | `1 2 3` | 1 | 0 | 2 |
| défiler → 2 | `1 2 3` | 2 | 0 | 1 |
| enfiler(4) | `4 2 3` | 2 | 1 | 2 |
| enfiler(5) | `4 5 3` | 2 | 2 | 3 |

À retenir :

- **enfiler(3)** : `queue` valait 2 et `(2 + 1) % 3 = 0`. Elle revient au début
  du tableau. Sans le `%`, elle vaudrait 3, hors du tableau.
- **enfiler(4)** : le 4 s'écrit à la case 0 alors que `tete` est à la case 2,
  parce que les cases 0 et 1 ont été libérées par les défilages.
- **Ordre de sortie** : le tableau en mémoire est `[4, 5, 3]`, mais l'ordre FIFO
  est 3, 4, 5. On lit à partir de `tete` (case 2), puis on revient à la case 0.
  L'ordre FIFO n'est pas l'ordre en mémoire.
- **Dernière ligne** : le buffer est plein et `tete == queue == 2`. C'est
  `n` qui distingue « plein » de « vide » (où `tete == queue` aussi).
  `enfiler(99)` doit donc retourner 0.

## Le principe du buffer circulaire

1. Explique avec un dessin : pour un buffer de capacité 3, où sont `tete` et
   `queue` après `enfiler(1), enfiler(2), enfiler(3), defiler, defiler, enfiler(4)` ?
   Quel est le contenu du tableau interne ?
   > Après les 3 enfilages : `queue` a rebouclé à 0. Après 2 défilages :
   > `tete = 2`. Après `enfiler(4)` : le 4 est écrit à l'index 0 (`queue`
   > passe à 1). Tableau interne `[4, 2, 3]`, mais les éléments valides sont
   > 3 puis 4 (`tete = 2`, `nb_elements = 2`). L'ordre FIFO n'est **pas**
   > l'ordre en mémoire.

2. Comment fais-tu « reboucler » un index à la fin du tableau ? Que se
   passerait-il si tu l'oubliais ?
   > Opérateur modulo : `index = (index + 1) % capacite`. Sans lui, `queue`
   > dépasserait la fin du tableau : écriture hors limites (débordement du tas),
   > comportement indéfini. Ça ne plante pas toujours immédiatement, d'où
   > l'intérêt de tests qui bouclent plusieurs fois (et de `valgrind`/ASan si
   > disponible).

3. Pourquoi ne pas simplement décaler tous les éléments du tableau vers la
   gauche à chaque `defiler` ? Quelle est la complexité de `enfiler` et
   `defiler` dans ton implémentation ?
   > Décaler coûterait O(n) par défilage. Avec deux index, `enfiler` et
   > `defiler` sont O(1) : c'est tout l'intérêt de la structure circulaire.

## Plein ou vide ?

4. Quand le buffer est vide, `tete == queue`. Et quand il est plein ? Comment
   distingues-tu les deux cas ?
   > Quand il est plein, `tete == queue` aussi (la queue a rattrapé la tête).
   > C'est le compteur `nb_elements` qui les distingue : vide si 0, plein si
   > égal à la capacité. (Alternative : sacrifier une case, mais l'énoncé
   > impose le compteur.)

5. Que fait `buffer_enfiler` sur un buffer plein ? Pourquoi est-il important
   qu'il ne modifie rien ?
   > Retourne 0 sans rien écrire. Si on écrivait quand même à `queue` (qui est
   > égal à `tete` quand plein), on **écraserait l'élément le plus ancien**
   > (perte de données silencieuse) et on corromprait l'invariant.

6. Que fait `buffer_defiler` sur un buffer vide ? Que contient `*valeur` après
   l'appel ?
   > Retourne 0. `*valeur` n'est pas modifié (documenté dans le `.h`). Sinon
   > l'appelant pourrait prendre une valeur bidon pour une vraie donnée.

## Interface et mémoire

7. Pourquoi `buffer_defiler` retourne-t-il un code (0/1) et passe la valeur par
   pointeur (`int *valeur`) plutôt que de retourner directement l'entier ?
   > Il faut pouvoir signaler « vide » : toutes les valeurs d'`int` sont des
   > données valides, donc aucune valeur de retour ne peut servir de sentinelle.
   > Le code de retour indique le succès, et la donnée sort par le paramètre de
   > sortie (pointeur).

8. Combien y a-t-il de `malloc` et de `free` ? Dans quel ordre libères-tu, et
   pourquoi ?
   > Deux `malloc` (la structure et le tableau) et deux `free`. On libère
   > d'abord le tableau (`free(b->donnees)`), puis la structure. Inverser
   > l'ordre lit `b->donnees` dans de la mémoire déjà libérée : accès après
   > `free`, comportement indéfini, et on perd l'adresse du tableau.

9. Dans `buffer_creer`, que se passe-t-il si le **deuxième** `malloc` échoue ?
   > Il faut libérer la structure déjà allouée avant de retourner NULL. Sinon
   > : fuite mémoire (la structure est perdue).

10. Que se passe-t-il avec `buffer_detruire(NULL)` ?
    > Sans effet (comme `free(NULL)`), documenté dans le `.h`. Sans la
    > vérification, `b->donnees` déréférencerait NULL et le programme planterait.

11. Pourquoi les fonctions de lecture (`buffer_taille`, `buffer_est_vide`,
    `buffer_afficher`…) prennent-elles `const buffer_t *` ?
    > Promesse de ne pas modifier le buffer, vérifiée par le compilateur.
    > `buffer_enfiler` et `buffer_defiler` modifient l'état, donc sans `const`.

12. Pourquoi `buffer_creer(0)` retourne-t-il NULL ? Que se passerait-il avec une
    capacité de 0 dans `enfiler` ?
    > Un tableau de 0 case n'a aucun emplacement utile, et `% capacite` ferait
    > une **division par zéro** (planter le programme). La validation de
    > `capacite <= 0` protège tout le reste du module.

## Tests unitaires

13. Pourquoi le test de wrap-around utilise-t-il un buffer de capacité 3 et non
    de 1000 ?
    > Avec un petit buffer, on atteint la fin du tableau en quelques opérations,
    > donc le cas « reboucle » est exercé facilement (et les cas plein/vide sont
    > faciles à atteindre). Avec 1000, il faudrait 1000 opérations pour le voir.

14. Pourquoi ne pas se contenter de tester des cycles « remplir complètement puis
    vider complètement » ? Quel test ajoute-t-on de plus ?
    > Dans ces cycles, `tete` et `queue` reviennent toujours à 0 : on ne teste
    > pas le cas où le wrap arrive avec un décalage. D'où le test avec
    > `enfiler ×3, defiler ×2, enfiler ×2` (le 4 s'écrit à l'index 0 alors que
    > `tete = 2`) et le test entrelacé (`enfiler`/`defiler` alternés 20 fois).

15. Si on retirait la vérification « buffer plein » dans `buffer_enfiler`, quel
    test échouerait ? Et si on retirait celle de « buffer vide » dans
    `buffer_defiler` ?
    > Plein : `test_buffer_enfiler_plein_retourne_0` (et
    > `..._ne_modifie_pas_le_contenu`, `plusieurs_cycles`…). Vide :
    > `test_buffer_defiler_vide_retourne_0`. Bonne question pour vérifier que
    > l'étudiant.e comprend ce que ses tests protègent.

16. Comment ton test vérifie-t-il que `defiler` sur un buffer vide ne modifie
    pas `*valeur` ?
    > On initialise une variable à une valeur « sentinelle » (ex. -999), on
    > appelle `defiler`, puis on vérifie qu'elle vaut toujours -999.

## Points de vigilance (erreurs courantes)

- **Structure dans le `.h`** : ça marche, mais ça viole l'objectif du lab
  (type opaque). C'est l'erreur la plus importante à repérer.
- **Oubli du `%`** (ou test `if (index == capacite) index = 0` incorrect) :
  débordement du tableau. Peut passer inaperçu sur de petits tests.
- **Buffer plein/vide confondus** : utiliser seulement `tete == queue` sans
  compteur.
- **`enfiler` sur buffer plein qui écrase quand même** l'élément le plus ancien.
- **Fuite mémoire** : `buffer_detruire` qui ne libère que la structure (pas
  le tableau), ou `buffer_creer` qui ne libère pas la structure si le tableau
  échoue, ou tests qui créent un buffer sans le détruire.
- **Ordre des `free` inversé** (structure avant tableau).
- **`buffer_afficher` qui montre l'ordre du tableau** plutôt que l'ordre FIFO
  (et donc affiche `[4, 2, 3]` au lieu de `[2, 3, 4]` après un wrap-around).
- **Tests qui ne couvrent que le cas sans wrap-around** (pas de cycle complet
  ni de décalage).
- **`buffer_defiler` qui modifie `*valeur` quand le buffer est vide.**
- **Oubli de `-I ../unity`** dans le Makefile, ou de `unity.c` dans `SRC`.
