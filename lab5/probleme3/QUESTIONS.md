# Questions de vérification — Problème 3 (Séquenceur de commandes)

But : vérifier que l'étudiant.e comprend SON code (peu importe comment il/elle
l'a écrit), pas juste qu'il fonctionne. Demande à l'étudiant.e d'expliquer
avec SES propres variables et SA propre logique. Piger 2-3 questions selon
le temps disponible.

Chaque question est suivie de la **réponse attendue** (pour le ou la chargé.e
de labo). Une formulation différente est acceptable si l'idée est juste.

## Organisation des données

1. Où est stocké le contexte courant : dans la Pile ou ailleurs ? Que
   contient la Pile juste après `sequenceur_creer` ?
   > Ailleurs (ici `config_t courant` par valeur dans la structure). La Pile
   > est vide : elle ne contient que les copies sauvegardées par CMD_PUSH.
   > C'est ce qu'implique l'énoncé : « POP sur pile vide » doit être possible
   > alors qu'il existe toujours un contexte courant. Une solution « le
   > sommet de la Pile est le contexte courant » est défendable seulement si
   > l'étudiant.e explique comment il/elle traite le POP du dernier élément.

2. `sequenceur_enfiler` reçoit `commande_t cmd` par valeur. Pourquoi ne
   peut-on pas faire `file_gen_enfiler(s->commandes, &cmd)` ?
   > `cmd` est une variable locale (paramètre) qui disparaît au retour de la
   > fonction : la File garderait un pointeur vers une zone de pile
   > réutilisée (pointeur pendant). Il faut `malloc` une copie. Même chose
   > pour le `config_t` empilé par CMD_PUSH.

3. Au CMD_PUSH, pourquoi empiler une COPIE du contexte et non `&s->courant` ?
   > Avec `&s->courant`, la Pile pointerait vers le contexte courant
   > lui-même : les SET suivants modifieraient aussi la « sauvegarde », et le
   > POP ne restaurerait rien. Le test PUSH → SET → POP le détecte. (Et
   > `free` sur `&s->courant` planterait.)

## Gestion de la mémoire

4. Qui libère les commandes ? À quels deux moments ?
   > Le séquenceur en est propriétaire. (a) Dans `executer_prochaine`, après
   > l'exécution de la commande défilée. (b) Dans `sequenceur_detruire`, pour
   > les commandes encore en attente : il faut vider la File élément par
   > élément, car `file_gen_liberer` → `vecteur_liberer` ne libère que le
   > tableau de pointeurs, pas les éléments pointés. Idem pour les contextes
   > encore dans la Pile.

5. Au CMD_POP, après `s->courant = *sauvegarde;`, que fais-tu du pointeur
   dépilé ? Pourquoi est-ce sans danger ?
   > `free(sauvegarde)`. L'affectation de structure a COPIÉ les trois float
   > dans `s->courant` ; on n'a plus besoin de la zone allouée.

## Robustesse face au code du cours

6. Que se passe-t-il si on appelle `pile_gen_depiler` sur une pile vide avec
   le code du cours ? Comment l'as-tu évité ?
   > `vecteur_retirer_dernier` n'a pas d'assert : la taille devient -1 et on
   > lit `contenu[-1]` (comportement indéfini). On vérifie
   > `pile_gen_est_vide` avant et on affiche un avertissement. Même problème
   > pour `file_gen_defiler` sur une file vide → on retourne 0 avant.

7. (Bonus) `pile_gen_empiler` ignore la valeur de retour de
   `vecteur_ajouter`. Quelle conséquence si le `realloc` échoue ?
   > L'élément n'est pas ajouté mais l'appelant ne le sait pas : la copie
   > allouée fuit. Solution possible sans modifier le cours : comparer la
   > taille avant/après et libérer la copie si elle n'a pas changé.

## const et affichage

8. `sequenceur_afficher_file` reçoit un `const sequenceur_t *`, mais la File
   du cours ne permet de voir que sa tête. Comment parcours-tu la File ?
   Ton code modifie-t-il la File ?
   > Rotation : défiler puis réenfiler n fois ; après un tour complet, même
   > contenu dans le même ordre. Oui, la File est modifiée temporairement.
   > Ça compile parce que le `const` est superficiel : `s->commandes` devient
   > `file_gen_t * const` (pointeur constant), la File pointée n'est pas
   > const. Le test vérifie que la taille et l'ordre sont conservés.

9. `sequenceur_contexte` retourne `const config_t *`. Pourquoi const ?
   Le pointeur reste-t-il valide après l'exécution d'autres commandes ?
   > Pour que l'appelant puisse lire mais pas modifier le contexte sans
   > passer par une commande. Oui, il pointe vers `s->courant` qui ne bouge
   > pas (jusqu'à `sequenceur_detruire`) ; seules les valeurs changent.

## Aléatoire et tests

10. Comment génères-tu le bruit de ±10 % ? Que donne ta formule quand
    `rand()` vaut 0 ? Quand il vaut `RAND_MAX` ?
    > Ex. `(2 * rand()/RAND_MAX - 1) * 0.10` : -10 % pour 0, +10 % pour
    > RAND_MAX. Piège : `rand() / RAND_MAX` en division ENTIÈRE vaut presque
    > toujours 0 → il faut convertir en float avant de diviser.

11. Pourquoi appeler `srand()` dans le test et pas dans le module ? Ton test
    de CMD_MESURER compare-t-il une valeur exacte ?
    > Le module ne doit pas décider du seed : le test le fixe pour être
    > reproductible, le programme final pourra utiliser `srand(time(NULL))`.
    > La suite de `rand()` dépend de la bibliothèque C (Windows ≠ Linux), donc
    > un bon test vérifie des propriétés (mesure dans ±10 %, même seed → même
    > mesure, attendu et tolérance corrects) plutôt qu'une valeur codée en dur.

12. CMD_POP sur une pile vide : que retourne `executer_prochaine` ?
    > 1 (choix de cette solution) : la commande a été défilée et consommée,
    > même si elle a été ignorée. Retourner 0 ferait arrêter
    > `executer_tout` avant la fin de la file. L'important est que
    > l'étudiant.e ait un choix justifié et cohérent avec ses tests.
