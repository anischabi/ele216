# Questions de vérification — Problème 2 (Servo-moteur)

But : vérifier que l'étudiant.e comprend SON code (peu importe comment il/elle
l'a écrit), pas juste qu'il fonctionne. Ces questions sont formulées pour
s'appliquer à n'importe quelle implémentation correcte — demande à
l'étudiant.e d'expliquer avec SES propres variables et SA propre logique.
Piger 2-3 questions selon le temps disponible.

Chaque question est suivie de la **réponse attendue** (pour le ou la chargé.e
de labo). Une formulation différente est acceptable si l'idée est juste.

## Type opaque et invariant

1. Quel est l'invariant de ta structure ? Quelles fonctions l'établissent et
   le maintiennent ?
   > `angle_min <= angle <= angle_max` (avec `angle_min < angle_max` et
   > `vitesse > 0`). Établi par `servo_creer` (validation + angle = min),
   > maintenu par `servo_aller_a` (saturation), la seule fonction qui
   > modifie `angle`. Le type opaque empêche le reste du programme de le
   > casser.

2. Les butées et la vitesse sont « immuables ». Comment le garantis-tu ?
   > Aucune fonction ne les modifie après la création (pas de « setter »)
   > et personne d'autre ne peut atteindre les champs (type opaque).

## Création

3. Que retournent `servo_creer(45, 45, 60)` et `servo_creer(-90, 90, 0)` ?
   Pourquoi refuser une vitesse nulle ?
   > NULL dans les deux cas (`min >= max` et `vitesse <= 0`). Une vitesse
   > nulle causerait une division par zéro dans `servo_temps_pour_atteindre`
   > (et un servo qui ne bouge pas n'a pas de sens physique).

4. Quel est l'angle courant juste après la création ? Pourquoi un test avec
   `angle_min = 0` ne suffit-il pas à le vérifier ?
   > L'angle minimum. Si `min = 0`, un angle initialisé à 0 par erreur
   > passerait quand même le test : il faut une butée minimale non nulle
   > (ex. -90) pour distinguer les deux.

## Saturation

5. Trace ton code : butées [-90, 90], on appelle `aller_a(200)` puis
   `aller_a(-200)` puis `aller_a(45)`. Quel est l'angle après chaque appel ?
   > 90, -90, 45.

6. Où la saturation est-elle faite : dans `aller_a` ou ailleurs ? Qu'est-ce
   qui se passerait si tu saturais seulement dans l'accesseur `servo_angle` ?
   > Dans `aller_a`, au moment de modifier l'état. Si on stockait la
   > consigne brute et saturait seulement à la lecture, l'invariant serait
   > faux en mémoire et `servo_temps_pour_atteindre` calculerait à partir
   > d'un angle impossible.

## Temps de déplacement

7. Calcule à la main : butées [-90, 90], vitesse 60 deg/s, angle courant
   -90. Combien de temps pour atteindre 30 ? Et ensuite, depuis 30, pour
   atteindre -60 ?
   > |30 - (-90)| / 60 = 2 s. Puis |-60 - 30| / 60 = 1.5 s.

8. Pourquoi une valeur absolue dans le calcul ? Que retournerait ton code
   sans elle pour un déplacement vers un angle plus petit ?
   > Un temps est toujours positif, peu importe le sens. Sans valeur
   > absolue, on obtiendrait un temps négatif (-1.5), impossible à
   > distinguer du code d'erreur « hors butées ».

9. `servo_temps_pour_atteindre` prend un `const servo_t *`. Qu'est-ce que ça
   garantit, et que ferait le compilateur si tu appelais `servo_aller_a`
   à l'intérieur ?
   > La fonction ne modifie pas le servo (calcul « sans modifier l'état »,
   > comme demandé). Appeler `servo_aller_a(s, ...)` avec un `const
   > servo_t *` donne un avertissement (on retire le `const`) : le
   > compilateur aide à respecter le contrat.

10. Que retourne ta fonction pour un angle égal à une butée (ex. 90) ? Et
    pour 120 ?
    > Pour 90 : un temps valide (course complète 180 / 60 = 3 s), les
    > butées font partie de la plage. Pour 120 : une valeur négative
    > (hors butées), pas le temps vers l'angle saturé.

## Nombres à virgule flottante

11. Pourquoi comparer des `float` avec `==` est-il risqué en général, et
    pourquoi tes tests peuvent-ils quand même utiliser des valeurs exactes ?
    > Beaucoup de décimaux (0.1, 1/3...) ne sont pas représentables
    > exactement, donc les calculs accumulent des erreurs d'arrondi. Des
    > valeurs comme 2.0, 1.5 ou 3.0 (issues d'entiers divisés par 60) sont
    > exactes. Sinon on utilise une tolérance (`TEST_ASSERT_FLOAT_WITHIN`).
    > Note : `TEST_ASSERT_EQUAL_FLOAT` de Unity utilise déjà une petite
    > tolérance relative.

## Points de vigilance

- Angle initial à 0 au lieu de `angle_min`.
- Validation `min > max` (accepte des butées égales) ou `vitesse < 0`
  (accepte 0, puis division par zéro).
- `servo_temps_pour_atteindre` qui oublie la valeur absolue, qui calcule
  depuis 0 au lieu de l'angle courant, ou qui modifie l'angle.
- Hors butées : retourner le temps vers l'angle saturé au lieu d'une valeur
  négative.
- Butées exactes rejetées par `servo_temps_pour_atteindre` (`<` / `>` au lieu
  de `<=` / `>=`).
- Test de saturation au minimum qui part de la position initiale (déjà au
  minimum) : il passerait même sans saturation.
- Accesseurs sans `const`, ou fonctions qui oublient `assert(s != NULL)`.
- NaN (hors énoncé, bonus) : `min >= max || vitesse <= 0` laisse passer NaN ;
  la version de référence écrit `!(min < max) || !(vitesse > 0)`.
