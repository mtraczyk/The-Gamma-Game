/** @file
 * Implementacja modułu odpowiadającego za grę w trybie batch mode.
 */

#include "gamma.h"
#include "batch_mode.h"
#include "interactive_mode.h"
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

static inline bool is_character_command(int32_t character) {
    if (character == COMMAND_BATCH || character == COMMAND_INTERACTIVE || character == COMMAND_COMMENT
        || character == COMMAND_GAMMA_BOARD || character == COMMAND_GAMMA_BUSY_FIELDS
        || character == COMMAND_GAMMA_GOLDEN_POSSIBLE || character == COMMAND_GAMMA_FREE_FIELDS
        || character == COMMAND_GAMMA_MOVE || character == COMMAND_GAMMA_GOLDEN_MOVE) {
        return true;
    }

    return false;
}

static inline bool is_character_white(int32_t character) {
    if (character == ' ' || character == '\t' || character == '\v'
        || character == '\f' || character == '\r') {
        return true;
    }

    return false;
}

static bool does_command_have_more_than_two_characters(int32_t character) {
    if (!(is_character_white(character) || character == EOF || character == '\n')) {
        return true;
    }

    return false;
}

static inline bool does_number_overflow(uint8_t number_of_digits, uint32_t number, int32_t character) {
    if (number_of_digits <= 9) {
        return false;
    }

    if ((UINT32_MAX / 10) > number || ((UINT32_MAX / 10) == number && (character - '0') <= 5)) {
        return false;
    }

    return true;
}

static inline bool check_whether_number_is_zero(int32_t *character) {
    if (*character == '0') {
        *character = getchar();
        if (*character == EOF || *character == '\n' || *character == ' ') {
            return true;
        }
    } else {
        return false;
    }

    return false;
}

static inline void read_trash(int32_t *character) {
    while (*character != '\n' && *character != EOF) {
        *character = getchar();
    }
}

static void wrong_input(uint64_t number_of_lines, int32_t *character) {
    read_trash(character);
    fprintf(stderr, "ERROR ");
    fprintf(stderr, "%" PRIu64 "\n", number_of_lines);
}

static void read_spaces(int32_t *character) {
    if (*character != EOF && *character != '\n') {
        *character = getchar();
    }

    while (is_character_white(*character)) {
        *character = getchar();
    }
}

static bool check_whether_there_are_only_space_characters(uint64_t number_of_lines, int32_t *character) {
    read_spaces(character);

    if (*character != '\n' && *character != EOF) {
        wrong_input(number_of_lines, character);
        return false;
    }

    return true;
}

static bool is_batch_mode_on(uint64_t number_of_lines, int32_t *character, uint8_t mode) {
    if (mode == BATCH) {
        return true;
    }
    wrong_input(number_of_lines, character);

    return false;
}

static bool read_number(int32_t *character, uint32_t *number) {
    uint8_t number_of_digits = 1;
    read_spaces(character);

    if (*character == '\n' || *character == EOF || !(*character >= '0' && *character <= '9')) {
        return false;
    }

    if (check_whether_number_is_zero(character)) {
        return true;
    }

    while (!is_character_white(*character) && *character != '\n' && *character != EOF) {
        if (!(*character >= '0' && *character <= '9')) {
            return false;
        }

        if (!does_number_overflow(number_of_digits, *number, *character)) {
            *number = (*number) * (uint32_t) 10 + ((uint32_t) (*character) - (uint32_t) '0');
        } else {
            return false;
        }
        *character = getchar();
        number_of_digits++;
    }

    return true;
}

static bool create_new_board(uint64_t number_of_lines, gamma_t **g, int32_t *character, uint8_t mode) {
    if (mode != NONE) {
        wrong_input(number_of_lines, character);
        return false;
    }

    uint32_t width = 0, height = 0, players = 0, areas = 0;
    if (!read_number(character, &width) || !read_number(character, &height)
        || !read_number(character, &players) || !read_number(character, &areas)) {
        wrong_input(number_of_lines, character);
        return false;
    }

    if (!check_whether_there_are_only_space_characters(number_of_lines, character)) {
        return false;
    }

    *g = gamma_new(width, height, players, areas);
    if (*g == NULL) {
        wrong_input(number_of_lines, character);
        return false;
    }

    return true;
}

static void turn_batch_mode_on(uint64_t number_of_lines, gamma_t **g, int32_t *character, uint8_t *mode) {
    if (create_new_board(number_of_lines, g, character, *mode)) {
        *mode = BATCH;
        printf("OK ");
        printf("%" PRIu64 "\n", number_of_lines);
    }
}

static void turn_interactive_mode_on(uint64_t number_of_lines, gamma_t **g, int32_t *character, uint8_t *mode) {
    if (create_new_board(number_of_lines, g, character, *mode)) {
        *mode = INTERACTIVE;
    }
}

static void execute_gamma_move(uint64_t number_of_lines, gamma_t **g, int32_t *character) {
    uint32_t player = 0, x = 0, y = 0;
    if (!read_number(character, &player) || !read_number(character, &x) || !read_number(character, &y)) {
        wrong_input(number_of_lines, character);
    } else if (check_whether_there_are_only_space_characters(number_of_lines, character)) {
        if (gamma_move(*g, player, x, y)) {
            printf("1\n");
        } else {
            printf("0\n");
        }
    }
}

static void execute_gamma_golden_move(uint64_t number_of_lines, gamma_t **g, int32_t *character) {
    uint32_t player = 0, x = 0, y = 0;
    if (!read_number(character, &player) || !read_number(character, &x) || !read_number(character, &y)) {
        wrong_input(number_of_lines, character);
    } else if (check_whether_there_are_only_space_characters(number_of_lines, character)) {
        if (gamma_golden_move(*g, player, x, y)) {
            printf("1\n");
        } else {
            printf("0\n");
        }
    }
}

static void execute_gamma_golden_possible(uint64_t number_of_lines, gamma_t **g, int32_t *character) {
    uint32_t player = 0;
    if (!read_number(character, &player)) {
        wrong_input(number_of_lines, character);
    } else if (check_whether_there_are_only_space_characters(number_of_lines, character)) {
        if (gamma_golden_possible(*g, player)) {
            printf("1\n");
        } else {
            printf("0\n");
        }
    }
}

static void execute_gamma_free_fields(uint64_t number_of_lines, gamma_t **g, int32_t *character) {
    uint32_t player = 0;
    if (!read_number(character, &player)) {
        wrong_input(number_of_lines, character);
    } else if (check_whether_there_are_only_space_characters(number_of_lines, character)) {
        printf("%" PRIu64 "\n", gamma_free_fields(*g, player));
    }
}

static void execute_gamma_busy_fields(uint64_t number_of_lines, gamma_t **g, int32_t *character) {
    uint32_t player = 0;
    if (!read_number(character, &player)) {
        wrong_input(number_of_lines, character);
    } else if (check_whether_there_are_only_space_characters(number_of_lines, character)) {
        printf("%" PRIu64 "\n", gamma_busy_fields(*g, player));
    }
}

static void execute_gamma_board(uint64_t number_of_lines, gamma_t **g, int32_t *character) {
    if (!check_whether_there_are_only_space_characters(number_of_lines, character)) {
        return;
    }

    char *board = NULL;
    board = gamma_board(*g);
    if (board == NULL) {
        wrong_input(number_of_lines, character);
    } else {
        printf("%s", board);
        free(board);
    }
}

static void execute_commands(uint64_t number_of_lines, gamma_t **g, int32_t *character, int32_t command_character) {
    switch (command_character) {
        case COMMAND_GAMMA_MOVE:
            execute_gamma_move(number_of_lines, g, character);
            break;
        case COMMAND_GAMMA_GOLDEN_MOVE:
            execute_gamma_golden_move(number_of_lines, g, character);
            break;
        case COMMAND_GAMMA_GOLDEN_POSSIBLE:
            execute_gamma_golden_possible(number_of_lines, g, character);
            break;
        case COMMAND_GAMMA_FREE_FIELDS:
            execute_gamma_free_fields(number_of_lines, g, character);
            break;
        case COMMAND_GAMMA_BUSY_FIELDS:
            execute_gamma_busy_fields(number_of_lines, g, character);
            break;
        case COMMAND_GAMMA_BOARD:
            execute_gamma_board(number_of_lines, g, character);
    }
}

static void read_commands(uint64_t number_of_lines, gamma_t **g, int32_t *character, uint8_t *mode) {
    int32_t command_character = *character;
    *character = getchar();
    if (command_character != '#' && does_command_have_more_than_two_characters(*character)) {
        wrong_input(number_of_lines, character);
        return;
    }

    switch (command_character) {
        case COMMAND_BATCH:
            turn_batch_mode_on(number_of_lines, g, character, mode);
            return;
        case COMMAND_INTERACTIVE:
            turn_interactive_mode_on(number_of_lines, g, character, mode);
            if (*mode == INTERACTIVE) {
                interactive_mode(g);
            }
            return;
        case COMMAND_COMMENT:
            read_trash(character);
            return;
    }

    if (is_batch_mode_on(number_of_lines, character, *mode)) {
        execute_commands(number_of_lines, g, character, command_character);
    }
}

static void read_next_line(uint64_t number_of_lines, gamma_t **g, int32_t *character, uint8_t *mode) {
    *character = getchar();
    if (*character == EOF || *character == '\n') {
        return;
    }

    if (!is_character_command(*character)) {
        wrong_input(number_of_lines, character);
        read_trash(character);
        return;
    }

    read_commands(number_of_lines, g, character, mode);
}

void batch_mode() {
    uint64_t number_of_lines = 0;
    gamma_t *g = NULL;
    int32_t character = ' ';
    uint8_t mode = NONE;

    while (character != EOF && mode != INTERACTIVE) {
        number_of_lines++;
        read_next_line(number_of_lines, &g, &character, &mode);
    }

    gamma_delete(g);
}
