# Questions de vérification — Problème 2 (Journal de résultats)

But : vérifier que l'étudiant.e comprend SON code (peu importe comment il/elle
l'a écrit), pas juste qu'il fonctionne. Demande à l'étudiant.e d'expliquer
avec SES propres variables et SA propre logique. Piger 2-3 questions selon
le temps disponible.

Chaque question est suivie de la **réponse attendue** (pour le ou la chargé.e
de labo). Une formulation différente est acceptable si l'idée est juste.

## Le Vecteur générique

1. Que contient réellement ton Vecteur : des `resultat_t` ou des pointeurs ?
   Pourquoi le Vecteur du cours est-il dit « générique » ?
   > Des `void *` qui pointent vers des `resultat_t` alloués par
   > `resultat_creer`. Le Vecteur ne connaît pas le type des éléments : il
   > stocke des adresses. C'est le journal qui sait qu'il faut les
   > reconvertir en `resultat_t *` (conversion implicite depuis `void *`).

2. Pourquoi un Vecteur plutôt qu'un tableau fixe `resultat_t *tab[100]` ?
   > Le nombre de tests n'est pas connu à l'avance. Le Vecteur double sa
   > capacité au besoin (`agrandir` → `realloc`). Le test d'ajout de 10
   > résultats dépasse la capacité initiale et vérifie que rien n'est perdu.

3. Pendant un `realloc` du Vecteur, le tableau interne peut changer
   d'adresse. Les `resultat_t` déménagent-ils aussi ? Un pointeur obtenu
   par `journal_obtenir` avant l'agrandissement reste-t-il valide ?
   > Non, seul le tableau de POINTEURS est déplacé ; les `resultat_t` restent
   > où `malloc` les a mis. Donc le pointeur reste valide (jusqu'à
   > `journal_detruire`). Ce serait différent si on stockait les structures
   > par valeur dans le tableau.

## Propriété et libération de la mémoire

4. Qui libère les `resultat_t` ? Que fait `vecteur_liberer` avec eux ?
   > Le journal : `journal_detruire` parcourt le Vecteur et appelle
   > `resultat_detruire` sur chacun, PUIS `vecteur_liberer`, PUIS `free(j)`.
   > `vecteur_liberer` ne libère que son tableau interne (« la libération des
   > éléments est la responsabilité de l'appelant »). Sans la boucle, tous
   > les résultats fuient.

5. Pourquoi cet ordre précis dans `journal_detruire` ?
   > Après `vecteur_liberer`, on ne peut plus lire le Vecteur pour retrouver
   > les résultats ; après `free(j)`, on ne peut plus lire `j->resultats`.
   > On libère donc de l'intérieur vers l'extérieur.

6. Dans `journal_creer`, que se passe-t-il si `vecteur_creer` échoue ?
   > Il faut faire `free(j)` avant de retourner NULL, sinon la structure du
   > journal fuit. (Le `pile_creer` du cours 4 a justement ce défaut.)

7. Que fait ton `journal_ajouter` si `resultat_creer` retourne NULL (id
   NULL) ? Et si `vecteur_ajouter` retourne 0 ?
   > Si NULL : ne rien ajouter (le `vecteur_ajouter` du cours fait
   > `assert(valeur != NULL)` → le programme s'arrêterait). Si 0 :
   > l'agrandissement a échoué, il faut détruire le résultat qu'on vient de
   > créer, sinon il fuit.

8. Comment vérifies-tu « création et destruction sans fuite » ? Unity
   peut-il le détecter ?
   > Non, Unity ne voit pas les fuites. Il faut un outil : valgrind (Linux),
   > Dr. Memory (Windows), ou `-fsanitize=address` (pas disponible avec
   > MinGW). Le test fournit le scénario (créer, remplir, détruire) ; la
   > réponse attendue est surtout de savoir l'expliquer en traçant les `free`.

## Interface et const

9. Pourquoi `journal_obtenir` retourne-t-il un `const resultat_t *` ?
   > Le journal reste propriétaire : l'appelant peut lire mais ne doit ni
   > modifier ni détruire le résultat. Avec `const`, le compilateur avertit
   > si on passe ce pointeur à `resultat_detruire` (qui prend un non-const).

10. `vecteur_obtenir` fait un `assert` sur la position. Comment ton
    `journal_obtenir` retourne-t-il NULL au lieu de planter ? Pour un
    journal de 3 éléments, quels index sont valides ?
    > Valider avant d'appeler : `if (index < 0 || index >= taille) return NULL;`.
    > Valides : 0, 1, 2. L'index 3 (= taille) est invalide : `>` au lieu de
    > `>=` est l'erreur classique.

11. Comptes-tu les réussis/échoués à chaque appel ou gardes-tu des
    compteurs dans la structure ? Avantages de chaque approche ?
    > Les deux sont acceptables. Recompter (boucle O(n)) : aucun risque
    > d'incohérence. Compteurs (O(1)) : il faut les mettre à jour à CHAQUE
    > ajout réussi seulement, pas quand l'ajout échoue. L'invariant
    > réussis + échoués = total doit tenir dans les deux cas.

12. Comment évites-tu de dupliquer le code entre `journal_afficher` et
    `journal_sauvegarder` ?
    > Une fonction `static` qui écrit le rapport dans un `FILE *` :
    > `stdout` pour afficher, le fichier ouvert pour sauvegarder. `printf(...)`
    > est équivalent à `fprintf(stdout, ...)`. (Accepter la duplication, mais
    > c'est une bonne question de revue de code.)

13. Que se passe-t-il si `fopen` échoue dans `journal_sauvegarder` ?
    Qu'arrive-t-il si on oublie `fclose` ?
    > `fopen` retourne NULL (dossier inexistant, pas de droits) : il faut le
    > vérifier, sinon `fprintf(NULL, ...)` plante. Sans `fclose`, le tampon
    > n'est pas forcément écrit sur disque (fichier vide ou tronqué) et le
    > descripteur fuit.

## Points de vigilance (erreurs fréquentes)

- `journal_detruire` qui appelle seulement `vecteur_liberer` : tous les
  `resultat_t` fuient.
- Libérer dans le mauvais ordre (`free(j)` puis `j->resultats`).
- `journal_creer` qui fuit la structure si `vecteur_creer` échoue.
- `journal_ajouter` qui passe NULL au Vecteur (assert) ou qui fuit le
  résultat si l'ajout échoue.
- `index > taille` au lieu de `index >= taille`, ou oubli de `index < 0`
  (le `assert` de `vecteur_obtenir` arrête alors le programme).
- Retourner `resultat_t *` non-const depuis `journal_obtenir`.
- `journal_detruire(NULL)` : le `vecteur_liberer` du cours fait un `assert`
  sur NULL, donc il faut tester `j == NULL` avant.
- `fprintf` sans vérifier le retour de `fopen` ; oubli de `fclose`.
- Makefile : oublier `../commun/vecteur_gen.c` et `resultat.c` dans `SRC`
  (erreur « undefined reference » à l'édition de liens) ou `-I../commun`
  dans `INCLUDES` (erreur « vecteur_gen.h: No such file »).
