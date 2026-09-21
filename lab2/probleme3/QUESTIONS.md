# Questions de vérification — Problème 3 (Structures)

But : vérifier que l'étudiant.e comprend SON code, pas juste qu'il
fonctionne. À partir de ce problème, l'énoncé ne impose plus les
paramètres des fonctions — les questions ci-dessous s'adressent à
n'importe quel choix de signature. Piger 2-3 questions selon le temps
disponible.

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
   caractères (`strcpy`, `strncpy`, `snprintf`, boucle manuelle, ...) ?
   Quel est le risque si on utilisait `strcpy` sans aucune précaution ?
9. Que contiendrait `capteur->titre` si tu avais oublié complètement
   l'étape de copie ?

## Points de vigilance (indépendants de l'implémentation)

- `titre`/`unite` sont des `char[N]` internes à la struct : le capteur doit
  posséder sa propre copie, peu importe la fonction utilisée pour copier.
- Une chaîne mal terminée (`'\0'` manquant) cause une lecture hors limites
  lors d'un affichage ultérieur — vérifier que l'étudiant.e y a pensé, quel
  que soit son choix de fonction de copie.
- Passer un pointeur (et non la struct par valeur) est nécessaire pour que
  les fonctions puissent modifier le capteur original.
