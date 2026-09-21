# Questions de vérification — Problème 6 (Tableau de structures)

But : vérifier que l'étudiant.e comprend SON code, pas juste qu'il
fonctionne. Piger 2-3 questions selon le temps disponible.

1. Pourquoi ta fonction `distance` reçoit-elle des pointeurs plutôt que les
   points par valeur ?
2. Que calcules-tu juste avant d'appliquer la racine carrée, et pourquoi
   (quel théorème) ?
3. As-tu utilisé `sqrt` ou `sqrtf` ? Pourquoi ce choix est-il cohérent (ou
   pas) avec le type de tes champs `x`/`y` ?
4. Pourquoi ton `main` utilise-t-il un tableau statique plutôt que
   dynamique pour les 5 points ?
5. Combien de distances "consécutives" calcules-tu pour 5 points ?
   Jusqu'où va ta boucle, et pourquoi pas plus loin ?
6. Comment calcules-tu la distance totale ? La recalcules-tu séparément ou
   l'accumules-tu au fil de la boucle ?
7. Que se passerait-il si ta boucle allait un indice trop loin (ex. jusqu'à
   l'indice du dernier point plutôt que de s'arrêter avant) ?

## Points de vigilance (indépendants de l'implémentation)

- Passer des pointeurs (plutôt que la struct par valeur) évite de copier la
  struct à chaque appel — bon réflexe même si techniquement pas obligatoire
  ici.
- Pour 5 points, il y a 4 distances consécutives — une boucle qui va trop
  loin accède à un élément hors du tableau (comportement indéfini).
- Un tableau statique/local ne nécessite aucun `free` — vérifier que
  l'étudiant.e ne confond pas avec les problèmes 4/5.
