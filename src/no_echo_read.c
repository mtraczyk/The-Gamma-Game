/** @file
 * Implementacja modułu pozwalającego wczytywać znaki wpisywane
 * na klawiaturze w trybie no echo. Kod pochodzi ze strony :
 * https://stackoverflow.com/questions/7469139/what-is-the-equivalent-to-getch-getche-in-linux?fbclid=IwAR1yfLwZGcLLj1C7YijQGj4LCVIcI9jNgc1N9JqxjKZ5g4pnyZEc1WomqGg
 * Wprowadzone zostały niewielkie zmiany.
 */

#include <termios.h>
#include <stdio.h>
#include "no_echo_read.h"

static struct termios old, current;

/* Initialize new terminal i/o settings */
static inline void init_termios(int32_t echo) {
    tcgetattr(0, &old); /* grab old terminal i/o settings */
    current = old; /* make new settings same as old settings */
    current.c_lflag &= ~ICANON; /* disable buffered i/o */
    if (echo) {
        current.c_lflag |= ECHO; /* set echo mode */
    } else {
        current.c_lflag &= ~ECHO; /* set no echo mode */
    }
    tcsetattr(0, TCSANOW, &current); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
static inline void reset_termios() {
    tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
static int32_t getch_aux(int32_t echo) {
    init_termios(echo);
    int32_t ch = getchar();
    reset_termios();
    return ch;
}

/* Read 1 character without echo */
int32_t getch() {
    return getch_aux(0);
}
