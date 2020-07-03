/** @file
 * Implementacja modułu odpowiadaającego za grę w trybie interaktywnym.
 */

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include "gamma.h"
#include "gamma_structures.h"
#include "interactive_mode.h"
#include "ansi_escape.h"
#include "no_echo_read.h"

static inline void exception_board_equals_to_null(char *board) {
    if (board == NULL) {
        exit(1);
    }
}

static inline void print_final_info_about_player(gamma_t **g, uint32_t player) {
    printf("PLAYER ");
    printf("%" PRIu32, player);
    printf(" ");
    printf("%" PRIu64, gamma_busy_fields(*g, player));
    printf("\n");
}

static inline void print_info_about_player(gamma_t **g, uint32_t player) {
    restore_saved_cursor();
    clear_line();
    printf("PLAYER ");
    printf("%" PRIu32, player);
    printf(" ");
    printf("%" PRIu64, gamma_busy_fields(*g, player));
    printf(" ");
    printf("%" PRIu64, gamma_free_fields(*g, player));
    if (gamma_golden_possible(*g, player)) {
        printf(" G");
    }
}

static bool special_command(int32_t character, bool game_with_many_players,
                            gamma_t **g, uint32_t player, uint32_t column, uint32_t row) {
    if (character == MOVE && gamma_move(*g, player, column, row)) {
        if (!game_with_many_players) {
            printf("%" PRIu32, player);
        }
        return true;
    } else if ((character == GOLDEN_MOVE_BIG_LETTER || character == GOLDEN_MOVE_SMALL_LETTER)
               && gamma_golden_move(*g, player, column, row)) {
        if (!game_with_many_players) {
            printf("%" PRIu32, player);
        }
        return true;
    }

    if (character == END_OF_GAME || character == RESIGN_BIG_LETTER || character == RESIGN_SMALL_LETTER) {
        return true;
    }

    return false;
}

static bool read_arrow(uint32_t *column, uint32_t *row, uint32_t length, bool game_with_many_players,
                       gamma_t **g, uint32_t player, int32_t character) {
    if (character == ARROW_UP && *row < gamma_get_height(*g) - 1) {
        move_up(1);
        (*row)++;
    } else if (character == ARROW_DOWN && *row >= 1) {
        move_down(1);
        (*row)--;
    } else if (character == ARROW_RIGHT && *column < gamma_get_width(*g) - 1) {
        move_right(length);
        (*column)++;
    } else if (character == ARROW_LEFT && *column >= 1) {
        move_left(length);
        (*column)--;
    } else if (character != ARROW_UP && character != ARROW_DOWN
               && character != ARROW_RIGHT && character != ARROW_LEFT
               && special_command(character, game_with_many_players, g, player, *column, *row)) {
        return true;
    }

    return false;
}

static bool make_move(uint32_t length, bool game_with_many_players, gamma_t **g, uint32_t player) {
    uint32_t column = 0, row = 0;
    int32_t character;

    while (true) {
        character = getch();
        if (character == '\033') {
            character = getch();
            if (character == '[') {
                character = getch();
                if (read_arrow(&column, &row, length, game_with_many_players, g, player, character)) {
                    break;
                }
            } else if (special_command(character, game_with_many_players, g, player, column, row)) {
                break;
            }
        } else if (special_command(character, game_with_many_players, g, player, column, row)) {
            break;
        }
    }

    if (character == END_OF_GAME) {
        return false;
    }

    return true;
}

static bool players_move(uint32_t length, bool game_with_many_players, gamma_t **g, uint32_t player) {
    char *board = NULL;
    bool end_of_game = false;

    if (game_with_many_players) {
        clear_screen();
        move_to_the_top_left();
        board = gamma_board(*g);
        exception_board_equals_to_null(board);
        printf("%s", board);
        save_cursor();
    }

    if (!game_with_many_players) {
        restore_saved_cursor();
        clear_line();
    }

    print_info_about_player(g, player);

    restore_saved_cursor();
    move_up(1);
    if (game_with_many_players) {
        length++;
    }

    end_of_game = !make_move(length, game_with_many_players, g, player);
    free(board);

    return end_of_game;
}

static void game_summary(gamma_t **g) {
    first_window();
    clear_screen();
    move_to_the_top_left();
    char *board = gamma_board(*g);
    exception_board_equals_to_null(board);
    printf("%s", board);
    free(board);

    for (uint32_t i = 1; i <= gamma_get_number_of_players(*g); i++) {
        print_final_info_about_player(g, i);
    }
}

static void manage_turns(bool game_with_many_players, gamma_t **g) {
    char *board = gamma_board(*g);
    exception_board_equals_to_null(board);
    uint32_t length = snprintf(NULL, 0, "%" PRIu32, gamma_get_number_of_players(*g));
    uint32_t number_of_players_who_made_move = 1;

    if (!game_with_many_players) {
        new_window();
        printf("%s", board);
        save_cursor();
    }
    free(board);

    while (number_of_players_who_made_move != 0) {
        number_of_players_who_made_move = 0;
        for (uint32_t i = 1; i <= gamma_get_number_of_players(*g); i++) {
            if (gamma_free_fields(*g, i) != 0 || gamma_golden_possible(*g, i)) {
                number_of_players_who_made_move++;
                if (players_move(length, game_with_many_players, g, i)) {
                    game_summary(g);
                    return;
                }
            }
        }
    }

    game_summary(g);
}

void interactive_mode(gamma_t **g) {
    bool game_with_many_players = false;
    if (gamma_get_number_of_players(*g) > 9) {
        game_with_many_players = true;
    }
    manage_turns(game_with_many_players, g);
}
