# Questions de vérification — Mini-app (Moniteur de batterie)

But : vérifier que l'étudiant.e comprend SON code (peu importe comment il/elle
l'a écrit), pas juste qu'il fonctionne. Ces questions sont formulées pour
s'appliquer à n'importe quelle implémentation correcte — demande à
l'étudiant.e d'expliquer avec SES propres variables et SA propre logique.
Piger 2-3 questions selon le temps disponible.

Chaque question est suivie de la **réponse attendue** (pour le ou la chargé.e
de labo). Une formulation différente est acceptable si l'idée est juste.

## Composition de modules

1. Que contient ta `struct moniteur` ? Pourquoi des pointeurs vers
   `pile_t`, `buffer_t` et `filtre_t` plutôt que les structures elles-mêmes ?
   > Trois pointeurs (pile, historique, filtre) et le seuil. Ce sont des types
   > opaques : `moniteur.c` ne connaît pas leur taille (type incomplet), il ne
   > peut donc pas les déclarer par valeur. Il passe par les fonctions
   > `*_creer` qui les allouent.

2. Dans `moniteur.c`, peux-tu écrire `m->pile->charge` ? Pourquoi ?
   > Non : `struct pile` est définie dans `pile.c`, invisible depuis
   > `moniteur.c`. Il faut passer par `pile_pourcentage`, etc. Le moniteur
   > est un client des modules du labo 3, comme le `main` l'est du moniteur.

3. Qui est responsable de libérer la pile, le buffer et le filtre ? Dans
   quel ordre libères-tu, et pourquoi la structure du moniteur en dernier ?
   > Le moniteur (il les a créés). Les sous-modules d'abord, puis `m` : après
   > `free(m)`, lire `m->pile` serait un accès à de la mémoire libérée.

## Création et échec partiel

4. Trace ta fonction : `moniteur_creer(1000, 3.7, 0, 4, 25)`. Qu'est-ce qui
   a été alloué avant l'échec, et qu'est-ce qui est libéré ?
   > `malloc` du moniteur et `pile_creer` réussissent, `buffer_creer(0)`
   > retourne NULL. Il faut libérer la pile ET la structure du moniteur
   > avant de retourner NULL (sinon fuite). Selon l'implémentation, le filtre
   > n'est pas créé, ou il l'est et doit aussi être libéré.

5. Ce cas d'échec est-il visible dans les tests Unity si tu oublies de
   libérer ? Comment pourrais-tu le détecter ?
   > Non : une fuite ne fait échouer aucune assertion. Il faut un outil
   > (Valgrind sous Linux, Dr. Memory sous Windows, `-fsanitize=address` avec
   > le détecteur de fuites) ou une relecture du code.

6. (Si l'étudiant.e réutilise `moniteur_detruire` dans `moniteur_creer`.)
   Pourquoi est-ce correct d'appeler `moniteur_detruire` sur un moniteur
   dont certains sous-modules sont NULL ?
   > Parce que `pile_detruire`, `buffer_detruire` et `filtre_detruire` sont
   > sans effet sur NULL (c'était une exigence du labo 3). Sans cette
   > garantie, il faudrait un nettoyage en cascade dans `moniteur_creer`.

## Historique et filtre

7. Le buffer du labo 3 refuse d'enfiler quand il est plein. Que fait ton
   `moniteur_enregistrer` après la 10e lecture (historique de 10) ?
   > Il retire la plus ancienne (`buffer_defiler`) puis enfile la nouvelle :
   > l'historique garde les 10 dernières. Si l'étudiant.e ignore le retour
   > de `buffer_enfiler`, l'historique reste figé sur les 10 premières
   > lectures (vérifier dans l'affichage final de la simulation : la dernière
   > valeur doit correspondre à la charge brute finale).

8. Le buffer stocke des `int` mais le pourcentage est un `float`. Comment
   fais-tu la conversion, et est-ce que le filtre reçoit la même valeur ?
   > Cast (troncature, 99.6 → 99) ou arrondi (`lroundf`, 99.6 → 100) pour
   > l'historique. Le filtre prend un `float` : lui donner la valeur exacte
   > évite de perdre de la précision. Lui donner l'`int` est accepté mais
   > moins précis.

9. Pourquoi `moniteur_charger` ne doit-il pas enregistrer de lecture ?
   > L'énoncé sépare l'action sur la pile de la mesure : on peut charger
   > plusieurs fois entre deux mesures. `enregistrer` représente
   > l'échantillonnage (ex. une lecture par période).

## Charge lissée et alerte

10. Calcule à la main : fenêtre 4, seuil 25 %, lectures 37.5, 37.5, 37.5,
    37.5, puis 12.5, 12.5. Quelle est la charge lissée et l'état de
    l'alerte après chacune des deux dernières lectures ?
    > (3 * 37.5 + 12.5) / 4 = 31.25 → pas d'alerte. (2 * 37.5 + 2 * 12.5) / 4
    > = 25 → pas d'alerte (25 n'est pas strictement inférieur à 25).

11. Pourquoi l'alerte réagit-elle en retard par rapport à la charge brute ?
    Est-ce un défaut ?
    > C'est l'effet voulu du filtre passe-bas : la moyenne mobile atténue le
    > bruit (une lecture isolée basse ne déclenche pas l'alerte) au prix d'un
    > retard d'environ la moitié de la fenêtre. Dans la simulation, la charge
    > brute passe sous 20 % à l'étape 18, l'alerte n'arrive qu'à l'étape 20.

12. Que retourne `moniteur_alerte` avant le premier `moniteur_enregistrer` ?
    > Le filtre du labo 3 retourne 0 sans échantillon, donc charge lissée 0
    > et alerte 1 (même si la pile est pleine). C'est une conséquence directe
    > de l'énoncé ; l'important est que l'étudiant.e sache l'expliquer.

13. Pourquoi `moniteur_charge_lissee` peut-elle prendre un
    `const moniteur_t *` alors qu'elle appelle une fonction du filtre ?
    > Parce que `filtre_valeur` prend elle-même un `const filtre_t *` (labo 3).
    > Si un accesseur du labo 3 n'était pas `const`, le compilateur
    > avertirait ici.

## Points de vigilance

- Historique qui ignore le retour 0 de `buffer_enfiler` quand il est plein
  (figé sur les premières lectures). Les tests Unity ne le voient pas : le
  buffer n'a pas de fonction pour consulter sans retirer. Regarder
  l'affichage final de la simulation (réf. : `[49, 44, 40, 35, 31, 26, 21,
  17, 12, 7]`).
- Fuite de la pile (ou du buffer) quand un sous-module suivant échoue dans
  `moniteur_creer`. Invisible dans Unity : lire le code.
- Alerte calculée sur la charge brute au lieu de la charge lissée.
- `<=` au lieu de `<` pour l'alerte.
- `charger` / `decharger` qui enregistrent aussi une lecture.
- Modules du labo 3 recopiés et modifiés (ex. structure rendue publique pour
  y accéder directement) : contraire à la réutilisation demandée.
- Makefile qui n'ajoute pas les `-I` et les `.c` des modules du labo 3.
- `main` sans fonction d'affichage d'une lecture (l'énoncé la demande
  explicitement), ou qui ne libère pas le moniteur à la fin.

## Simulation de référence

Pile 5400 mAh, 11.7 V, historique 10, fenêtre 5, seuil 20 %, 20 décharges de
250 mAh : charge brute finale 7.41 %, lissée 16.67 %, alerte à partir de
l'étape 20 (brute sous 20 % dès l'étape 18). Avec des décharges d'une autre
taille, les valeurs changent, mais le retard de l'alerte doit être visible.
