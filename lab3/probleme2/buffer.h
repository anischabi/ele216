#ifndef BUFFER_H
#define BUFFER_H

/**
 * @file buffer.h
 * @brief Module de file FIFO d'entiers à capacité fixe (buffer circulaire).
 *
 * La capacité est définie à la création et ne peut plus changer. Les
 * éléments sont défilés dans l'ordre où ils ont été enfilés (premier entré,
 * premier sorti).
 */

/** Type opaque : les champs sont cachés dans buffer.c. */
typedef struct buffer buffer_t;

/**
 * Crée un buffer circulaire vide. Alloue la structure ET son tableau interne.
 *
 * @param capacite nombre maximal d'éléments. Doit être strictement positif.
 * @return un pointeur vers le buffer créé. NULL si capacite <= 0 ou en cas
 * d'erreur d'allocation mémoire.
 */
buffer_t *buffer_creer(int capacite);

/**
 * Libère la mémoire du buffer ET de son tableau interne.
 *
 * @param b le buffer à détruire. Si le pointeur est NULL, la fonction n'a
 * pas d'effet (voir la définition de free()). Après l'appel, le pointeur de
 * l'appelant n'est plus valide.
 */
void buffer_detruire(buffer_t *b);

/**
 * Ajoute une valeur à la queue du buffer. Si le buffer est plein, il n'est
 * pas modifié.
 *
 * @param b      pointeur non-nul vers le buffer.
 * @param valeur la valeur à enfiler.
 * @return 1 si la valeur a été ajoutée, 0 si le buffer est plein.
 */
int buffer_enfiler(buffer_t *b, int valeur);

/**
 * Retire l'élément le plus ancien du buffer et le place dans *valeur. Si le
 * buffer est vide, ni le buffer ni *valeur ne sont modifiés.
 *
 * @param b      pointeur non-nul vers le buffer.
 * @param valeur pointeur non-nul vers l'emplacement qui reçoit la valeur retirée.
 * @return 1 si une valeur a été retirée, 0 si le buffer est vide.
 */
int buffer_defiler(buffer_t *b, int *valeur);

/**
 * @param b pointeur non-nul vers le buffer.
 * @return la capacité du buffer (nombre maximal d'éléments).
 */
int buffer_capacite(const buffer_t *b);

/**
 * @param b pointeur non-nul vers le buffer.
 * @return le nombre d'éléments actuellement présents.
 */
int buffer_taille(const buffer_t *b);

/**
 * @param b pointeur non-nul vers le buffer.
 * @return 1 si le buffer ne contient aucun élément, 0 sinon.
 */
int buffer_est_vide(const buffer_t *b);

/**
 * @param b pointeur non-nul vers le buffer.
 * @return 1 si le buffer contient autant d'éléments que sa capacité, 0 sinon.
 */
int buffer_est_plein(const buffer_t *b);

/**
 * Affiche le contenu du buffer sur la sortie standard, dans l'ordre FIFO
 * (du plus ancien au plus récent), dans un format utile au débogage.
 *
 * @param b pointeur non-nul vers le buffer.
 */
void buffer_afficher(const buffer_t *b);

#endif // BUFFER_H
