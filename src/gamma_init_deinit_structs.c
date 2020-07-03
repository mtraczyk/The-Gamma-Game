/** @file
 * Implementacja modułu przygotowującego potrzebne struktury.
 */

#include "gamma.h"
#include "gamma_structures.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void fill_the_structure_with_starting_values(gamma_t *new_board, uint32_t width,
                                             uint32_t height, uint32_t players,
                                             uint32_t areas) {
    new_board->width = width;
    new_board->height = height;
    new_board->players = players;
    new_board->areas = areas;
    new_board->free_fields = (uint64_t) width * (uint64_t) height;
    new_board->imaginary_vertex_x = width;
    new_board->imaginary_vertex_y = height;
    new_board->parents = NULL;
    new_board->free_neighbouring_fields = NULL;
    new_board->number_of_player_fields = NULL;
    new_board->number_of_player_areas = NULL;
    new_board->is_golden_move_used = NULL;
    new_board->is_the_state_accurate = NULL;
}

void initialize_the_board(gamma_t *new_board) {
    for (uint32_t i = 0; i < new_board->width; i++)
        for (uint32_t j = 0; j < new_board->height; j++) {
            (new_board->board)[i][j] = 0;
        }

    for (uint32_t i = 0; i < new_board->players; i++) {
        (new_board->number_of_player_fields)[i] = 0;
        (new_board->number_of_player_areas)[i] = 0;
        (new_board->is_golden_move_used)[i] = false;
        (new_board->free_neighbouring_fields)[i] = 0;
        (new_board->is_the_state_accurate)[i] = false;
    }
}

bool allocate_the_board(gamma_t *new_board) {
    uint32_t w = new_board->width;
    uint32_t h = new_board->height;
    uint32_t p = new_board->players;

    new_board->board = malloc(w * sizeof(uint32_t *));
    if (new_board->board == NULL) {
        return false;
    }

    for (uint32_t i = 0; i < w; i++) {
        (new_board->board)[i] = NULL;
    }

    for (uint32_t i = 0; i < w; i++) {
        (new_board->board)[i] = malloc(h * sizeof(uint32_t));
        if ((new_board->board)[i] == NULL) {
            return false;
        }
    }

    new_board->free_neighbouring_fields = malloc(p * sizeof(uint64_t));
    new_board->number_of_player_fields = malloc(p * sizeof(uint64_t));
    new_board->number_of_player_areas = malloc(p * sizeof(uint32_t));
    new_board->is_golden_move_used = malloc(p * sizeof(bool));
    new_board->is_the_state_accurate = malloc(p * sizeof(bool));

    if (new_board->free_neighbouring_fields == NULL ||
        new_board->number_of_player_fields == NULL ||
        new_board->number_of_player_areas == NULL ||
        new_board->is_golden_move_used == NULL) {
        return false;
    }

    initialize_the_board(new_board);
    return true;
}

bool prepare_dsu(gamma_t *new_board) {
    uint32_t w = new_board->width;
    uint32_t h = new_board->height;

    new_board->parents = malloc(w * sizeof(pair *));
    if (new_board->parents == NULL) {
        return false;
    }

    for (uint32_t i = 0; i < w; i++) {
        (new_board->parents)[i] = NULL;
    }

    for (uint32_t i = 0; i < w; i++) {
        (new_board->parents)[i] = malloc(h * sizeof(pair));

        if ((new_board->parents)[i] == NULL) {
            return false;
        }

        for (uint32_t j = 0; j < h; j++) {
            (new_board->parents)[i][j] = make_pair(i, j);
        }
    }

    return true;
}

void free_memory_exception(gamma_t *g) {
    if (g == NULL) {
        return;
    }

    for (uint32_t i = 0; i < g->width; i++) {
        if (g->board != NULL && (g->board)[i] != NULL) {
            free((g->board)[i]);
        } else {
            break;
        }
    }

    for (uint32_t i = 0; i < g->width; i++) {
        if (g->parents != NULL && (g->parents)[i] != NULL) {
            free((g->parents)[i]);
        } else {
            break;
        }
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
