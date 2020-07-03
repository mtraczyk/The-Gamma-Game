/** @file
 * Implementacja modułu funkcji pomocniczych potrzebnych do
 * przygotowania bufora z reprezentacją planszy.
 */

#include "gamma_structures.h"
#include "gamma.h"
#include "aux_for_gamma_board.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define multiplier 3 ///< mnożnik
#define divider 2    ///< dzielnik

bool are_arguments_correct(gamma_t *g, uint32_t player) {
    if (g == NULL || player == 0 || player > (g->players)) {
        return false;
    }

    return true;
}

/** @brief Aktualizuje rozmiar bufora.
 * @param [in,out] board - bufor,
 * @param [in] counter - licznik, liczba, która mówi jak dużo znaków
 * zostało zapisanych do bufora,
 * @param [in,out] size - aktualny rozmiar bufora.
 * @return Fałsz jeśli nie udało się powiększyć rozmiaru bufora.
 * Prawda w przeciwnym wypadku.
 */
bool update_size_of_string(char **board, uint64_t *counter, uint64_t *size) {
    if (*counter == *size) {
        *size = (multiplier * (*size)) / divider + 1;
        (*board) = realloc((*board), (*size) * sizeof(char));
        if ((*board) == NULL) {
            return false;
        }

        for (uint64_t i = *counter; i < *size; i++)
            (*board)[i] = 0;
    }

    return true;
}

/** @brief Dodaje znak do bufora.
 * @param [in] c - znak,
 * @param [in,out] board - bufor,
 * @param [in,out] counter - licznik, liczba  która mówi jak dużo znaków
 * zostało zapisanych do bufora,
 * @param [in,out] size - aktualny rozmiar bufora.
 * @return Fałsz jeśli nie udało się dodać znaku.
 * Prawda w przeciwnym wypadku.
 */
bool add_character_to_string(char c, char **board, uint64_t *counter,
                             uint64_t *size) {
    if (!update_size_of_string(board, counter, size)) {
        return false;
    }

    (*board)[*counter] = c;
    *counter = *counter + 1;

    return true;
}

/** @brief Dodaje numer gracza do bufora.
 * @param [in] players - liczba graczy,
 * @param [in] player - numer gracza, którego identyfikator chcę
 * dopisać do bufora,
 * @param [in,out] board - bufor,
 * @param [in,out] counter - licznik, liczba  która mówi jak dużo znaków
 * zostało zapisanych do bufora,
 * @param [in,out] size - aktualny rozmiar bufora.
 * @return Fałsz jeśli nie udało się dodać napisu.
 * Prawda w przeciwnym wypadku.
 */
bool add_id_to_string(uint32_t players, uint32_t player, char **board, uint64_t *counter, uint64_t *size) {
    uint32_t length = snprintf(NULL, 0, "%" PRIu32, players);
    char *id_of_player = malloc((length + 1) * sizeof(char));
    if (id_of_player == NULL) {
        return false;
    }
    snprintf(id_of_player, length + 1, "%*" PRIu32 "\n", length, player);

    if (player != 0) {
        for (uint32_t i = 0; i < length; i++) {
            if (!add_character_to_string(id_of_player[i], board, counter, size)) {
                free(id_of_player);
                return false;
            }
        }
    } else {
        for (uint32_t i = 0; i < length; i++) {
            if (!add_character_to_string(' ', board, counter, size)) {
                free(id_of_player);
                return false;
            }
        }
    }

    free(id_of_player);
    return true;
}

char *gamma_board_with_many_players(gamma_t *g) {
    char *board = NULL;
    uint32_t w = g->width;
    uint32_t h = g->height;
    uint64_t counter = 0;
    uint64_t size = 1;

    board = malloc(sizeof(char));
    if (board == NULL) {
        return NULL;
    }
    board[0] = 0;

    while (h != 0) {
        for (uint32_t j = 0; j < w; j++) {
            if ((g->board)[j][h - 1] != 0) {
                if (!add_id_to_string(g->players, (g->board)[j][h - 1], &board, &counter, &size)) {
                    return NULL;
                }
                if (!add_character_to_string(' ', &board, &counter, &size)) {
                    return NULL;
                }
            } else {
                if (!add_character_to_string('.', &board, &counter, &size)) {
                    return NULL;
                }

                if (!add_id_to_string(g->players, 0, &board, &counter, &size)) {
                    return NULL;
                }
            }
        }
        if (!add_character_to_string('\n', &board, &counter, &size)) {
            return NULL;
        }
        h--;
    }

    return board;
}

char *gamma_board_with_less_than_10_players(gamma_t *g) {
    uint32_t w = g->width;
    uint32_t h = g->height;
    uint64_t counter = 0;
    char *board = NULL;

    board = malloc((h * (w + 1) + 1) * sizeof(char));
    if (board == NULL) {
        return NULL;
    }

    for (uint64_t i = 0; i < h * (w + 1) + 1; i++)
        board[i] = 0;

    while (h != 0) {
        for (uint32_t j = 0; j < w; j++) {
            if ((g->board)[j][h - 1] != 0) {
                board[counter] = '0' + (g->board)[j][h - 1];
            } else {
                board[counter] = '.';
            }
            counter++;
        }

        board[counter] = '\n';
        counter++;
        h--;
    }

    return board;
}
