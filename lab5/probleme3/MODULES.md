# Problème 3 du laboratoire 5 : séquenceur de commandes

Le séquenceur est construit à partir de la File et de la Pile génériques du
cours 4 (elles-mêmes bâties sur le Vecteur générique) et du journal du
problème 2. Il ne connaît de ces modules que leur `.h` : il ne touche jamais
à leurs champs.

## 1. Diagramme de classes

```mermaid
---
config:
  themeVariables:
    fontSize: 18px
---
classDiagram
    direction TB

    namespace Probleme3 {
        class sequenceur_t {
            -file_gen_t *commandes
            -pile_gen_t *contextes
            -config_t courant
            +sequenceur_t *sequenceur_creer(void)
            +void sequenceur_detruire(sequenceur_t *s)
            +void sequenceur_enfiler(sequenceur_t *s, commande_t cmd)
            +int sequenceur_nb_en_attente(const sequenceur_t *s)
            +int sequenceur_executer_prochaine(sequenceur_t *s, journal_t *j)
            +void sequenceur_executer_tout(sequenceur_t *s, journal_t *j)
            +const config_t *sequenceur_contexte(const sequenceur_t *s)
            +void sequenceur_afficher_file(const sequenceur_t *s)
            -void executer(sequenceur_t *s, const commande_t *c, journal_t *j)
            -void empiler_contexte(sequenceur_t *s)
            -void depiler_contexte(sequenceur_t *s)
            -void mesurer(const sequenceur_t *s, const commande_t *c, journal_t *j)
            -const char *nom_commande(type_commande_t type)
        }
        class config_t {
            +float tension
            +float courant
            +float tolerance
        }
        class commande_t {
            +type_commande_t type
            +char id[COMMANDE_ID_MAX]
            +float valeur
        }
        class type_commande_t {
            <<enumeration>>
            CMD_SET_TENSION
            CMD_SET_COURANT
            CMD_SET_TOLERANCE
            CMD_MESURER
            CMD_PUSH
            CMD_POP
            CMD_RAPPORT
        }
    }

    namespace Probleme2 {
        class journal_t {
            -vecteur_t *resultats
            +journal_t *journal_creer(void)
            +void journal_detruire(journal_t *j)
            +void journal_ajouter(journal_t *j, const char *id, float attendu, float mesure, float tolerance)
            +int journal_nb_resultats(const journal_t *j)
            +int journal_nb_reussis(const journal_t *j)
            +int journal_nb_echoues(const journal_t *j)
            +const resultat_t *journal_obtenir(const journal_t *j, int index)
            +void journal_afficher(const journal_t *j)
            +void journal_sauvegarder(const journal_t *j, const char *chemin)
            -int compter_verdicts(const journal_t *j, int verdict)
            -void ecrire_rapport(const journal_t *j, FILE *flux)
        }
    }

    namespace Probleme1 {
        class resultat_t {
            -char *id
            -float attendu
            -float mesure
            -float tolerance
            -int reussi
            +resultat_t *resultat_creer(const char *id, float attendu, float mesure, float tolerance)
            +void resultat_detruire(resultat_t *r)
            +const char *resultat_id(const resultat_t *r)
            +float resultat_attendu(const resultat_t *r)
            +float resultat_mesure(const resultat_t *r)
            +float resultat_tolerance(const resultat_t *r)
            +int resultat_est_reussi(const resultat_t *r)
            +void resultat_afficher(const resultat_t *r)
            -int calculer_verdict(float attendu, float mesure, float tolerance)
        }
    }

    namespace Commun_cours4 {
        class file_gen_t {
            -vecteur_t *vect
            +file_gen_t *file_gen_creer()
            +void file_gen_liberer(file_gen_t *f)
            +void file_gen_enfiler(file_gen_t *f, void *element)
            +void *file_gen_defiler(file_gen_t *f)
            +void *file_gen_regarder(file_gen_t *f)
            +bool file_gen_est_vide(file_gen_t *f)
            +int file_gen_taille(file_gen_t *f)
        }
        class pile_gen_t {
            -vecteur_t *vect
            +pile_gen_t *pile_gen_creer()
            +void pile_gen_liberer(pile_gen_t *p)
            +void pile_gen_empiler(pile_gen_t *p, void *element)
            +void *pile_gen_depiler(pile_gen_t *p)
            +void *pile_gen_regarder(pile_gen_t *p)
            +bool pile_gen_est_vide(pile_gen_t *p)
            +int pile_gen_taille(pile_gen_t *p)
        }
        class vecteur_t {
            -int capacite
            -int taille
            -void **contenu
            +vecteur_t *vecteur_creer(int taille_initiale)
            +int vecteur_ajouter(vecteur_t *vect, void *valeur)
            +void *vecteur_obtenir(vecteur_t *vect, int position)
            +void *vecteur_retirer_dernier(vecteur_t *vect)
            +void *vecteur_retirer_premier(vecteur_t *vect)
            +void vecteur_liberer(vecteur_t *vect)
            +void vecteur_afficher(vecteur_t *vect)
            +int vecteur_taille(vecteur_t *vect)
            +bool vecteur_est_vide(vecteur_t *vect)
            +int vecteur_capacite(vecteur_t *vect)
            -bool agrandir(vecteur_t *vect)
        }
    }

    sequenceur_t *-- "1" file_gen_t : commandes
    sequenceur_t *-- "1" pile_gen_t : contextes
    sequenceur_t *-- "1" config_t : courant (par valeur)
    sequenceur_t *-- "0..*" commande_t : en attente (via la File)
    sequenceur_t *-- "0..*" config_t : sauvegardés (via la Pile)
    sequenceur_t ..> journal_t : reçu en paramètre (non possédé)
    commande_t --> type_commande_t : type

    file_gen_t *-- "1" vecteur_t : vect
    pile_gen_t *-- "1" vecteur_t : vect
    file_gen_t o-- "0..*" commande_t : void* stockés
    pile_gen_t o-- "0..*" config_t : void* stockés

    journal_t *-- "1" vecteur_t : resultats
    journal_t *-- "0..*" resultat_t : possède (via le Vecteur)
```

Lecture du diagramme :

- **Visibilité.** Les champs `-` sont privés : ils sont définis dans le `.c`
  (type opaque) et invisibles pour les autres modules. Les fonctions `-` sont
  les fonctions `static` du `.c`. Les fonctions `+` sont exactement celles du
  `.h`. `config_t` et `commande_t` ne sont **pas** opaques : leurs champs sont
  publics (`+`), car l'appelant doit pouvoir construire une commande et lire
  le contexte.
- **Losange plein (composition).** `sequenceur_t` crée et détruit sa File, sa
  Pile, son contexte courant, ainsi que toutes les copies `commande_t` et
  `config_t` qu'il alloue. Le journal fait de même pour ses `resultat_t`.
- **Losange vide (agrégation).** La File et la Pile ne font que **stocker des
  `void *`**. Elles ne libèrent jamais leurs éléments (`vecteur_liberer` ne
  libère que le tableau de pointeurs). C'est pourquoi `sequenceur_detruire`
  doit les vider lui-même.
- **Pointillé (dépendance).** Le séquenceur utilise un `journal_t` reçu en
  paramètre d'exécution, mais il n'en est pas propriétaire : c'est
  l'appelant qui le crée et le détruit.
- Il y a **deux** `config_t` distincts : `courant`, stocké par valeur dans la
  structure, et les copies sauvegardées par `CMD_PUSH` dans la Pile. La Pile
  est vide à la création.

## 2. Diagramme de séquence : cycle de vie complet

Le scénario suit la démonstration du `main` de `sequenceur_test.c` :
création, enfilage, affichage de la file, exécution de toutes les commandes
(chaque type de commande est détaillé dans un bloc `alt`), lecture du
contexte, puis destruction. Le Vecteur interne de la File, de la Pile et du
journal n'est pas montré.

```mermaid
---
config:
  sequence:
    actorFontSize: 18
    messageFontSize: 16
    noteFontSize: 15
    messageMargin: 30
---
sequenceDiagram
    participant T as main (sequenceur_test.c)
    participant S as sequenceur_t
    participant F as file_gen_t
    participant P as pile_gen_t
    participant J as journal_t
    participant R as resultat_t
    participant C as stdlib (malloc, free, rand)

%% ---------- Création ----------
    T->>S: sequenceur_creer()
    S->>C: malloc(sizeof(sequenceur_t))
    S->>F: file_gen_creer()
    F-->>S: commandes
    S->>P: pile_gen_creer()
    P-->>S: contextes (pile vide)
    alt File ou Pile NULL
        S->>F: file_gen_liberer() si créée
        S->>P: pile_gen_liberer() si créée
        S->>C: free(s)
        S-->>T: NULL
    else succès
        Note over S: courant = { 0 V, 0 A, 5 % }
        S-->>T: s
    end
    T->>J: journal_creer()
    J-->>T: j
    T->>C: srand(SEED)

%% ---------- Enfilage ----------
    loop pour chaque commande du scénario
        T->>S: sequenceur_enfiler(s, cmd) (copie par valeur)
        S->>C: malloc(sizeof(commande_t))
        C-->>S: copie
        Note over S: *copie = cmd<br/>id[COMMANDE_ID_MAX - 1] = terminateur nul
        S->>F: file_gen_taille()
        F-->>S: avant
        S->>F: file_gen_enfiler(copie)
        S->>F: file_gen_taille()
        opt taille inchangée (realloc échoué)
            S->>C: free(copie)
        end
    end

%% ---------- Affichage ----------
    T->>S: sequenceur_afficher_file(s)
    S->>F: file_gen_taille()
    F-->>S: n
    loop n fois (rotation complète)
        S->>F: file_gen_defiler()
        F-->>S: c
        Note over S: printf(nom_commande(c->type), id, valeur)
        S->>F: file_gen_enfiler(c)
    end
    Note over F: même contenu, même ordre

%% ---------- Exécution ----------
    T->>S: sequenceur_executer_tout(s, j)
    loop tant que sequenceur_executer_prochaine(s, j) retourne 1
        S->>F: file_gen_est_vide()
        alt file vide
            F-->>S: true
            Note over S: retourne 0 (fin de la boucle)
        else file non vide
            F-->>S: false
            S->>F: file_gen_defiler()
            F-->>S: c
            S->>S: executer(s, c, j)
            alt CMD_SET_TENSION, CMD_SET_COURANT, CMD_SET_TOLERANCE
                Note over S: courant.champ = c->valeur
            else CMD_PUSH
                S->>C: malloc(sizeof(config_t))
                C-->>S: copie
                Note over S: *copie = courant
                S->>P: pile_gen_taille()
                S->>P: pile_gen_empiler(copie)
                S->>P: pile_gen_taille()
                opt taille inchangée (realloc échoué)
                    S->>C: free(copie)
                end
            else CMD_POP
                S->>P: pile_gen_est_vide()
                alt pile vide
                    P-->>S: true
                    Note over S: stderr : CMD_POP ignorée<br/>courant inchangé
                else pile non vide
                    P-->>S: false
                    S->>P: pile_gen_depiler()
                    P-->>S: sauvegarde
                    Note over S: courant = *sauvegarde
                    S->>C: free(sauvegarde)
                end
            else CMD_MESURER
                alt j == NULL
                    Note over S: stderr : CMD_MESURER ignorée
                else journal présent
                    S->>C: rand()
                    C-->>S: r
                    Note over S: bruit = (2 r / RAND_MAX - 1) x 0.10<br/>mesure = valeur x (1 + bruit)
                    S->>J: journal_ajouter(j, c->id, c->valeur, mesure, courant.tolerance)
                    J->>R: resultat_creer(id, attendu, mesure, tolerance)
                    Note over R: copie de id<br/>verdict PASS ou FAIL calculé
                    R-->>J: r
                    Note over J: vecteur_ajouter(resultats, r)
                end
            else CMD_RAPPORT
                alt j == NULL
                    Note over S: stderr : CMD_RAPPORT ignorée
                else journal présent
                    S->>J: journal_afficher(j)
                    loop chaque résultat
                        J->>R: resultat_id(), attendu, mesure, tolerance, est_reussi
                        R-->>J: valeurs
                    end
                    Note over J: tableau + Total, Reussis, Echoues
                end
            else type inconnu
                Note over S: stderr : commande ignorée
            end
            S->>C: free(c)
            Note over S: retourne 1
        end
    end
    S-->>T: file vide

%% ---------- Lecture du contexte ----------
    T->>S: sequenceur_contexte(s)
    S-->>T: const config_t * vers courant

%% ---------- Destruction ----------
    T->>S: sequenceur_detruire(s)
    loop tant que la File n'est pas vide
        S->>F: file_gen_defiler()
        F-->>S: commande restante
        S->>C: free(commande)
    end
    loop tant que la Pile n'est pas vide
        S->>P: pile_gen_depiler()
        P-->>S: contexte sauvegardé
        S->>C: free(contexte)
    end
    S->>F: file_gen_liberer()
    S->>P: pile_gen_liberer()
    S->>C: free(s)
    T->>J: journal_detruire(j)
    loop chaque résultat
        J->>R: resultat_detruire(r)
    end
```

Points à remarquer :

- **Copies sur le tas.** `sequenceur_enfiler` et `CMD_PUSH` font chacun un
  `malloc` : la File et la Pile ne stockent que des adresses. Stocker
  l'adresse du paramètre `cmd` ou de `courant` serait une erreur.
- **Garde avant chaque retrait.** `file_gen_est_vide` et `pile_gen_est_vide`
  sont toujours appelées avant `defiler` et `depiler`, car le code du cours
  n'a pas d'assert (la taille deviendrait -1).
- **Un seul `free` par élément.** Chaque commande est libérée soit après son
  exécution, soit dans `sequenceur_detruire` si elle est encore en attente.
  Chaque contexte sauvegardé est libéré soit par `CMD_POP`, soit dans
  `sequenceur_detruire`.
- **Rotation en lecture seule.** `sequenceur_afficher_file` modifie
  temporairement la File, mais après `n` rotations elle est identique. Le
  `const` du paramètre ne protège que le pointeur `s->commandes`, pas la
  File pointée.
- **Journal externe.** Le séquenceur appelle le journal mais ne le crée ni
  ne le détruit.
