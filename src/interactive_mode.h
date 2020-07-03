/** @file
 * Interfejs modułu odpowiadającego za grę w trybie interaktywnym.
 */

#ifndef INTERACTIVE_MODE_H
#define INTERACTIVE_MODE_H

#include "gamma.h"

/// Oznaczenie na koniec gry.
enum End_of_game {
    END_OF_GAME = 4
};

/// Pomocnicze oznaczenia na strzałki.
enum Arrows {
    ARROW_UP = 'A',
    ARROW_DOWN = 'B',
    ARROW_RIGHT = 'C',
    ARROW_LEFT = 'D'
};

/// Pomocnicze oznaczenia dla komend specjalnych.
enum Special_command {
    MOVE = ' ',
    GOLDEN_MOVE_BIG_LETTER = 'G',
    GOLDEN_MOVE_SMALL_LETTER = 'g',
    RESIGN_BIG_LETTER = 'C',
    RESIGN_SMALL_LETTER = 'c'
};

/** @brief Uruchamia rozgrywkę w trybie interaktywnym.
 * @param[in] g - wskaźnik na wskaźnik, na strukturę przechowującą stan gry.
 */
extern void interactive_mode(gamma_t **g);

#endif /* INTERACTIVE_MODE_H */
