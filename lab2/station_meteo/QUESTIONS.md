# Questions de vérification — Mini-app Station météo

But : vérifier que l'étudiant.e comprend SON code, pas juste qu'il
fonctionne. Cette mini-app combine tout le labo — bon endroit pour vérifier
la compréhension d'ensemble, pas juste un détail isolé. Piger 2-3 questions
selon le temps disponible.

1. Pourquoi le tableau de capteurs et la matrice d'historique ne sont-ils
   pas alloués de la même façon (une seule allocation vs plusieurs) ?
2. Que représente le nombre de colonnes de ta matrice d'historique, et à
   quel moment est-il fixé ?
3. `station_creer` retourne-t-elle la station par valeur ou via un
   pointeur ? Quelle est la conséquence sur les pointeurs internes
   (capteurs, historique) ?
4. As-tu initialisé les pointeurs de ta structure à `NULL` avant de tenter
   les allocations ? Pourquoi est-ce utile ?
5. Que fait ton code si une allocation échoue en cours de route (ex.
   l'historique échoue après que les capteurs aient réussi) ?
6. Comment as-tu initialisé les capteurs prédéfinis (température, pression,
   humidité) ?
7. Comment `station_enregistrer_mesure` sait-elle combien de valeurs
   contient le tableau `valeurs[]` reçu en paramètre ?
8. Appelles-tu `capteur_mise_a_jour` en plus d'écrire dans l'historique ?
   Pourquoi (pas) ?
9. Pourquoi ne peux-tu pas réutiliser `indice_max` du problème 1 tel quel
   pour `station_indice_valeur_max` ?
10. Dans `station_liberer`, pourquoi la libération de l'historique
    nécessite (généralement) une boucle, contrairement à celle des
    capteurs (ou l'inverse, selon ton implémentation) ?
11. Dans quel ordre libères-tu les différents éléments de la station, et
    pourquoi cet ordre est important ?

## Points de vigilance (indépendants de l'implémentation)

- L'énoncé exige la réutilisation des fonctions pertinentes des numéros
  précédents (`capteur_init`, `capteur_mise_a_jour`, la logique de
  `indice_max`, etc.) — vérifier que ce n'est pas du copier-coller aveugle
  mais une vraie compréhension de pourquoi ça s'applique ici.
- Le C n'a pas de généricité : la logique de `indice_max` (sur `int[]`) doit
  être réécrite pour `float` si l'historique est en `float`.
- Peu importe l'implémentation, une bonne gestion mémoire doit couvrir : le
  cas d'échec d'allocation partielle, et l'ordre de libération (lignes
  avant le tableau de pointeurs, dans un sens ou dans l'autre selon la
  structure choisie).
