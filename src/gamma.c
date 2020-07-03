/** @file
 * Implementacja modułu przechowującego stan gry gamma.
 */

#include "gamma.h"
#include "aux_for_gamma_board.h"
#include "aux_for_gamma_move.h"
#include "update_structures_after_move.h"
#include "gamma_init_deinit_structs.h"
#include "gamma_structures.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

gamma_t *gamma_new(uint32_t width, uint32_t height, uint32_t players, uint32_t areas) {
    gamma_t *new_board = NULL;

    if (width > 0 && height > 0 && players > 0 && areas > 0) {
        new_board = malloc(sizeof(gamma_t));
        if (new_board == NULL) {
            return NULL;
        }

        fill_the_structure_with_starting_values(new_board, width, height, players, areas);

        if (!allocate_the_board(new_board) || !prepare_dsu(new_board)) {
            free_memory_exception(new_board);
            return NULL;
        }
    }

    return new_board;
}

void gamma_delete(gamma_t *g) {
    if (g != NULL) {
        for (uint32_t i = 0; i < g->width; i++) {
            free((g->board)[i]);
            free((g->parents)[i]);
        }

        free(g->board);
        free(g->parents);
        free(g->free_neighbouring_fields);
        free(g->number_of_player_fields);
        free(g->number_of_player_areas);
        free(g->is_golden_move_used);
        free(g->is_the_state_accurate);
        free(g);
    }
}

bool gamma_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y) {
    if (g == NULL || x >= (g->width) || y >= (g->height) ||
        player > (g->players) || player == 0 || (g->board)[x][y] != 0) {
        return false;
    }

    if ((g->number_of_player_areas)[player - 1] < g->areas ||
        ((g->number_of_player_areas)[player - 1] == g->areas &&
         check_whether_field_has_neighbour_that_belongs_to_player(g, player, x, y))) {
        update_the_structure_after_move(g, player, x, y);
        return true;
    }

    return false;
}

bool gamma_golden_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y) {
    if (g == NULL || x >= (g->width) || y >= (g->height) ||
        player > (g->players) || player == 0 ||
        (g->is_golden_move_used)[player - 1] || (g->board)[x][y] == player || (g->board)[x][y] == 0) {
        return false;
    }

    if (is_golden_possible(g, player, x, y, true)) {
        update_the_structure_after_golden_move(g, player, x, y);
        return true;
    }

    return false;
}

uint32_t gamma_get_width(gamma_t *g) {
    return g->width;
}

uint32_t gamma_get_height(gamma_t *g) {
    return g->height;
}

uint32_t gamma_get_number_of_players(gamma_t *g) {
    return g->players;
}

uint64_t gamma_busy_fields(gamma_t *g, uint32_t player) {
    if (!are_arguments_correct(g, player)) {
        return 0;
    }

    return (uint64_t) (g->number_of_player_fields)[player - 1];
}

uint64_t gamma_free_fields(gamma_t *g, uint32_t player) {
    if (!are_arguments_correct(g, player)) {
        return 0;
    }

    if ((g->number_of_player_areas)[player - 1] < g->areas) {
        return g->free_fields;
    }

    return (g->free_neighbouring_fields)[player - 1];
}

bool gamma_golden_possible(gamma_t *g, uint32_t player) {
    if (!are_arguments_correct(g, player)) {
        return false;
    }

    if ((g->is_golden_move_used)[player - 1] ||
        (uint64_t) g->width * (uint64_t) g->height - g->free_fields - (g->number_of_player_fields)[player - 1] == 0) {
        return false;
    }

    return check_whether_gamma_golden_is_possible(g, player);
}

char *gamma_board(gamma_t *g) {
    if (g == NULL)
        return NULL;

    if (g->players < (uint32_t) 10) {
        return gamma_board_with_less_than_10_players(g);
    }

    return gamma_board_with_many_players(g);
}

void gamma_board_print(gamma_t *g) {
    char *board = gamma_board(g);
    if (board != NULL) {
        printf("%s", board);
    }
    free(board);
}
