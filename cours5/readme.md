Cours 05
Programmation haut-niveau 

Implémentation d'une File Prioritaire avec Vecteur
Étude de cas : simulation d'ordonnanceur

Algorithme Round-Robin avec priorités
Système d'exploitation 

Notion de processus
Rôle et fonctionnement de l'ordonnanceur
Notion d'appel au système
(commun, ordonnanceur-v1 et ordonnanceur-v2)

=====================================================================

Méthode de déverminage
    Obtenir un mode de défaillance (symptôme).
    Émettre une hypothèse sur les causes du défaut.
    Concevoir un test minimal permettant de vérifier l'hypothèse.
    Exécuter le test et noter le résultat.
    Placer un breakpoint à l'endroit stratégique et comparer valeur attendue/obtenue.
    Corriger le défaut et exécuter à nouveau la suite de test pour trouver le prochain ;-)
    (ordonnanceur-v3)

===============================================================================

Importance de la suite de test

Il peut être très difficile d'émettre une hypothèse utile ou de concevoir un test si on semble être en présence de plusieurs défauts, surtout si la base de code est étendue.

La présence d'une suite de tests unitaires complète permet de réduire le domaine de recherche des erreurs.

Elle permet aussi de vérifier que la résolution d'un problème n'est pas la cause d'un autre (test de régression).

(ordonnanceur-v4)

==============================================================================
```mermaid
---
config:
  theme: neutral
---
classDiagram
    direction LR
    
    class processus {
        -char* nom
        -int duree
        -uint8_t priorite
        -bool est_termine
        +processus_creer()
        +processus_detruire()
        +processus_obtenir_nom()
        +processus_obtenir_duree()
        +processus_obtenir_priorite()
        +processus_est_termine()
        +processus_execute()
        +processus_afficher()
    }
    
    class vecteur {
        -size_t capacite
        -size_t taille
        -void** contenu
        +vecteur_creer()
        +vecteur_detruire()
        +vecteur_ajouter()
        +vecteur_obtenir()
        +vecteur_modifier()
        +vecteur_taille()
        +vecteur_capacite()
        +vecteur_retirer_dernier()
        +vecteur_inserer()
        +vecteur_supprimer()
        +vecteur_vider()
        +vecteur_est_vide()
        +vecteur_afficher()
    }
    
    class file {
        -vecteur_t* tableau
        +file_creer()
        +file_enfiler()
        +file_defiler()
        +file_regarder_tete()
        +file_liberer()
        +file_taille()
        +file_est_vide()
        +file_vider()
        +file_afficher()
    }
    
    class fileprio {
        -vecteur_t* listes
        +fileprio_creer()
        +fileprio_enfiler()
        +fileprio_defiler()
        +fileprio_regarder_tete()
        +fileprio_liberer()
        +fileprio_taille()
        +fileprio_niveaux()
        +fileprio_est_vide()
        +fileprio_afficher()
    }
    
    class ordonnanceur {
        -fileprio_t* taches
        +ordonnanceur_creer()
        +ordonnanceur_interprete()
        +ordonnanceur_ajouter()
        +ordonnanceur_executer()
    }
    
    file --> vecteur
    fileprio --> vecteur
    fileprio --> file
    ordonnanceur --> fileprio
    ordonnanceur --> processus
```

============================

Exercices cours 5

Problème 1

Ajoutez une suite de tests Unity pour chacuns des modules du scheduler_v3 ou v4, à votre choix.