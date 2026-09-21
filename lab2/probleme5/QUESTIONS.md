# Questions de vérification — Problème 5 (Tableaux 2D dynamiques : allocation ligne par ligne)

But : vérifier que l'étudiant.e comprend SON code, pas juste qu'il
fonctionne. C'est le problème le plus important conceptuellement du labo —
ne pas hésiter à demander un dessin au tableau si l'étudiant.e bloque sur
la structure mémoire (l'énoncé lui-même invite à venir voir le chargé de
lab si la structure n'est pas claire). Piger 2-3 questions selon le temps
disponible.

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
   dans ta structure (quel est le type de `matrice[i]` seul) ?
9. Pourquoi ne peux-tu pas réutiliser `afficher_matrice` du problème 2 pour
   afficher cette matrice-ci ?

## Points de vigilance (indépendants de l'implémentation)

- La bonne réponse en mémoire : `lignes + 1` allocations (1 tableau de
  pointeurs + 1 par ligne), peu importe les noms de variables choisis.
- Libérer chaque ligne AVANT le tableau de pointeurs — l'inverse fait
  perdre les adresses des lignes (fuite irrécupérable), peu importe la
  syntaxe exacte utilisée.
- `srand()` ne doit être appelée qu'UNE FOIS, avant toute utilisation de
  `rand()` — peu importe où précisément dans le code, tant que ce n'est pas
  répété dans une boucle.
- Le type `int mat[][5]` (problème 2) et `int **` (ce problème) ne sont PAS
  interchangeables — bonne question de compréhension peu importe
  l'implémentation exacte.
