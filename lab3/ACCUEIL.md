# ELE216 - Laboratoire 3 : Modules et types opaques

Documentation de l'interface (`.h`) et des tests unitaires (`*_test.c`) des
trois problèmes du laboratoire.

| Problème | Module | Interface | Tests |
|---|---|---|---|
| 1 | Pile rechargeable | pile.h | pile_test.c |
| 2 | Buffer circulaire (FIFO) | buffer.h | buffer_test.c |
| 3 | Filtre moyenne mobile | filtre.h | filtre_test.c |

## Comment lire les tests

Ouvrir l'onglet **Fichiers**, puis un fichier `*_test.c`. Chaque fonction
`test_...` est décrite par :

- **Scénario** : les opérations effectuées ;
- **Attendu** : le résultat vérifié ;
- **Détecte** : le défaut d'implémentation que ce test permet d'attraper ;
- **Trace** : où trouver une trace pas à pas (problèmes 2 et 3).

Le code de chaque test est affiché sous sa description.
