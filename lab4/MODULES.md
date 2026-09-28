# Modules des laboratoires 3 et 4

```mermaid
---
title: ELE216 - Modules des laboratoires 3 et 4
---
classDiagram
    direction TB

    namespace Laboratoire_3 {
        class pile_t {
            <<type opaque>>
            -float capacite_max
            -float charge
            -float tension
            +pile_creer(float capacite_mAh, float tension_V)
            +pile_detruire()
            +pile_charger(float mAh)
            +pile_decharger(float mAh)
            +pile_capacite_max() float
            +pile_charge_courante() float
            +pile_tension() float
            +pile_pourcentage() float
            +pile_afficher()
        }
        class buffer_t {
            <<type opaque>>
            -int capacite
            -int[] donnees
            -int tete
            -int queue
            -int nb_elements
            +buffer_creer(int capacite)
            +buffer_detruire()
            +buffer_enfiler(int valeur) int
            +buffer_defiler(int* valeur) int
            +buffer_capacite() int
            +buffer_taille() int
            +buffer_est_vide() int
            +buffer_est_plein() int
            +buffer_afficher()
        }
        class filtre_t {
            <<type opaque>>
            -int taille_fenetre
            -float[] echantillons
            -int nb_echantillons
            -int index_ecriture
            +filtre_creer(int taille_fenetre)
            +filtre_detruire()
            +filtre_ajouter(float echantillon)
            +filtre_valeur() float
            +filtre_taille_fenetre() int
            +filtre_nb_echantillons() int
            +filtre_afficher()
        }
    }

    namespace Laboratoire_4 {
        class compteur_t {
            <<type opaque>>
            -int valeur
            -int seuil
            -int depassements
            +compteur_creer(int seuil)
            +compteur_detruire()
            +compteur_incrementer()
            +compteur_reinitialiser()
            +compteur_valeur() int
            +compteur_seuil() int
            +compteur_depassements() int
            +compteur_afficher()
        }
        class servo_t {
            <<type opaque>>
            -float angle
            -float angle_min
            -float angle_max
            -float vitesse
            +servo_creer(float angle_min, float angle_max, float vitesse)
            +servo_detruire()
            +servo_aller_a(float angle)
            +servo_temps_pour_atteindre(float angle) float
            +servo_angle() float
            +servo_angle_min() float
            +servo_angle_max() float
            +servo_vitesse() float
            +servo_afficher()
        }
        class moniteur_t {
            <<type opaque>>
            -pile_t* pile
            -buffer_t* historique
            -filtre_t* filtre
            -float seuil_alerte
            +moniteur_creer(float capacite, float tension, int taille_historique, int taille_filtre, float seuil_alerte)
            +moniteur_detruire()
            +moniteur_charger(float mAh)
            +moniteur_decharger(float mAh)
            +moniteur_enregistrer()
            +moniteur_charge_brute() float
            +moniteur_charge_lissee() float
            +moniteur_alerte() int
            +moniteur_seuil_alerte() float
            +moniteur_nb_lectures() int
            +moniteur_afficher()
        }
    }

    moniteur_t *-- "1" pile_t : pile surveillée
    moniteur_t *-- "1" buffer_t : historique des %
    moniteur_t *-- "1" filtre_t : lissage des %

    note for moniteur_t "Mini-app du lab 4 : réutilise les modules\ndu lab 3 sans les copier (Makefile :\n-I ../../lab3/probleme1, 2 et 3)"
```

## Légende

- `+` : fonction publique, déclarée dans le `.h` ; c'est tout ce que l'utilisateur
  du module voit.
- `-` : champ privé de la `struct`, caché dans le `.c` (type opaque) ; on n'y
  accède que par les fonctions publiques.
- Toutes les fonctions, sauf `*_creer`, reçoivent le pointeur vers le type
  opaque en premier paramètre (omis sur la figure pour l'alléger).
- `*_creer` retourne un pointeur vers une nouvelle instance allouée sur le heap,
  ou `NULL` si un paramètre est invalide ; `*_detruire` la libère
  (`*_detruire(NULL)` ne fait rien).
- Losange plein (composition) : le moniteur crée sa pile, son buffer et son
  filtre dans `moniteur_creer` et les détruit dans `moniteur_detruire`.
