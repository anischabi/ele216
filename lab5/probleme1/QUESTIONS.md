# Questions de vérification — Problème 1 (Résultat de test)

But : vérifier que l'étudiant.e comprend SON code (peu importe comment il/elle
l'a écrit), pas juste qu'il fonctionne. Demande à l'étudiant.e d'expliquer
avec SES propres variables et SA propre logique. Piger 2-3 questions selon
le temps disponible.

Chaque question est suivie de la **réponse attendue** (pour le ou la chargé.e
de labo). Une formulation différente est acceptable si l'idée est juste.

## Type opaque et immuabilité

1. L'énoncé dit que le verdict « ne peut pas être modifié par la suite ».
   Qu'est-ce qui le garantit dans ton code ?
   > `struct resultat` est définie dans `resultat.c` seulement (type opaque) :
   > le code externe ne peut pas écrire `r->reussi = 1`. Et le module n'offre
   > aucun « setter ». Le verdict est calculé une seule fois, dans
   > `resultat_creer`.

2. Pourquoi stocker le verdict plutôt que le recalculer dans
   `resultat_est_reussi` ? Les deux sont-ils acceptables ?
   > Les deux donnent le même résultat puisque les champs ne changent jamais.
   > Le stocker suit l'énoncé (« calculé à la création ») et coûte un `int`.
   > Le recalculer évite une donnée redondante. Accepter l'un ou l'autre si
   > l'étudiant.e sait justifier.

## Gestion de l'identifiant

3. Comment stockes-tu `id` ? Que se passe-t-il si l'appelant passe un tampon
   local qu'il réécrit ensuite (ex. dans une boucle avec `snprintf`) ?
   > Copie profonde attendue : `malloc(strlen(id) + 1)` + `strcpy`/`memcpy`,
   > ou un tableau `char id[N]` dans la structure avec `snprintf`. Si on
   > garde seulement le pointeur (`r->id = id`), tous les résultats
   > partagent le tampon de l'appelant : ils changent quand il change, et
   > pointent dans le vide s'il est libéré (test `id_copie_profonde`).

4. Pourquoi `+ 1` dans `malloc(strlen(id) + 1)` ?
   > Pour le `'\0'` final, que `strlen` ne compte pas. Sans lui, la copie
   > déborde d'un octet.

5. (Si allocation dynamique de `id`) Que fait ton `resultat_creer` si le
   2e `malloc` (celui de `id`) échoue ? Et `resultat_detruire` : dans quel
   ordre libères-tu ?
   > Libérer la structure déjà allouée avant de retourner NULL (sinon fuite).
   > Dans `detruire`, libérer `r->id` AVANT `r` : après `free(r)`, lire
   > `r->id` est un accès à de la mémoire libérée.

6. (Si tableau fixe) Que se passe-t-il avec un id plus long que ton tableau ?
   > Avec `snprintf`/`strncpy` bien utilisés, il est tronqué (et terminé par
   > `'\0'` avec `snprintf`). Avec `strcpy`, débordement de tampon. C'est la
   > limite du tableau fixe ; l'allocation dynamique n'en a pas.

## Calcul du verdict

7. Écris ta condition de verdict. Trace-la pour attendu = 100, tolérance =
   5 %, mesure = 105, puis 94.
   > `fabsf(mesure - attendu) <= fabsf(attendu) * tolerance / 100`.
   > 105 : écart 5 <= 5 → PASS (borne incluse). 94 : écart 6 > 5 → FAIL.
   > Équivalent accepté : `mesure >= min && mesure <= max`.

8. Pourquoi `<=` et pas `<` ? Quel test le vérifie ?
   > L'énoncé donne un intervalle fermé `[ , ]`. Avec `<`, la mesure exacte
   > sur la borne échoue, et surtout avec tolérance 0 % même la valeur exacte
   > échoue (0 < 0 est faux). Tests `bornes_incluses` et `tolerance_zero`.

9. Que se passe-t-il avec attendu = 0 ? Est-ce un bug ?
   > L'écart permis vaut 0 peu importe le pourcentage, donc seul 0 exact
   > passe. Ce n'est pas un bug, c'est la définition de l'énoncé (cas limite
   > testé), mais c'est une limite réelle d'une tolérance relative : pour
   > mesurer un offset nul, il faudrait une tolérance absolue.

10. (Bonus) Que donne ta formule avec attendu = -12 V, tolérance 5 %,
    mesure -12.5 ?
    > Avec la formule littérale de l'énoncé, l'intervalle devient
    > [-12 + 0.6, -12 - 0.6] = [-11.4, -12.6] : min > max, donc tout échoue.
    > Avec `fabsf(attendu)`, l'intervalle est [-12.6, -11.4] → PASS. Pas exigé,
    > mais bonne question pour les étudiant.e.s rapides.

11. Pourquoi les tests utilisent-ils 100, 105, 95 plutôt que 3.3 et 3.465
    pour tester les bornes ?
    > 3.3 et 3.465 ne sont pas exactement représentables en `float` : le
    > calcul de la borne peut tomber juste au-dessus ou en dessous, et le test
    > devient dépendant de l'arrondi. 100 * 5 / 100 = 5 est exact.

## Tests

12. Ton test « tolérance de 0 % » vérifie-t-il les DEUX côtés (exact passe ET
    proche échoue) ? Pourquoi les deux ?
    > Il faut les deux : seulement « exact passe » ne détecte pas une
    > tolérance ignorée ; seulement « proche échoue » ne détecte pas `<`.

## Points de vigilance (erreurs fréquentes)

- `r->id = id;` (copie superficielle) au lieu d'une copie de la chaîne.
- `malloc(strlen(id))` sans le `+ 1` du `'\0'`.
- Vérifier `id == NULL` APRÈS `strlen(id)` → plantage.
- `<` au lieu de `<=` : la tolérance 0 % ne laisse plus rien passer.
- Tolérance utilisée comme valeur absolue (oubli de `* attendu / 100`).
- Division entière : `tolerance / 100` est correcte car `tolerance` est un
  `float`, mais `5 / 100` avec deux `int` vaut 0.
- Test d'un seul côté de l'intervalle (pas de valeur absolue sur l'écart).
- `free(r)` puis `free(r->id)` (ordre inversé).
- `resultat_detruire(NULL)` qui déréférence le pointeur.
- Tests aux bornes avec des valeurs non représentables en `float` qui
  passent ou échouent « par hasard ».
