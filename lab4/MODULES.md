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

## 2. Ce qui se passe lors d'une lecture

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
