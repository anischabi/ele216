# Questions de vérification — Problème 3 (Filtre moyenne mobile)

But : vérifier que l'étudiant.e comprend SON code (peu importe comment il/elle
l'a écrit), pas juste qu'il fonctionne. Ces questions sont formulées pour
s'appliquer à n'importe quelle implémentation correcte — demande à
l'étudiant.e d'expliquer avec SES propres variables et SA propre logique.
Piger 2-3 questions selon le temps disponible.

Chaque question est suivie de la **réponse attendue** (pour le ou la chargé.e
de labo). Une formulation différente est acceptable si l'idée est juste.

Ce problème réutilise l'idée du problème 2 : le tableau est un buffer
circulaire dont `index_ecriture` joue le rôle de `queue`. Un.e étudiant.e qui a
compris le problème 2 devrait comprendre celui-ci rapidement.

## Aide-mémoire : trace d'un débordement (fenêtre de 4)

Scénario du test `test_filtre_debordement_remplace_le_plus_ancien`.
`index_ecriture` = case où sera écrit le prochain échantillon, `n` = nombre
d'échantillons dans la fenêtre. `_` = case jamais écrite. « Fenêtre » = les
échantillons pris en compte, du plus ancien au plus récent.

| Opération | Tableau (cases 0 à 3) | index_ecriture | n | Fenêtre (ancien -> récent) | Moyenne |
|---|---|---|---|---|---|
| départ | `_ _ _ _` | 0 | 0 | vide | 0 |
| `ajouter(1)` | `1 _ _ _` | 1 | 1 | 1 | 1 |
| `ajouter(2)` | `1 2 _ _` | 2 | 2 | 1, 2 | 1.5 |
| `ajouter(3)` | `1 2 3 _` | 3 | 3 | 1, 2, 3 | 2 |
| `ajouter(4)` | `1 2 3 4` | **0** | 4 | 1, 2, 3, 4 | 2.5 |
| `ajouter(100)` | `100 2 3 4` | 1 | 4 | 2, 3, 4, 100 | 27.25 |
| `ajouter(200)` | `100 200 3 4` | 2 | 4 | 3, 4, 100, 200 | 76.75 |

À retenir :

- Quand la fenêtre est pleine, `index_ecriture` pointe sur **le plus ancien**
  échantillon : c'est la case qui est écrasée. `ajouter(100)` remplace le 1.
- `n` reste à 4 : il est plafonné à la taille de la fenêtre.
- La moyenne divise par `n`, pas par 4, tant que la fenêtre n'est pas pleine
  (lignes 2 à 4 : 1/1, 3/2, 6/3).
- Le tableau brut (`100 200 3 4`) n'est pas dans l'ordre chronologique. Pour
  la **moyenne**, ça n'a pas d'importance (une somme ne dépend pas de l'ordre) ;
  pour l'**affichage** dans l'ordre, il faut partir de `index_ecriture`.

## Le principe du filtre

1. Qu'est-ce qu'une moyenne mobile ? Pourquoi dit-on que c'est un filtre
   **passe-bas** ?
   > À chaque instant, on moyenne les N derniers échantillons. Les variations
   > rapides (bruit, pics) sont lissées, les variations lentes passent presque
   > telles quelles : ça atténue les hautes fréquences, donc passe-bas. Plus la
   > fenêtre est grande, plus le lissage est fort (et plus la réponse est lente).

2. Que retourne `filtre_valeur` avant tout échantillon ? Pourquoi faut-il traiter
   ce cas explicitement ?
   > 0 (énoncé). Sans traitement, on calculerait `0 / 0`, ce qui donne `NaN`
   > en `float` (et une division par zéro pour des entiers). Le cas particulier
   > évite ce résultat.

3. Avant que la fenêtre soit pleine, sur combien d'échantillons portent la
   moyenne et le diviseur ? Que se passe-t-il si on divise par la taille de la
   fenêtre ?
   > Sur les échantillons reçus seulement, donc on divise par `n`. Diviser par la
   > taille de la fenêtre sous-estimerait la moyenne (ex. 2, 4, 6 dans une fenêtre
   > de 4 donnerait 3 au lieu de 4, comme si les cases vides valaient 0).

4. Que fait `filtre_ajouter` quand la fenêtre est pleine ? Comment sais-tu
   quel échantillon remplacer ?
   > Il écrase le plus ancien. Avec un index d'écriture circulaire
   > (`(index + 1) % taille`), quand la fenêtre est pleine l'index pointe
   > justement sur le plus ancien échantillon. Pas besoin de décaler les valeurs.

5. Pourquoi `filtre_valeur` peut-elle simplement additionner les cases 0 à
   `n - 1` sans se soucier de l'ordre ?
   > Une somme est commutative. Avant que la fenêtre soit pleine, les valeurs
   > valides sont dans les cases 0 à `n - 1` (rien n'a encore bouclé). Une fois
   > pleine, toutes les cases sont valides. Dans les deux cas, ce sont bien les
   > échantillons de la fenêtre.

## Compteur, précision, complexité

6. L'énoncé dit que l'état contient « le nombre d'échantillons reçus depuis la
   création » mais que `filtre_nb_echantillons` est « saturé à la taille ».
   Comment as-tu géré ça ? Y a-t-il un risque avec un compteur qui ne
   s'arrête jamais ?
   > Deux approches valables : plafonner le compteur à la taille (c'est ce que
   > fait cette solution), ou garder le compteur brut et retourner un `min` dans
   > `filtre_nb_echantillons`. Avec le compteur brut, un `int` finit par
   > **déborder** après environ 2 milliards d'échantillons (quelques semaines à
   > 1 kHz), ce qui est un comportement indéfini : le plafonnement évite ça.

7. Quelle est la complexité de `filtre_ajouter` et de `filtre_valeur` ? Comment
   pourrait-on rendre `filtre_valeur` en O(1) ?
   > `ajouter` : O(1). `valeur` : O(N) (elle reparcourt la fenêtre). Pour O(1),
   > garder une somme courante (on ajoute le nouvel échantillon et on retire
   > celui écrasé). Inconvénient : l'erreur d'arrondi des `float` s'accumule dans
   > la somme au fil du temps. L'énoncé ne prévoit pas ce champ.

8. Pourquoi ne pas comparer la moyenne avec `==` dans les tests ? Que veut dire
   « à la précision flottante près » pour le signal constant ?
   > Les `float` sont approximatifs : la somme puis la division arrondissent. On
   > compare avec une tolérance (`TEST_ASSERT_FLOAT_WITHIN`). Ex. 3.3 n'est pas
   > exactement représentable en binaire : la moyenne de 20 valeurs 3.3 peut
   > différer de 3.3 de quelques 1e-7.

## Interface et mémoire

9. Combien y a-t-il de `malloc` et de `free` ? Que se passe-t-il si le deuxième
   `malloc` échoue dans `filtre_creer` ?
   > Deux `malloc` (structure et tableau) et deux `free`. Si le tableau échoue,
   > il faut libérer la structure avant de retourner NULL, sinon fuite mémoire.
   > À la destruction : d'abord le tableau, puis la structure (sinon on lit
   > `f->echantillons` dans de la mémoire déjà libérée).

10. Que se passe-t-il avec `filtre_detruire(NULL)` ? Pourquoi
    `filtre_creer(0)` retourne-t-il NULL ?
    > `detruire(NULL)` : sans effet (comme `free(NULL)`), documenté dans le `.h`.
    > `creer(0)` : une fenêtre de 0 échantillon n'a aucun sens, et `% 0` serait une
    > division par zéro dans `filtre_ajouter`.

11. Que fait un filtre avec une fenêtre de taille 1 ?
    > Il retourne toujours le dernier échantillon : moyenne d'un seul élément,
    > donc aucun filtrage. Bon cas limite à tester (`% 1` donne toujours 0).

12. Pourquoi les fonctions de lecture prennent-elles `const filtre_t *` ?
    > Promesse de ne pas modifier le filtre, vérifiée par le compilateur.

13. Dans quel ordre `filtre_afficher` doit-il montrer les échantillons ? Comment
    trouver le plus ancien ?
    > Du plus ancien au plus récent. Si la fenêtre est pleine, le plus ancien
    > est à `index_ecriture` ; sinon, il est à la case 0. On parcourt ensuite
    > avec `% taille`. Afficher le tableau brut donnerait un ordre trompeur
    > après un débordement (`100 200 3 4` au lieu de `3, 4, 100, 200`).

## Tests unitaires

14. Pourquoi le test de débordement utilise-t-il des valeurs toutes différentes
    (1, 2, 3, 4 puis 100) et non pas 1, 1, 1, 1 ?
    > Pour détecter si le mauvais échantillon est remplacé. Avec des valeurs
    > toutes différentes, chaque erreur donne une moyenne différente (ex. remplacer
    > le 4 au lieu du 1 donne 26.5 au lieu de 27.25). Avec des valeurs identiques,
    > le test passerait même avec un bug.

15. Si on divisait par la taille de la fenêtre au lieu de `n`, quels tests
    échoueraient ? Si on remplaçait toujours la case 0 au lieu du plus ancien ?
    > Diviser par la taille : `test_filtre_un_seul_echantillon`,
    > `test_filtre_moyenne_partielle`, et `test_filtre_signal_constant` (partie
    > partielle) et `test_filtre_valeurs_negatives`. Toujours case 0 :
    > `test_filtre_debordement_remplace_le_plus_ancien` (la moyenne après 200 serait
    > fausse) et `..._plusieurs_tours`.

16. Pourquoi tester le signal constant et pas seulement des valeurs « rondes » ?
    > Un signal constant doit donner exactement cette constante, quelle que soit
    > la taille de la fenêtre (partielle, pleine, ou débordée). C'est une
    > propriété de la moyenne, vérifiée sans avoir à calculer d'attendu à la
    > main. Ça vérifie aussi la précision.

## Points de vigilance (erreurs courantes)

- **Structure dans le `.h`** : ça marche, mais ça viole l'objectif du lab (type
  opaque). C'est l'erreur la plus importante à repérer.
- **Division par la taille de la fenêtre** au lieu du nombre d'échantillons
  disponibles (moyennes trop basses au démarrage).
- **Division par zéro / `NaN`** quand aucun échantillon n'a été ajouté.
- **Remplacer le mauvais échantillon** (toujours la case 0, ou toujours la
  dernière) à la place du plus ancien.
- **Oubli du `%`** : écriture hors du tableau. Peut passer inaperçu sur de
  petits tests, d'où l'importance de tests qui bouclent plusieurs fois.
- **Compteur qui n'est jamais plafonné** : `filtre_nb_echantillons` retourne plus
  que la taille de la fenêtre (l'énoncé demande « saturé à la taille »).
- **Fuite mémoire** : `filtre_detruire` qui ne libère que la structure, ou
  `filtre_creer` qui ne libère pas la structure si le tableau échoue, ou tests
  qui créent un filtre sans le détruire.
- **Comparer des `float` avec `==`** dans les tests.
- **`filtre_detruire(NULL)`** qui plante.
- **Oubli de `-I ../unity`** dans le Makefile, ou de `unity.c` dans `SRC`.
