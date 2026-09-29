# Mini-app du laboratoire 4 : moniteur de batterie

Le moniteur est un module du laboratoire 4 construit à partir de trois
modules du laboratoire 3. Il ne connaît d'eux que leur `.h` : il ne touche
jamais à leurs champs.

## 1. Qui contient quoi

```mermaid
---
config:
  themeVariables:
    fontSize: 22px
---
flowchart LR
    MAIN["<b>moniteur_test.c</b><br/>simulation + tests"]

    subgraph LAB4["Laboratoire 4"]
        MON["<b>moniteur_t</b><br/>seuil_alerte (float)"]
    end

    subgraph LAB3["Laboratoire 3 (réutilisé, non copié)"]
        direction TB
        PILE["<b>pile_t</b><br/>la batterie surveillée"]
        BUF["<b>buffer_t</b><br/>historique des %<br/>(entiers)"]
        FIL["<b>filtre_t</b><br/>moyenne mobile des %"]
    end

    MAIN -- "moniteur.h" --> MON
    MON -- "pile.h" --> PILE
    MON -- "buffer.h" --> BUF
    MON -- "filtre.h" --> FIL
```

Chaque flèche signifie « utilise uniquement les fonctions déclarées dans ce
`.h` ». `moniteur_creer` crée les trois sous-modules et `moniteur_detruire`
les libère.

## 2. Toutes les fonctions des laboratoires 3 et 4

```mermaid
---
config:
  themeVariables:
    fontSize: 18px
---
classDiagram
    direction TB

    namespace Laboratoire4 {
        class compteur_t {
            -int valeur
            -int seuil
            -int depassements
            +compteur_t *compteur_creer(int seuil)
            +void compteur_detruire(compteur_t *c)
            +void compteur_incrementer(compteur_t *c)
            +void compteur_reinitialiser(compteur_t *c)
            +int compteur_valeur(const compteur_t *c)
            +int compteur_seuil(const compteur_t *c)
            +int compteur_depassements(const compteur_t *c)
            +void compteur_afficher(const compteur_t *c)
        }
        class servo_t {
            -float angle
            -float angle_min
            -float angle_max
            -float vitesse
            +servo_t *servo_creer(float angle_min, float angle_max, float vitesse)
            +void servo_detruire(servo_t *s)
            +void servo_aller_a(servo_t *s, float angle)
            +float servo_temps_pour_atteindre(const servo_t *s, float angle)
            +float servo_angle(const servo_t *s)
            +float servo_angle_min(const servo_t *s)
            +float servo_angle_max(const servo_t *s)
            +float servo_vitesse(const servo_t *s)
            +void servo_afficher(const servo_t *s)
        }
        class moniteur_t {
            -pile_t *pile
            -buffer_t *historique
            -filtre_t *filtre
            -float seuil_alerte
            +moniteur_t *moniteur_creer(float capacite, float tension, int taille_historique, int taille_filtre, float seuil_alerte)
            +void moniteur_detruire(moniteur_t *m)
            +void moniteur_charger(moniteur_t *m, float mAh)
            +void moniteur_decharger(moniteur_t *m, float mAh)
            +void moniteur_enregistrer(moniteur_t *m)
            +float moniteur_charge_brute(const moniteur_t *m)
            +float moniteur_charge_lissee(const moniteur_t *m)
            +int moniteur_alerte(const moniteur_t *m)
            +float moniteur_seuil_alerte(const moniteur_t *m)
            +int moniteur_nb_lectures(const moniteur_t *m)
            +void moniteur_afficher(const moniteur_t *m)
        }
    }

    namespace Laboratoire3 {
        class pile_t {
            -float capacite_max
            -float charge
            -float tension
            +pile_t *pile_creer(float capacite_mAh, float tension_V)
            +void pile_detruire(pile_t *p)
            +void pile_charger(pile_t *p, float mAh)
            +void pile_decharger(pile_t *p, float mAh)
            +float pile_capacite_max(const pile_t *p)
            +float pile_charge_courante(const pile_t *p)
            +float pile_tension(const pile_t *p)
            +float pile_pourcentage(const pile_t *p)
            +void pile_afficher(const pile_t *p)
        }
        class buffer_t {
            -int capacite
            -int *donnees
            -int tete
            -int queue
            -int nb_elements
            +buffer_t *buffer_creer(int capacite)
            +void buffer_detruire(buffer_t *b)
            +int buffer_enfiler(buffer_t *b, int valeur)
            +int buffer_defiler(buffer_t *b, int *valeur)
            +int buffer_capacite(const buffer_t *b)
            +int buffer_taille(const buffer_t *b)
            +int buffer_est_vide(const buffer_t *b)
            +int buffer_est_plein(const buffer_t *b)
            +void buffer_afficher(const buffer_t *b)
        }
        class filtre_t {
            -int taille_fenetre
            -float *echantillons
            -int nb_echantillons
            -int index_ecriture
            +filtre_t *filtre_creer(int taille_fenetre)
            +void filtre_detruire(filtre_t *f)
            +void filtre_ajouter(filtre_t *f, float echantillon)
            +float filtre_valeur(const filtre_t *f)
            +int filtre_taille_fenetre(const filtre_t *f)
            +int filtre_nb_echantillons(const filtre_t *f)
            +void filtre_afficher(const filtre_t *f)
        }
    }

    moniteur_t *-- "1" pile_t : pile
    moniteur_t *-- "1" buffer_t : historique
    moniteur_t *-- "1" filtre_t : filtre
```

Les champs (`-`) sont privés : ils sont définis dans le `.c` et invisibles
pour les autres modules. Les fonctions (`+`) sont publiques : ce sont
exactement celles déclarées dans le `.h`. Le losange plein indique que
`moniteur_t` possède ses trois sous-modules (il les crée et les détruit).
`compteur_t` (problème 1) et `servo_t` (problème 2) sont indépendants.

## 3. Ce qui se passe lors d'une lecture

```mermaid
---
config:
  sequence:
    actorFontSize: 20
    messageFontSize: 20
    noteFontSize: 18
    messageMargin: 40
---
sequenceDiagram
    participant U as main
    participant M as moniteur_t
    participant P as pile_t
    participant B as buffer_t
    participant F as filtre_t

    U->>M: moniteur_enregistrer(m)
    M->>P: pile_pourcentage()
    P-->>M: pourcentage
    opt historique plein
        M->>B: buffer_defiler()
        Note right of B: retire la plus<br/>ancienne lecture
    end
    M->>B: buffer_enfiler(arrondi du %)
    M->>F: filtre_ajouter(%)

    U->>M: moniteur_alerte(m)
    M->>F: filtre_valeur()
    F-->>M: charge lissée
    M-->>U: lissée < seuil_alerte ?
```

`moniteur_charger` et `moniteur_decharger` ne font que relayer vers
`pile_charger` / `pile_decharger` : l'historique et le filtre ne changent
qu'au prochain `moniteur_enregistrer`.
