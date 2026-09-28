# ELE216 - Laboratoire 4 : Types opaques (suite)

Documentation de l'interface (`.h`) et des tests unitaires (`*_test.c`) des
deux problèmes et de la mini-application du laboratoire.

| Partie | Module | Interface | Tests |
|---|---|---|---|
| Problème 1 | Compteur d'événements | compteur.h | compteur_test.c |
| Problème 2 | Servo-moteur | servo.h | servo_test.c |
| Mini-app | Moniteur de batterie | moniteur.h | moniteur_test.c |

Le moniteur de batterie réutilise les modules pile, buffer circulaire et
filtre du laboratoire 3 (voir la documentation du lab3).

## Comment lire les tests

Ouvrir l'onglet **Fichiers**, puis un fichier `*_test.c`. Chaque fonction
`test_...` est décrite par :

- **Scénario** : les opérations effectuées ;
- **Attendu** : le résultat vérifié ;
- **Détecte** : le défaut d'implémentation que ce test permet d'attraper ;
- **Trace** : le calcul pas à pas, quand il n'est pas évident.

Le code de chaque test est affiché sous sa description.
