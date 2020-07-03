/** @file
 * Implementacja modułu funkcji pomocniczych potrzebnych do
 * wykonania ruchu/złotego ruchu.
 */

#include "aux_for_gamma_move.h"
#include "update_structures_after_move.h"
#include "gamma.h"
#include "gamma_structures.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/** @brief Uaktualnia strukturę zbiorów rozłącznych.
 * @param [in,out] g - struktura reprezentująca grę,
 * @param [in] player - numer gracza, do którego mają należeć pola,
 * @param [in] x - pierwsza współrzędna aktualnie rozważanego pola,
 * @param [in] y - druga współrzędna aktualnie rozważanego pola.
 */
void update_dsu_after_golden_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y) {
    uint32_t w = g->width;
    uint32_t h = g->height;
    bool a = x != 0 ? 0 : 1;
    bool b = y != 0 ? 0 : 1;

    for (int32_t i = a - 1; i <= 1; i++) {
        for (int32_t j = b - 1; j <= 1; j++) {
            if ((i == 0 || j == 0) && x + i < w && y + j < h &&
                (g->board)[x + i][y + j] == player &&
                compare((g->parents)[x + i][y + j],
                        make_pair(g->imaginary_vertex_x, g->imaginary_vertex_y))) {
                dfs_to_update_dsu(g, player, x + i, y + j, x + i, y + j);
            }
        }
    }
}

/** @brief Łączy składowe.
 * Po wykonaniu złotego ruchu, trzeba uaktualnić stan, niektórych składowych
 * należących do gracza, który wykonał złoty ruch i gracza, któremu
 * zabrano pole.
 * @param [in,out] g - struktura reprezentująca grę,
 * @param [in] player - numer gracza, do którego mają należeć pola,
 * @param [in] x - pierwsza współrzędna pola, które zostało przejęte
 * albo utracone,
 * @param [in] y - druga współrzędna pola.
 */
void join_components_after_golden_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y) {
    uint32_t w = g->width;
    uint32_t h = g->height;
    pair p = {x, y};
    bool a = x != 0 ? 0 : 1;
    bool b = y != 0 ? 0 : 1;

    for (int32_t i = a - 1; i <= 1; i++) {
        for (int32_t j = b - 1; j <= 1; j++) {
            if ((i == 0 || j == 0) && x + i < w && y + j < h &&
                (g->board)[x + i][y + j] == player) {
                pair p2 = {x + i, y + j};
                pair c1 = find(g, p);
                pair c2 = find(g, p2);

                if (c1.f != c2.f || c1.s != c2.s) {
                    (g->number_of_player_areas)[player - 1]--;
                    merge(g, p, p2);
                }
            }
        }
    }
}

void update_the_structure_after_golden_move(gamma_t *g, uint32_t player,
                                            uint32_t x, uint32_t y) {
    uint32_t player2 = (g->board)[x][y];
    pair p = {x, y};

    (g->number_of_player_fields)[player - 1]++;
    (g->number_of_player_areas)[player - 1]++;
    (g->number_of_player_fields)[player2 - 1]--;
    (g->parents)[x][y] = p;
    (g->is_golden_move_used)[player - 1] = true;

    update_number_of_free_fields_after_golden(g, player, x, y);

    join_components_after_golden_move(g, player, x, y);

    update_dsu_after_golden_move(g, player2, x, y);
}

/** @brief Uaktualnia strukturę zbiorów rozłączych po wykonaniu ruchu.
 * @param [in,out] g - struktura reprezentująca grę,
 * @param [in] player - numer gracza, który wykonał ruch,
 * @param [in] x - pierwsza współrzędna pola, na które wykonał
 * ruch @p player.
 * @param [in] y - druga współrzędna pola.
 */
void update_dsu_after_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y) {
    bool a = x != 0 ? 0 : 1;
    bool b = y != 0 ? 0 : 1;
    uint32_t w = g->width;
    uint32_t h = g->height;

    for (int32_t i = a - 1; i <= 1; i++) {
        for (int32_t j = b - 1; j <= 1; j++) {
            if ((i != 0 || j != 0) && (i == 0 || j == 0) && x + i < w && y + j < h &&
                ((g->board)[x + i][y + j] == player)) {
                pair p1 = {x, y};
                pair p2 = {x + i, y + j};
                pair c1 = find(g, p1);
                pair c2 = find(g, p2);

                if (c1.f != c2.f || c1.s != c2.s) {
                    (g->number_of_player_areas)[player - 1]--;
                    merge(g, p1, p2);
                }
            }
        }
    }
}

void update_the_structure_after_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y) {
    (g->board)[x][y] = player;
    (g->number_of_player_fields)[player - 1]++;
    (g->number_of_player_areas)[player - 1]++;
    g->free_fields--;

    update_dsu_after_move(g, player, x, y);

    update_number_of_free_fields_after_move(g, player, x, y);
}

bool check_whether_gamma_golden_is_possible(gamma_t *g, uint32_t player) {
    for (uint32_t i = 0; i < g->width; i++) {
        for (uint32_t j = 0; j < g->height; j++) {
            if ((g->board)[i][j] == player || (g->board)[i][j] == 0) {
                continue;
            } else if (is_golden_possible(g, player, i, j, false)) {
                return true;
            }
        }
    }

    return false;
}

bool is_golden_possible(gamma_t *g, uint32_t player, uint32_t x, uint32_t y, bool type) {
    if (((g->number_of_player_areas)[player - 1] < g->areas ||
         check_whether_field_has_neighbour_that_belongs_to_player(g, player, x, y)) &&
        check_number_of_areas_after_golden(g, x, y, type)) {
        return true;
    }

    return false;
}
