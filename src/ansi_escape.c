/** @file
 * Implementacja modułu udostępniającego operacje wykorzystujące
 * ANSI escape codes.
 */

#include "ansi_escape.h"
#include <stdio.h>

void clear_screen() {
    printf("\x1b[%dJ", CLEAR_ALL);
}

void clear_line() {
    printf("\x1b[%dK", CLEAR_ALL);
}

void new_window() {
    printf("\033[?1049h\033[H");
}

void first_window() {
    printf("\033[?1049l");
}

void move_to_the_top_left() {
    printf("\033[2J\033[1;1H");
}

void move_up(uint32_t positions) {
    printf("\x1b[%dA", positions);
}

void move_down(uint32_t positions) {
    printf("\x1b[%dB", positions);
}

void move_right(uint32_t positions) {
    printf("\x1b[%dC", positions);
}

void move_left(uint32_t positions) {
    printf("\x1b[%dD", positions);
}

void save_cursor() {
    printf("\x1b%d", 7);
}

void restore_saved_cursor() {
    printf("\x1b%d", 8);
}
