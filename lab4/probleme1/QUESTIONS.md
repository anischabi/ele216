# Questions de vérification — Problème 1 (Compteur d'événements)

But : vérifier que l'étudiant.e comprend SON code (peu importe comment il/elle
l'a écrit), pas juste qu'il fonctionne. Ces questions sont formulées pour
s'appliquer à n'importe quelle implémentation correcte — demande à
l'étudiant.e d'expliquer avec SES propres variables et SA propre logique.
Piger 2-3 questions selon le temps disponible.

Chaque question est suivie de la **réponse attendue** (pour le ou la chargé.e
de labo). Une formulation différente est acceptable si l'idée est juste.

## Type opaque et encapsulation

1. Où est définie `struct compteur` et que se passe-t-il si le fichier de
   tests écrit `c->valeur = 42;` ?
   > Dans `compteur.c`. Le `.h` ne contient que
   > `typedef struct compteur compteur_t;` (type incomplet), donc erreur de
   > compilation (« invalid use of incomplete typedef »). Ça protège
   > l'invariant `0 <= valeur < seuil`.

2. L'énoncé dit que le seuil est « immuable ». Qu'est-ce qui le garantit
   dans ton code ?
   > Aucune fonction ne modifie `seuil` après `compteur_creer` (pas de
   > « setter »), et le type opaque empêche le code externe d'y toucher.
   > L'immuabilité vient de l'interface, pas d'un mot-clé.

3. Pourquoi les accesseurs prennent-ils un `const compteur_t *` alors que
   `compteur_incrementer` prend un `compteur_t *` ?
   > `const` promet que la fonction ne modifie pas le compteur : le
   > compilateur refuse toute écriture dans `c->...` à l'intérieur. Ça
   > documente l'intention et permet de les appeler avec un pointeur `const`.
   > `incrementer` modifie l'état, donc pas de `const`.

## Création et destruction

4. Que retournent `compteur_creer(0)` et `compteur_creer(-3)` ? Pourquoi un
   seuil de 0 est-il invalide ?
   > NULL dans les deux cas. Avec seuil 0, la valeur (0) serait déjà « au
   > seuil » avant tout incrément, et l'invariant `0 <= valeur < seuil`
   > serait impossible. Seuil 1 est le plus petit valide (chaque incrément
   > est un dépassement).

5. Que fait ton `compteur_detruire(NULL)` ? Et que se passe-t-il si on
   utilise le compteur après l'avoir détruit ?
   > `free(NULL)` est sans effet, donc rien. Utiliser le pointeur après
   > coup est un comportement indéfini (pointeur pendant) : ça peut
   > « marcher », planter ou lire des valeurs quelconques.

## Logique d'incrémentation

6. Trace ton code : seuil 3, on incrémente 7 fois. Quelles sont la valeur
   et le nombre de dépassements à la fin ?
   > Valeur 1 2 0 | 1 2 0 | 1 → valeur 1, 2 dépassements (7 = 2 * 3 + 1).

7. Dans ta condition de dépassement, utilises-tu `==`, `>=` ou `>` ? Que se
   passerait-il avec `>` ?
   > `==` et `>=` sont corrects (la valeur ne peut pas dépasser le seuil
   > grâce à l'invariant ; `>=` est plus défensif). Avec `>`, la valeur
   > atteindrait `seuil` et le dépassement arriverait un incrément trop
   > tard (le test « atteinte exacte du seuil » échoue).

8. Au moment du dépassement, la valeur doit revenir à 0 et non à 1.
   Pourquoi ?
   > L'incrément qui atteint le seuil est « consommé » par le dépassement :
   > il faut ensuite `seuil` nouveaux incréments pour le prochain
   > dépassement. Revenir à 1 donnerait des cycles de `seuil - 1`.

## Réinitialisation

9. Quelle est la différence entre `compteur_reinitialiser` et détruire puis
   recréer le compteur ?
   > `reinitialiser` remet seulement la valeur à 0 et conserve les
   > dépassements. Recréer remettrait aussi les dépassements à 0.

## Tests

10. Quel test échouerait si `compteur_reinitialiser` remettait aussi les
    dépassements à 0 ? Et si tu avais écrit `>` au lieu de `>=` ?
    > Le test qui fait des dépassements, puis une réinitialisation, puis
    > vérifie que les dépassements sont conservés. Pour `>` : le test
    > d'atteinte exacte du seuil (valeur = seuil au lieu de 0, 0 dépassement).

11. Pourquoi est-il utile de tester « seuil - 1 incréments » en plus de
    « seuil incréments » ?
    > Ce sont les deux côtés de la frontière : ça attrape un dépassement
    > déclenché un incrément trop tôt autant que trop tard (erreur « off by
    > one »).

## Points de vigilance

- Un dépassement déclenché à `valeur > seuil` (un incrément trop tard) ou
  à `valeur == seuil - 1` (un trop tôt).
- `reinitialiser` qui remet aussi `depassements` à 0.
- Des tests qui créent un compteur local sans le détruire (fuite), comme
  dans les exemples du cours 4.
- Accesseurs sans `const` alors que l'énoncé les impose.
- `compteur_creer` qui accepte un seuil de 0.
- Un `main` qui appelle les fonctions sans vérifier que `compteur_creer` n'a
  pas retourné NULL.
