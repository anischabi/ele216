# Questions de vérification — Mini-app Station météo

But : vérifier que l'étudiant.e comprend son code, pas juste qu'il fonctionne.
Piger 2-3 questions selon le temps disponible. Cette mini-app combine tout le
labo — bon endroit pour vérifier la compréhension d'ensemble, pas juste un
détail isolé.

## `station_t` — la structure globale

- Pourquoi `capteurs` est-il un tableau PLAT (`capteur_t *`) alors que
  `historique` est un tableau de pointeurs (`float **`) ?
  *(`capteur_t` a une taille fixe connue à la compilation, une seule
  allocation suffit (comme `creer_tableau` du problème 4). `historique`
  est une matrice 2D dont chaque ligne est allouée séparément (comme
  `creer_matrice` du problème 5).)*
- Que représente `nb_mesures` dans cette structure ?
  *(Le nombre de colonnes de `historique`, fixé une fois à la création par
  `station_creer` — pas un compteur qui change.)*

## `station_creer` — retour par valeur

- Pourquoi `station_creer` retourne-t-elle un `station_t` PAR VALEUR
  (`station_t station_creer(...)`) plutôt qu'un pointeur
  (`station_t *station_creer(...)`) ?
  *(Choix valide dans les deux cas ; ici, on suit le même patron que
  `creer_date_heure` du cours1 — la struct entière (avec ses pointeurs
  internes) est copiée dans la variable du `main`. Les pointeurs
  `capteurs`/`historique` copiés continuent de pointer vers la même
  mémoire allouée — pas de duplication du contenu.)*
- Que se passerait-il si on faisait `station_t s2 = s1;` après avoir créé
  `s1` ?
  *(Copie superficielle : `s2` et `s1` partageraient les mêmes pointeurs
  internes. Libérer l'une (`station_liberer(&s1)`) invaliderait l'autre —
  danger de double `free` ou d'utilisation après libération si on n'y
  prend pas garde.)*
- Pourquoi initialise-t-on `station.capteurs` et `station.historique` à
  `NULL` avant même de tenter les allocations ?
  *(Pour que si une allocation échoue et qu'on retourne tôt, les champs
  non encore alloués aient une valeur connue (`NULL`) plutôt
  qu'indéterminée — l'appelant peut alors vérifier facilement si la
  création a réussi.)*
- Si l'allocation de la ligne 3 (sur 5) de `historique` échoue, que fait
  le code avant de retourner ? Pourquoi cet ordre est-il important ?
  *(Libère les lignes 0 à 2 déjà allouées, puis le tableau de pointeurs
  `historique`, puis le tableau `capteurs` — dans cet ordre précis, pour
  n'oublier aucune fuite mémoire partielle.)*

## Initialisation des capteurs prédéfinis

- Pourquoi les 3 premiers capteurs sont-ils codés en dur (température,
  pression, humidité) plutôt que dans une boucle générique ?
  *(Plus simple et plus lisible pour un nombre fixe et connu de capteurs
  "spéciaux" ; on réutilise directement `capteur_init` du problème 3 pour
  chacun.)*
- Que se passe-t-il si `station_creer` est appelée avec `nb_capteurs = 1` ?
  *(Seul le capteur température est initialisé — les `if (nb_capteurs > N)`
  empêchent d'écrire dans des cases du tableau qui n'existent pas.)*

## `station_enregistrer_mesure`

- D'où vient la taille attendue du tableau `valeurs[]` passé en
  paramètre ? Comment la fonction le sait-elle ?
  *(Elle ne le "sait" pas directement — elle assume que `valeurs[]`
  contient au moins `s->nb_capteurs` éléments, dans le même ordre que
  `s->capteurs`. C'est une responsabilité de l'appelant, pas vérifiée par
  la fonction.)*
- Pourquoi appelle-t-on `capteur_mise_a_jour` en plus d'écrire dans
  `historique` ?
  *(Pour que le champ `derniere_valeur` de chaque capteur reflète toujours
  sa mesure la plus récente, même si on ne consulte que le capteur seul
  sans regarder l'historique.)*

## `station_moyenne_capteur` et `station_indice_valeur_max`

- Pourquoi ne peut-on pas réutiliser directement `indice_max` du
  problème 1 pour implémenter `station_indice_valeur_max` ?
  *(`indice_max` travaille sur un `int[]`, mais `historique` contient des
  `float`. Le C n'a pas de généricité (pas de templates) — il faut
  réécrire la même logique pour le type `float`.)*
- Que se passerait-il si on appelait `station_moyenne_capteur` avant
  d'avoir enregistré la moindre mesure ?
  *(Division par `nb_mesures`, qui vaut la capacité voulue — si aucune
  mesure n'a réellement été écrite, la moyenne inclurait des valeurs
  indéterminées (mémoire non initialisée) provenant de `historique`.)*

## `station_liberer`

- Pourquoi `historique` a-t-il besoin d'une boucle de `free` alors que
  `capteurs` n'a besoin que d'un seul `free` ?
  *(`historique` est un tableau de pointeurs vers des lignes allouées
  séparément — chacune doit être libérée individuellement, puis le
  tableau de pointeurs lui-même. `capteurs` est un bloc unique, alloué en
  une seule fois.)*
- Dans quel ordre doit-on libérer les lignes de `historique` et le
  tableau de pointeurs lui-même ? Pourquoi ?
  *(Les lignes d'abord, puis le tableau de pointeurs — l'inverse ferait
  perdre les adresses des lignes, qui resteraient allouées mais
  inaccessibles pour toujours.)*
