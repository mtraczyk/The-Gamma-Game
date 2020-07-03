/** @file
 * Implementacja modułu funkcji pomocniczych potrzebnych do
 * wykonania ruchu/złotego ruchu.
 */

#include "update_structures_after_move.h"
#include "gamma.h"
#include "gamma_structures.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/** @brief Sprawdza czy pole ma dokładnie jednego sąsiada należącego
 * do gracza.
 * @param [in] g - struktura reprezentująca grę,
 * @param [in] player - numer gracza,
 * @param [in] x - pierwsza współrzędna pola,
 * @param [in] y - druga współrzędna pola.
 * @return Prawda, jeśli jest dokładnie jedno takie pole.
 * Fałsz w przeciwnym przypadku.
 */
bool check_whether_field_has_only_one_neighbour_that_belongs_to_player(gamma_t *g, uint32_t player,
                                                                       uint32_t x, uint32_t y) {
    if ((g->board)[x][y] != 0) {
        return false;
    }

    bool a = x != 0 ? 0 : 1;
    bool b = y != 0 ? 0 : 1;
    uint32_t w = g->width;
    uint32_t h = g->height;
    uint32_t pom = 0;

    for (int32_t i = a - 1; i <= 1; i++) {
        for (int32_t j = b - 1; j <= 1; j++) {
            if ((i != 0 || j != 0) && (i == 0 || j == 0) && x + i < w && y + j < h &&
                (g->board)[x + i][y + j] == player) {
                pom++;
            }
        }
    }

    if (pom == 1) {
        return true;
    }

    return false;
}

/** @brief Procedura pomocnicza dla
 * @ref update_number_of_free_fields_after_golden.
 * @param [in,out] g - struktura reprezentująca grę,
 * @param [in] player - numer gracza,
 * @param [in] x - pierwsza współrzędna pola,
 * @param [in] y - druga współrzędna pola,
 * @param [in] add - ma wartość 1 albo -1, specyfikuje to implementacja
 * @ref update_number_of_free_fields_after_golden.
 */
void update_number_of_free_fields_after_golden_aux(gamma_t *g, uint32_t player,
                                                   uint32_t x, uint32_t y,
                                                   int32_t add) {
    bool a = x != 0 ? 0 : 1;
    bool b = y != 0 ? 0 : 1;
    uint32_t w = g->width;
    uint32_t h = g->height;

    for (int32_t i = a - 1; i <= 1; i++) {
        for (int32_t j = b - 1; j <= 1; j++) {
            if ((i != 0 || j != 0) && (i == 0 || j == 0) && x + i < w && y + j < h) {
                if (check_whether_field_has_only_one_neighbour_that_belongs_to_player(g, player, x + i, y + j)) {
                    (g->free_neighbouring_fields)[player - 1] += add;
                }
            }
        }
    }
}

void update_number_of_free_fields_after_golden(gamma_t *g, uint32_t player, uint32_t x, uint32_t y) {
    uint32_t player2 = (g->board)[x][y];

    update_number_of_free_fields_after_golden_aux(g, player2, x, y, -1);

    (g->board)[x][y] = player;

    update_number_of_free_fields_after_golden_aux(g, player, x, y, 1);
}

/** @brief Procedura pomocnicza.
 * Przywraca tablicę pomocniczą gamma_t.is_the_state_accurate
 * do poprzedniego stanu.
 * @param [in,out] g - struktura reprezentująca grę,
 * @param [in] x - pierwsza współrzędna pola,
 * @param [in] y - druga współrzędna pola.
 */
void rollback_is_the_state_accurate(gamma_t *g, uint32_t x, uint32_t y) {
    bool a = x != 0 ? 0 : 1;
    bool b = y != 0 ? 0 : 1;
    uint32_t w = g->width;
    uint32_t h = g->height;

    for (int32_t i = a - 1; i <= 1; i++) {
        for (int32_t j = b - 1; j <= 1; j++) {
            if ((i != 0 || j != 0) && (i == 0 || j == 0) && x + i < w && y + j < h) {
                uint32_t p = (g->board)[x + i][y + j];
                if (p != 0) {
                    (g->is_the_state_accurate)[p - 1] = false;
                }
            }
        }
    }
}

void update_number_of_free_fields_after_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y) {
    bool a = x != 0 ? 0 : 1;
    bool b = y != 0 ? 0 : 1;
    uint32_t w = g->width;
    uint32_t h = g->height;

    for (int32_t i = a - 1; i <= 1; i++) {
        for (int32_t j = b - 1; j <= 1; j++) {
            if ((i != 0 || j != 0) && (i == 0 || j == 0) && x + i < w && y + j < h) {
                if (check_whether_field_has_only_one_neighbour_that_belongs_to_player(g, player, x + i, y + j)) {
                    (g->free_neighbouring_fields)[player - 1]++;
                }

                uint32_t p = (g->board)[x + i][y + j];
                if (p != 0 && (g->is_the_state_accurate)[p - 1] == false) {
                    (g->is_the_state_accurate)[p - 1] = true;
                    (g->free_neighbouring_fields)[p - 1]--;
                }
            }
        }
    }

    rollback_is_the_state_accurate(g, x, y);
}

void dfs_to_update_dsu(gamma_t *g, uint32_t player, uint32_t x, uint32_t y, uint32_t father_x, uint32_t father_y) {
    (g->parents)[x][y] = make_pair(x, y);
    merge(g, make_pair(father_x, father_y), make_pair(x, y));
    bool a = x != 0 ? 0 : 1;
    bool b = y != 0 ? 0 : 1;

    for (int32_t i = a - 1; i <= 1; i++) {
        for (int32_t j = b - 1; j <= 1; j++) {
            if ((i == 0 || j == 0) && x + i < g->width && y + j < g->height &&
                (g->board)[x + i][y + j] == player &&
                compare((g->parents)[x + i][y + j],
                        make_pair(g->imaginary_vertex_x, g->imaginary_vertex_y))) {
                dfs_to_update_dsu(g, player, x + i, y + j, x, y);
            }
        }
    }
}

bool check_whether_field_has_neighbour_that_belongs_to_player(gamma_t *g, uint32_t player, uint32_t x, uint32_t y) {
    uint32_t w = g->width;
    uint32_t h = g->height;
    bool a = x != 0 ? 0 : 1;
    bool b = y != 0 ? 0 : 1;

    for (int32_t i = a - 1; i <= 1; i++) {
        for (int32_t j = b - 1; j <= 1; j++) {
            if ((i == 0 || j == 0) && x + i < w && y + j < h &&
                (g->board)[x + i][y + j] == player) {
                return true;
            }
        }
    }

    return false;
}

void dfs(gamma_t *g, uint32_t player, uint32_t x, uint32_t y) {
    (g->parents)[x][y] = make_pair(g->imaginary_vertex_x, g->imaginary_vertex_y);
    bool a = x != 0 ? 0 : 1;
    bool b = y != 0 ? 0 : 1;

    for (int32_t i = a - 1; i <= 1; i++) {
        for (int32_t j = b - 1; j <= 1; j++) {
            if ((i == 0 || j == 0) && x + i < g->width && y + j < g->height &&
                (g->board)[x + i][y + j] == player &&
                !compare((g->parents)[x + i][y + j],
                         make_pair(g->imaginary_vertex_x, g->imaginary_vertex_y))) {
                dfs(g, player, x + i, y + j);
            }
        }
    }
}

bool check_number_of_areas_after_golden(gamma_t *g, uint32_t x, uint32_t y, bool type) {
    uint32_t w = g->width;
    uint32_t h = g->height;
    uint32_t counter = 0;
    uint32_t player = (g->board)[x][y];
    (g->parents)[x][y] = make_pair(g->imaginary_vertex_x, g->imaginary_vertex_y);

    bool a = x != 0 ? 0 : 1;
    bool b = y != 0 ? 0 : 1;

    for (int32_t i = a - 1; i <= 1; i++) {
        for (int32_t j = b - 1; j <= 1; j++) {
            if ((i == 0 || j == 0) && x + i < w && y + j < h &&
                (g->board)[x + i][y + j] == player) {
                if (!compare((g->parents)[x + i][y + j],
                             make_pair(g->imaginary_vertex_x, g->imaginary_vertex_y))) {
                    counter++;
                    dfs(g, player, x + i, y + j);
                }
            }
        }
    }

    if ((g->number_of_player_areas)[player - 1] + counter - 1 <= g->areas) {
        if(type == IS_MOVE_POSSIBLE) {
            dfs_to_update_dsu(g, player, x, y, x, y);
            return true;
        } else if(type == MOVE) {
            (g->number_of_player_areas)[player - 1] += counter - 1;
            return true;
        }
    }

    dfs_to_update_dsu(g, player, x, y, x, y);

    return false;
}
