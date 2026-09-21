# Questions de vérification — Problème 1 (Pile rechargeable)

But : vérifier que l'étudiant.e comprend SON code (peu importe comment il/elle
l'a écrit), pas juste qu'il fonctionne. Ces questions sont formulées pour
s'appliquer à n'importe quelle implémentation correcte — demande à
l'étudiant.e d'expliquer avec SES propres variables et SA propre logique.
Piger 2-3 questions selon le temps disponible.

Chaque question est suivie de la **réponse attendue** (pour le ou la chargé.e
de labo). Une formulation différente est acceptable si l'idée est juste.

## Type opaque et encapsulation

1. Où est définie `struct pile` et pourquoi pas dans le `.h` ? Que se passe-t-il
   si le `main` ou le fichier de tests écrit `p->charge = 5000;` ?
   > La structure est dans `pile.c`. Le `.h` ne contient que
   > `typedef struct pile pile_t;` (type incomplet). Le code externe ne peut
   > donc pas accéder aux champs : erreur de compilation (« invalid use of
   > incomplete typedef »). Ça protège l'invariant `0 <= charge <= capacite_max`.

2. Comment garantis-tu que la capacité maximale et la tension sont
   « immuables » ? Il n'y a pas de `const` sur les champs, alors qu'est-ce qui
   l'empêche ?
   > Aucune fonction du module ne les modifie après `pile_creer`, et personne
   > d'autre ne peut les atteindre (type opaque). L'immuabilité vient de
   > l'interface (pas de « setter »), pas d'un mot-clé.

3. Quel est l'invariant de ta structure et quelles fonctions doivent le
   respecter ?
   > `0 <= charge <= capacite_max`. Établi par `pile_creer` (charge = 0),
   > maintenu par `pile_charger` et `pile_decharger` (les seules qui modifient
   > `charge`). Comme les champs sont cachés, personne d'autre ne peut le casser.

## Création et destruction

4. Que retournent `pile_creer(0, 3.7)`, `pile_creer(1000, -1)` et
   `pile_creer(-5, 0)` ? Pourquoi retourner NULL plutôt qu'utiliser un `assert` ?
   > NULL dans les trois cas. Des paramètres invalides sont une erreur possible
   > à l'exécution (valeur venant de l'utilisateur), signalée par la valeur de
   > retour comme le fait `malloc`. Un `assert` est réservé aux erreurs de
   > programmation (ex. passer un pointeur NULL à une fonction qui l'interdit).

5. Pourquoi refuser une capacité de 0 et pas seulement les capacités négatives ?
   > `pile_pourcentage` divise par la capacité : capacité = 0 donnerait une
   > division par zéro (résultat `inf` ou `NaN`). Condition de validation :
   > `<= 0` et non `< 0`.

6. Que se passe-t-il si on appelle `pile_detruire(NULL)` ? Et si on utilise `p`
   après `pile_detruire(p)` ?
   > `NULL` : sans effet, car `free(NULL)` est défini comme ne rien faire
   > (comportement documenté dans le `.h`). Utiliser `p` après : pointeur
   > pendant (*dangling*), comportement indéfini. Bonne pratique : mettre `p = NULL`
   > après la destruction.

7. Combien y a-t-il de `malloc` et de `free` dans ton module ? Qui libère la
   mémoire d'une pile, l'appelant ou le module ?
   > Un seul `malloc` (la structure, pas de tableau interne) et un seul `free`.
   > Le module fournit `pile_detruire`, mais c'est l'appelant qui doit l'appeler,
   > une fois par pile créée avec succès.

## Charge, décharge, pourcentage

8. Comment as-tu implémenté la saturation ? Pourquoi ne suffit-il pas d'écrire
   `charge += mAh;` ?
   > On ajoute puis on plafonne : `if (charge > capacite_max) charge = capacite_max;`
   > (ou un `min`). Sans plafond, la charge dépasserait la capacité maximale et
   > `pile_pourcentage` retournerait plus de 100 %.

9. Que fait ton code si on appelle `pile_decharger(p, 1000)` sur une pile qui
   contient 300 mAh ? Et sur une pile vide ?
   > La charge tombe à 0 dans les deux cas (plancher à 0), jamais de valeur
   > négative.

10. Que se passe-t-il avec `pile_charger(p, -100)` ? Est-ce que l'énoncé le dit ?
    Qu'aurait-on pu faire d'autre ?
    > L'énoncé n'en parle pas : c'est une décision de conception. Cette solution
    > ignore les valeurs <= 0 (documenté dans le `.h`). Sinon, `charger(-100)`
    > déchargerait la pile en douce. Alternatives valables : `assert`, ou
    > retourner un code d'erreur. L'important est que le choix soit conscient et
    > documenté.

11. Comment calcules-tu `pile_pourcentage` ? Que retourne-t-il à 0 %, 50 % et
    100 % ?
    > `charge / capacite_max * 100.0f`. Donne 0, 50 et 100 (pour une pile de
    > 1000 mAh contenant 0, 500 et 1000 mAh).

## Qualité de l'interface

12. Pourquoi les accesseurs et `pile_afficher` prennent-ils `const pile_t *` alors
    que `pile_charger` prend `pile_t *` ?
    > `const` est une promesse que la fonction ne modifie pas la pile : le
    > compilateur refuse toute écriture dans la structure. `pile_charger` et
    > `pile_decharger` modifient la charge, donc pas de `const`.

13. Pourquoi les fonctions utilisent-elles `assert(p != NULL)` ? Que se passe-t-il
    à la compilation avec `-DNDEBUG` ?
    > Le pointeur NULL est une erreur du programmeur appelant, pas une situation
    > normale : on la détecte en développement. Avec `-DNDEBUG`, les `assert`
    > disparaissent (aucune vérification, plantage possible sur un NULL).

## Tests unitaires

14. Pourquoi utilise-t-on `TEST_ASSERT_FLOAT_WITHIN` (ou `EQUAL_FLOAT`) plutôt
    que `==` pour comparer des `float` ?
    > Les `float` sont approximatifs (ex. additionner dix fois `0.1f` donne
    > `1.0000001` et non `1.0f`). Il faut une tolérance. Dans ces tests, les
    > valeurs (250, 500, 1000) sont exactement représentables, mais il faut
    > prendre l'habitude de la tolérance.

15. À quoi servent `setUp` et `tearDown` ? Que se passerait-il sans `tearDown` ?
    > `setUp` s'exécute avant chaque test (pile neuve), `tearDown` après (libère
    > la pile). Chaque test est ainsi indépendant. Sans `tearDown`, chaque test
    > laisserait une fuite mémoire.

16. Si on supprimait la ligne qui plafonne la charge dans `pile_charger`, quel
    test échouerait ? Et si on supprimait le plancher dans `pile_decharger` ?
    > Saturation : `test_pile_charger_sature` (et 2 autres). Plancher :
    > `test_pile_decharger_plus_que_la_charge` (et `..._pile_vide`). Bonne question
    > pour vérifier que l'étudiant.e comprend ce que ses tests protègent.

17. Quelle est la complexité de chaque fonction ?
    > Toutes en O(1) : pas de boucle, seulement des opérations arithmétiques et
    > des comparaisons (`malloc`/`free` mis à part).

## Points de vigilance (erreurs courantes)

- **Structure dans le `.h`** : ça marche, mais ça viole l'objectif du lab
  (type opaque). C'est l'erreur la plus importante à repérer.
- **Fuite mémoire** : un test qui appelle `pile_creer` sans `pile_detruire`
  (typique dans les tests de création valide).
- **`pile_creer` qui valide avec `< 0`** au lieu de `<= 0` : accepte une
  capacité de 0, d'où `inf`/`NaN` dans `pile_pourcentage`.
- **Saturation ou plancher oublié** (ou codé seulement dans un des deux sens).
- **`pile_detruire(NULL)`** qui plante (déréférencement sans vérification).
- **Comparaison de `float` avec `==`** dans les tests.
- **Oubli de `-I`** dans le Makefile pour trouver `unity.h`, ou d'inclure
  `unity.c` dans `SRC`.
- **`pile_afficher` incomplet** : l'énoncé demande *tous* les champs
  (capacité maximale, charge courante, tension).
- **Modifier `capacite_max` ou `tension`** ailleurs que dans `pile_creer`.
