# Labo 2 — Banque de questions de vérification (tous les problèmes)

Compilation de toutes les questions de vérification (`QUESTIONS.md` de
chaque sous-dossier de `lab2/`). Formulées de façon **générique** pour
s'appliquer à n'importe quelle implémentation correcte — pas de référence à
des noms de variables ou des techniques précises (ex. `snprintf` vs
`strncpy`), sauf quand l'énoncé lui-même impose une signature (problèmes 1
et 2). Voir `ELE216_laboratoire_2.pdf` pour l'énoncé complet.

But : vérifier que l'étudiant.e comprend son code, pas juste qu'il
fonctionne (voir `consigne_charger_lab.md`). Piger 2-3 questions par
étudiant.e selon le temps disponible.

---

## Problème 1 — Tableaux 1D

1. Que retourne ta fonction `somme_tableau` si `n = 0` ? Est-ce cohérent
   avec ce qu'on attendrait ?
2. Comment as-tu géré l'initialisation de l'accumulateur de somme, et
   pourquoi c'est nécessaire ?
3. Si plusieurs éléments du tableau partagent la valeur maximale, quel
   indice ton `indice_max` retourne-t-il ? Pourquoi ?
4. Que retournerait ton `indice_max` si `n = 0` ? Ce résultat est-il
   valide/utilisable pour l'appelant ?
5. Comment as-tu évité d'avoir un espace après le dernier élément dans
   `afficher_tableau` ? Explique ta technique.
6. Pourquoi tes fonctions prennent-elles `n` en paramètre plutôt que de
   calculer la taille du tableau avec `sizeof` à l'intérieur ?
7. Quelle est la complexité (nombre d'opérations en fonction de `n`) de
   `somme_tableau` et `indice_max` ?
8. As-tu testé `indice_max` ou `somme_tableau` avec un `n` négatif ou une
   valeur invalide ? Que se passe-t-il dans ton code ?
9. Pourquoi le `main` initialise-t-il le tableau directement dans le code
   plutôt que de le lire depuis l'entrée standard ?
10. Peux-tu expliquer, ligne par ligne, ce que fait ta boucle dans
    `indice_max` ?

---

## Problème 2 — Tableaux 2D

1. Que représente le `5` dans la signature `int mat[][5]` ? Pourquoi le
   compilateur en a-t-il besoin, mais pas du nombre de lignes ?
2. Comment as-tu géré l'alignement des colonnes dans `afficher_matrice` ?
3. Explique comment `sommes_lignes` calcule la somme de chaque ligne.
   As-tu besoin d'initialiser `resultats[i]` avant la boucle ? Pourquoi
   (pas) ?
4. Pour `sommes_colonnes`, dois-tu initialiser `resultats[j]` à 0 avant de
   commencer à accumuler ? Pourquoi (pas), et est-ce différent de
   `sommes_lignes` ?
5. Une colonne est-elle contiguë en mémoire dans un tableau 2D en C ?
   Comment cela influence-t-il l'ordre de tes boucles dans
   `sommes_colonnes` ?
6. Pourquoi les tableaux `resultats` pour les lignes et pour les colonnes
   n'ont pas nécessairement la même taille ?
7. Que se passerait-il si on appelait tes fonctions avec `lignes = 0` ?
8. Pourquoi le nombre de colonnes doit-il être connu à la compilation dans
   la signature d'une fonction recevant un tableau 2D ?
9. Si tu devais adapter ton code pour une matrice à 8 colonnes au lieu de
   5, que devrais-tu changer ?

---

## Problème 3 — Structures

1. Qu'est-ce qu'une "copie profonde" et pourquoi l'énoncé insiste là-dessus
   pour `titre` et `unite` ?
2. Comment as-tu copié la chaîne de caractères reçue en paramètre dans le
   champ `titre` du capteur ? Pourquoi ne peux-tu pas simplement écrire
   `capteur->titre = titre;` ?
3. Que se passe-t-il dans ton implémentation si le titre fourni dépasse 19
   caractères (la capacité utile de `char[20]`) ?
4. Comment t'assures-tu que la chaîne copiée est bien terminée par `'\0'`
   dans tous les cas ?
5. Pourquoi `capteur_mise_a_jour` reçoit-elle (généralement) seulement la
   nouvelle valeur mesurée et pas les autres champs ?
6. Pourquoi passes-tu un pointeur vers le capteur (plutôt que le capteur
   par valeur) à tes fonctions ?
7. Après avoir mis à jour un des deux capteurs dans ton `main`, pourquoi
   l'autre reste-t-il inchangé ?
8. Quelle fonction (ou technique) as-tu utilisée pour copier les chaînes de
   caractères ? Quel est le risque si on utilisait `strcpy` sans aucune
   précaution ?
9. Que contiendrait `capteur->titre` si tu avais oublié complètement
   l'étape de copie ?

---

## Problème 4 — Tableaux dynamiques (allocation et libération)

1. Pourquoi utilises-tu `malloc(n * sizeof(int))` plutôt que `malloc(n)` ?
2. Que fais-tu si `malloc` échoue (retourne `NULL`) ? Pourquoi est-ce
   important de vérifier ?
3. Qui est responsable d'appeler `free` sur le tableau retourné ? Pourquoi
   pas la fonction `creer_tableau` elle-même ?
4. Que se passe-t-il si on oublie d'appeler `free` dans le `main` ? Le
   programme plante-t-il ?
5. Que se passerait-il si on appelait `free` deux fois de suite sur le
   même pointeur ?
6. Pourquoi ta fonction a-t-elle besoin d'un paramètre `n`, même si
   l'énoncé montre `creer_tableau()` sans paramètre visible dans sa
   signature simplifiée ?
7. As-tu réutilisé une fonction d'affichage d'un problème précédent ?
   Pourquoi est-ce possible ici ?

---

## Problème 5 — Tableaux 2D dynamiques (allocation ligne par ligne)

*C'est le problème le plus important conceptuellement du labo — ne pas
hésiter à demander un dessin au tableau si l'étudiant.e bloque.*

1. Décris la structure mémoire de ta matrice : combien d'appels à `malloc`
   fais-tu au total pour une matrice de 5 lignes ?
2. Tes lignes sont-elles garanties d'être contiguës en mémoire les unes par
   rapport aux autres ? Pourquoi (pas) ?
3. Si l'allocation d'une ligne échoue en cours de route, que fait ton
   code ? As-tu géré ce cas ?
4. Dans quel ordre libères-tu les lignes et le tableau de pointeurs dans
   `liberer_matrice` ? Pourquoi cet ordre est important ?
5. Pourquoi passes-tu `colones` à `creer_matrice`, mais `liberer_matrice`
   n'en a probablement pas besoin ?
6. Où et combien de fois appelles-tu `srand()` dans ton programme ?
   Pourquoi cet endroit précis ?
7. Comment as-tu obtenu des valeurs aléatoires entre 20 et 80 exactement ?
8. Peux-tu expliquer, étape par étape, ce que représente `matrice[i][j]`
   dans ta structure ?
9. Pourquoi ne peux-tu pas réutiliser `afficher_matrice` du problème 2 pour
   afficher cette matrice-ci ?

---

## Problème 6 — Tableau de structures

1. Pourquoi ta fonction `distance` reçoit-elle des pointeurs plutôt que les
   points par valeur ?
2. Que calcules-tu juste avant d'appliquer la racine carrée, et pourquoi ?
3. As-tu utilisé `sqrt` ou `sqrtf` ? Pourquoi ce choix est-il cohérent avec
   le type de tes champs `x`/`y` ?
4. Pourquoi ton `main` utilise-t-il un tableau statique plutôt que
   dynamique pour les 5 points ?
5. Combien de distances "consécutives" calcules-tu pour 5 points ?
   Jusqu'où va ta boucle, et pourquoi pas plus loin ?
6. Comment calcules-tu la distance totale ?
7. Que se passerait-il si ta boucle allait un indice trop loin ?

---

## Mini-app Station météo (synthèse)

*Combine tout le labo — bon endroit pour vérifier la compréhension
d'ensemble, pas juste un détail isolé.*

1. Pourquoi le tableau de capteurs et la matrice d'historique ne sont-ils
   pas alloués de la même façon ?
2. Que représente le nombre de colonnes de ta matrice d'historique, et à
   quel moment est-il fixé ?
3. `station_creer` retourne-t-elle la station par valeur ou via un
   pointeur ? Quelle est la conséquence sur les pointeurs internes ?
4. As-tu initialisé les pointeurs de ta structure à `NULL` avant de tenter
   les allocations ? Pourquoi est-ce utile ?
5. Que fait ton code si une allocation échoue en cours de route ?
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
    capteurs ?
11. Dans quel ordre libères-tu les différents éléments de la station, et
    pourquoi cet ordre est important ?
