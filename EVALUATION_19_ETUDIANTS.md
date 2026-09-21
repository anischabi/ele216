# Labo 2 — Grille de questions pour 19 étudiant.e.s

Une question par problème (7 questions au total : Problèmes 1 à 6 +
Station météo) par étudiant.e, piochées dans `QUESTIONS_LAB2.md`. Questions
**génériques** — elles s'appliquent peu importe comment l'étudiant.e a
implémenté sa solution (noms de variables, choix de fonctions, structure de
boucle) puisqu'elles demandent d'expliquer SON code, pas de reconnaître une
implémentation précise.

Remplace "Étudiant 1", "Étudiant 2", ... par les noms réels. Rappel du
guide : 2-3 questions suffisent normalement par étudiant.e selon le temps —
ceci te donne 7 questions **au choix**, pas 7 questions obligatoires à
poser.

---

## Étudiant 1
- **P1** — Que retourne ta fonction `somme_tableau` si `n = 0` ? Est-ce
  cohérent avec ce qu'on attendrait ?
- **P2** — Pour `sommes_colonnes`, dois-tu initialiser `resultats[j]` à 0
  avant de commencer à accumuler ? Pourquoi (pas), et est-ce différent de
  `sommes_lignes` ?
- **P3** — Après avoir mis à jour un des deux capteurs dans ton `main`,
  pourquoi l'autre reste-t-il inchangé ?
- **P4** — Qui est responsable d'appeler `free` sur le tableau retourné ?
  Pourquoi pas la fonction `creer_tableau` elle-même ?
- **P5** — Où et combien de fois appelles-tu `srand()` dans ton
  programme ? Pourquoi cet endroit précis ?
- **P6** — Que calcules-tu juste avant d'appliquer la racine carrée, et
  pourquoi ?
- **Station** — Que fait ton code si une allocation échoue en cours de
  route ?

## Étudiant 2
- **P1** — Comment as-tu géré l'initialisation de l'accumulateur de somme,
  et pourquoi c'est nécessaire ?
- **P3** — Quelle fonction (ou technique) as-tu utilisée pour copier les
  chaînes de caractères ? Quel est le risque si on utilisait `strcpy` sans
  aucune précaution ?
- **P4** — Que se passe-t-il si on oublie d'appeler `free` dans le
  `main` ? Le programme plante-t-il ?
- **P5** — Comment as-tu obtenu des valeurs aléatoires entre 20 et 80
  exactement ?
- **P6** — As-tu utilisé `sqrt` ou `sqrtf` ? Pourquoi ce choix est-il
  cohérent avec le type de tes champs `x`/`y` ?
- **P2** — Une colonne est-elle contiguë en mémoire dans un tableau 2D en
  C ? Comment cela influence-t-il l'ordre de tes boucles dans
  `sommes_colonnes` ?
- **Station** — Comment as-tu initialisé les capteurs prédéfinis
  (température, pression, humidité) ?

## Étudiant 3
- **P1** — Si plusieurs éléments du tableau partagent la valeur maximale,
  quel indice ton `indice_max` retourne-t-il ? Pourquoi ?
- **P2** — Pourquoi les tableaux `resultats` pour les lignes et pour les
  colonnes n'ont pas nécessairement la même taille ?
- **P3** — Que contiendrait `capteur->titre` si tu avais oublié
  complètement l'étape de copie ?
- **P4** — Que se passerait-il si on appelait `free` deux fois de suite sur
  le même pointeur ?
- **P5** — Peux-tu expliquer, étape par étape, ce que représente
  `matrice[i][j]` dans ta structure ?
- **P6** — Pourquoi ton `main` utilise-t-il un tableau statique plutôt que
  dynamique pour les 5 points ?
- **Station** — Comment `station_enregistrer_mesure` sait-elle combien de
  valeurs contient le tableau `valeurs[]` reçu en paramètre ?

## Étudiant 4
- **P1** — Que retournerait ton `indice_max` si `n = 0` ? Ce résultat
  est-il valide/utilisable pour l'appelant ?
- **P2** — Que se passerait-il si on appelait tes fonctions avec
  `lignes = 0` ?
- **P3** — Qu'est-ce qu'une "copie profonde" et pourquoi l'énoncé insiste
  là-dessus pour `titre` et `unite` ?
- **P4** — Pourquoi ta fonction a-t-elle besoin d'un paramètre `n`, même si
  l'énoncé montre `creer_tableau()` sans paramètre visible ?
- **P5** — Pourquoi ne peux-tu pas réutiliser `afficher_matrice` du
  problème 2 pour afficher cette matrice-ci ?
- **P6** — Combien de distances "consécutives" calcules-tu pour 5 points ?
  Jusqu'où va ta boucle, et pourquoi pas plus loin ?
- **Station** — Appelles-tu `capteur_mise_a_jour` en plus d'écrire dans
  l'historique ? Pourquoi (pas) ?

## Étudiant 5
- **P1** — Comment as-tu évité d'avoir un espace après le dernier élément
  dans `afficher_tableau` ? Explique ta technique.
- **P2** — Pourquoi le nombre de colonnes doit-il être connu à la
  compilation dans la signature d'une fonction recevant un tableau 2D ?
- **P3** — Comment as-tu copié la chaîne de caractères reçue en paramètre
  dans le champ `titre` du capteur ? Pourquoi ne peux-tu pas simplement
  écrire `capteur->titre = titre;` ?
- **P4** — As-tu réutilisé une fonction d'affichage d'un problème
  précédent ? Pourquoi est-ce possible ici ?
- **P5** — Décris la structure mémoire de ta matrice : combien d'appels à
  `malloc` fais-tu au total pour une matrice de 5 lignes ?
- **P6** — Comment calcules-tu la distance totale ?
- **Station** — Pourquoi ne peux-tu pas réutiliser `indice_max` du
  problème 1 tel quel pour `station_indice_valeur_max` ?

## Étudiant 6
- **P1** — Pourquoi tes fonctions prennent-elles `n` en paramètre plutôt
  que de calculer la taille du tableau avec `sizeof` à l'intérieur ?
- **P2** — Si tu devais adapter ton code pour une matrice à 8 colonnes au
  lieu de 5, que devrais-tu changer ?
- **P3** — Que se passe-t-il dans ton implémentation si le titre fourni
  dépasse 19 caractères (la capacité utile de `char[20]`) ?
- **P4** — Pourquoi utilises-tu `malloc(n * sizeof(int))` plutôt que
  `malloc(n)` ?
- **P5** — Tes lignes sont-elles garanties d'être contiguës en mémoire les
  unes par rapport aux autres ? Pourquoi (pas) ?
- **P6** — Que se passerait-il si ta boucle allait un indice trop loin ?
- **Station** — Dans `station_liberer`, pourquoi la libération de
  l'historique nécessite (généralement) une boucle, contrairement à celle
  des capteurs ?

## Étudiant 7
- **P1** — Quelle est la complexité (nombre d'opérations en fonction de
  `n`) de `somme_tableau` et `indice_max` ?
- **P2** — Que représente le `5` dans la signature `int mat[][5]` ?
  Pourquoi le compilateur en a-t-il besoin, mais pas du nombre de lignes ?
- **P3** — Comment t'assures-tu que la chaîne copiée est bien terminée par
  `'\0'` dans tous les cas ?
- **P4** — Que fais-tu si `malloc` échoue (retourne `NULL`) ? Pourquoi
  est-ce important de vérifier ?
- **P5** — Si l'allocation d'une ligne échoue en cours de route, que fait
  ton code ? As-tu géré ce cas ?
- **P6** — Pourquoi ta fonction `distance` reçoit-elle des pointeurs plutôt
  que les points par valeur ?
- **Station** — Dans quel ordre libères-tu les différents éléments de la
  station, et pourquoi cet ordre est important ?

## Étudiant 8
- **P1** — As-tu testé `indice_max` ou `somme_tableau` avec un `n` négatif
  ou une valeur invalide ? Que se passe-t-il dans ton code ?
- **P2** — Comment as-tu géré l'alignement des colonnes dans
  `afficher_matrice` ?
- **P3** — Pourquoi `capteur_mise_a_jour` reçoit-elle (généralement)
  seulement la nouvelle valeur mesurée et pas les autres champs ?
- **P4** — Qui est responsable d'appeler `free` sur le tableau retourné ?
  Pourquoi pas la fonction `creer_tableau` elle-même ?
- **P5** — Dans quel ordre libères-tu les lignes et le tableau de
  pointeurs dans `liberer_matrice` ? Pourquoi cet ordre est important ?
- **P6** — As-tu utilisé `sqrt` ou `sqrtf` ? Pourquoi ce choix est-il
  cohérent avec le type de tes champs `x`/`y` ?
- **Station** — Pourquoi le tableau de capteurs et la matrice d'historique
  ne sont-ils pas alloués de la même façon ?

## Étudiant 9
- **P1** — Pourquoi le `main` initialise-t-il le tableau directement dans
  le code plutôt que de le lire depuis l'entrée standard ?
- **P2** — Explique comment `sommes_lignes` calcule la somme de chaque
  ligne. As-tu besoin d'initialiser `resultats[i]` avant la boucle ?
  Pourquoi (pas) ?
- **P3** — Pourquoi passes-tu un pointeur vers le capteur (plutôt que le
  capteur par valeur) à tes fonctions ?
- **P4** — Que se passerait-il si on appelait `free` deux fois de suite sur
  le même pointeur ?
- **P5** — Pourquoi passes-tu `colones` à `creer_matrice`, mais
  `liberer_matrice` n'en a probablement pas besoin ?
- **P6** — Pourquoi ton `main` utilise-t-il un tableau statique plutôt que
  dynamique pour les 5 points ?
- **Station** — Que représente le nombre de colonnes de ta matrice
  d'historique, et à quel moment est-il fixé ?

## Étudiant 10
- **P1** — Peux-tu expliquer, ligne par ligne, ce que fait ta boucle dans
  `indice_max` ?
- **P2** — Pour `sommes_colonnes`, dois-tu initialiser `resultats[j]` à 0
  avant de commencer à accumuler ? Pourquoi (pas) ?
- **P3** — Après avoir mis à jour un des deux capteurs dans ton `main`,
  pourquoi l'autre reste-t-il inchangé ?
- **P4** — Pourquoi ta fonction a-t-elle besoin d'un paramètre `n`, même si
  l'énoncé montre `creer_tableau()` sans paramètre visible ?
- **P5** — Où et combien de fois appelles-tu `srand()` dans ton
  programme ?
- **P6** — Combien de distances "consécutives" calcules-tu pour 5 points ?
  Jusqu'où va ta boucle, et pourquoi pas plus loin ?
- **Station** — `station_creer` retourne-t-elle la station par valeur ou
  via un pointeur ? Quelle est la conséquence sur les pointeurs internes ?

## Étudiant 11
- **P1** — Que retourne ta fonction `somme_tableau` si `n = 0` ? Est-ce
  cohérent avec ce qu'on attendrait ?
- **P2** — Une colonne est-elle contiguë en mémoire dans un tableau 2D en
  C ? Comment cela influence-t-il l'ordre de tes boucles dans
  `sommes_colonnes` ?
- **P3** — Que contiendrait `capteur->titre` si tu avais oublié
  complètement l'étape de copie ?
- **P4** — As-tu réutilisé une fonction d'affichage d'un problème
  précédent ? Pourquoi est-ce possible ici ?
- **P5** — Comment as-tu obtenu des valeurs aléatoires entre 20 et 80
  exactement ?
- **P6** — Comment calcules-tu la distance totale ?
- **Station** — As-tu initialisé les pointeurs de ta structure à `NULL`
  avant de tenter les allocations ? Pourquoi est-ce utile ?

## Étudiant 12
- **P1** — Comment as-tu géré l'initialisation de l'accumulateur de somme,
  et pourquoi c'est nécessaire ?
- **P2** — Pourquoi les tableaux `resultats` pour les lignes et pour les
  colonnes n'ont pas nécessairement la même taille ?
- **P3** — Qu'est-ce qu'une "copie profonde" et pourquoi l'énoncé insiste
  là-dessus pour `titre` et `unite` ?
- **P4** — Que se passerait-il si on appelait `free` deux fois de suite sur
  le même pointeur ?
- **P5** — Peux-tu expliquer, étape par étape, ce que représente
  `matrice[i][j]` dans ta structure ?
- **P6** — Que se passerait-il si ta boucle allait un indice trop loin ?
- **Station** — Que fait ton code si une allocation échoue en cours de
  route ?

## Étudiant 13
- **P1** — Si plusieurs éléments du tableau partagent la valeur maximale,
  quel indice ton `indice_max` retourne-t-il ? Pourquoi ?
- **P2** — Que se passerait-il si on appelait tes fonctions avec
  `lignes = 0` ?
- **P3** — Qu'est-ce qu'une "copie profonde" et pourquoi l'énoncé insiste
  là-dessus pour `titre` et `unite` ?
- **P4** — Pourquoi utilises-tu `malloc(n * sizeof(int))` plutôt que
  `malloc(n)` ?
- **P5** — Pourquoi ne peux-tu pas réutiliser `afficher_matrice` du
  problème 2 pour afficher cette matrice-ci ?
- **P6** — Pourquoi ta fonction `distance` reçoit-elle des pointeurs plutôt
  que les points par valeur ?
- **Station** — Comment as-tu initialisé les capteurs prédéfinis
  (température, pression, humidité) ?

## Étudiant 14
- **P1** — Que retournerait ton `indice_max` si `n = 0` ? Ce résultat
  est-il valide/utilisable pour l'appelant ?
- **P2** — Comment as-tu géré l'alignement des colonnes dans
  `afficher_matrice` ?
- **P3** — Comment as-tu copié la chaîne de caractères reçue en paramètre
  dans le champ `titre` du capteur ? Pourquoi ne peux-tu pas simplement
  écrire `capteur->titre = titre;` ?
- **P4** — Que fais-tu si `malloc` échoue (retourne `NULL`) ? Pourquoi
  est-ce important de vérifier ?
- **P5** — Décris la structure mémoire de ta matrice : combien d'appels à
  `malloc` fais-tu au total pour une matrice de 5 lignes ?
- **P6** — Pourquoi ta fonction `distance` reçoit-elle des pointeurs plutôt
  que les points par valeur ?
- **Station** — Dans quel ordre libères-tu les différents éléments de la
  station, et pourquoi cet ordre est important ?

## Étudiant 15
- **P1** — Comment as-tu évité d'avoir un espace après le dernier élément
  dans `afficher_tableau` ? Explique ta technique.
- **P2** — Explique comment `sommes_lignes` calcule la somme de chaque
  ligne. As-tu besoin d'initialiser `resultats[i]` avant la boucle ?
- **P3** — Que se passe-t-il dans ton implémentation si le titre fourni
  dépasse 19 caractères ?
- **P4** — Qui est responsable d'appeler `free` sur le tableau retourné ?
  Pourquoi pas la fonction `creer_tableau` elle-même ?
- **P5** — Tes lignes sont-elles garanties d'être contiguës en mémoire les
  unes par rapport aux autres ? Pourquoi (pas) ?
- **P6** — Que calcules-tu juste avant d'appliquer la racine carrée, et
  pourquoi ?
- **Station** — Comment `station_enregistrer_mesure` sait-elle combien de
  valeurs contient le tableau `valeurs[]` reçu en paramètre ?

## Étudiant 16
- **P1** — Pourquoi tes fonctions prennent-elles `n` en paramètre plutôt
  que de calculer la taille du tableau avec `sizeof` à l'intérieur ?
- **P2** — Que représente le `5` dans la signature `int mat[][5]` ?
  Pourquoi le compilateur en a-t-il besoin, mais pas du nombre de lignes ?
- **P3** — Comment t'assures-tu que la chaîne copiée est bien terminée par
  `'\0'` dans tous les cas ?
- **P4** — Que se passe-t-il si on oublie d'appeler `free` dans le
  `main` ? Le programme plante-t-il ?
- **P5** — Si l'allocation d'une ligne échoue en cours de route, que fait
  ton code ? As-tu géré ce cas ?
- **P6** — As-tu utilisé `sqrt` ou `sqrtf` ? Pourquoi ce choix est-il
  cohérent avec le type de tes champs `x`/`y` ?
- **Station** — Que représente le nombre de colonnes de ta matrice
  d'historique, et à quel moment est-il fixé ?

## Étudiant 17
- **P1** — Quelle est la complexité (nombre d'opérations en fonction de
  `n`) de `somme_tableau` et `indice_max` ?
- **P2** — Pour `sommes_colonnes`, dois-tu initialiser `resultats[j]` à 0
  avant de commencer à accumuler ?
- **P3** — Pourquoi `capteur_mise_a_jour` reçoit-elle (généralement)
  seulement la nouvelle valeur mesurée et pas les autres champs ?
- **P4** — Pourquoi ta fonction a-t-elle besoin d'un paramètre `n`, même si
  l'énoncé montre `creer_tableau()` sans paramètre visible ?
- **P5** — Dans quel ordre libères-tu les lignes et le tableau de
  pointeurs dans `liberer_matrice` ? Pourquoi cet ordre est important ?
- **P6** — Pourquoi ton `main` utilise-t-il un tableau statique plutôt que
  dynamique pour les 5 points ?
- **Station** — `station_creer` retourne-t-elle la station par valeur ou
  via un pointeur ? Quelle est la conséquence sur les pointeurs internes ?

## Étudiant 18
- **P1** — As-tu testé `indice_max` ou `somme_tableau` avec un `n` négatif
  ou une valeur invalide ? Que se passe-t-il dans ton code ?
- **P2** — Pourquoi les tableaux `resultats` pour les lignes et pour les
  colonnes n'ont pas nécessairement la même taille ?
- **P3** — Pourquoi passes-tu un pointeur vers le capteur (plutôt que le
  capteur par valeur) à tes fonctions ?
- **P4** — Que se passerait-il si on appelait `free` deux fois de suite sur
  le même pointeur ?
- **P5** — Pourquoi passes-tu `colones` à `creer_matrice`, mais
  `liberer_matrice` n'en a probablement pas besoin ?
- **P6** — Comment calcules-tu la distance totale ?
- **Station** — As-tu initialisé les pointeurs de ta structure à `NULL`
  avant de tenter les allocations ? Pourquoi est-ce utile ?

## Étudiant 19
- **P1** — Pourquoi le `main` initialise-t-il le tableau directement dans
  le code plutôt que de le lire depuis l'entrée standard ?
- **P2** — Comment as-tu géré l'alignement des colonnes dans
  `afficher_matrice` ?
- **P3** — Après avoir mis à jour un des deux capteurs dans ton `main`,
  pourquoi l'autre reste-t-il inchangé ?
- **P4** — As-tu réutilisé une fonction d'affichage d'un problème
  précédent ? Pourquoi est-ce possible ici ?
- **P5** — Où et combien de fois appelles-tu `srand()` dans ton
  programme ?
- **P6** — Comment calcules-tu la distance totale ?
- **Station** — Que fait ton code si une allocation échoue en cours de
  route ?

---

*Généré à partir de la banque complète dans `QUESTIONS_LAB2.md`. Pour une
question de secours si un.e étudiant.e a manifestement discuté avec un.e
voisin.e déjà passé.e, pige n'importe quelle autre question du même
problème dans `QUESTIONS_LAB2.md`.*
