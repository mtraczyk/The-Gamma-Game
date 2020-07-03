/** @file
 * Interfejs modułu odpowiadającego za grę w trybie batch mode.
 */

#ifndef BATCH_MODE_H
#define BATCH_MODE_H

/// Oznaczenia dla trybów rozgrywek.
enum Mode {
    NONE = 0, BATCH = 1, INTERACTIVE = 2
};

/// Oznaczenia dla komend w trybie wsadowym.
enum Command {
    COMMAND_BATCH = 'B',
    COMMAND_INTERACTIVE = 'I',
    COMMAND_GAMMA_MOVE = 'm',
    COMMAND_GAMMA_GOLDEN_MOVE = 'g',
    COMMAND_GAMMA_BUSY_FIELDS = 'b',
    COMMAND_GAMMA_FREE_FIELDS = 'f',
    COMMAND_GAMMA_GOLDEN_POSSIBLE = 'q',
    COMMAND_GAMMA_BOARD = 'p',
    COMMAND_COMMENT = '#'
};

extern void batch_mode(); ///< Uruchamia batch mode.

#endif /* BATCH_MODE_H */
