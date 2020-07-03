/** @file
 * Interfejs modułu udostępniającego operacje wykorzystujące
 * ANSI escape codes.
 */

#include <inttypes.h>

#ifndef ANSI_ESCAPE_H
#define ANSI_ESCAPE_H

/// Pomocnicze oznaczenie dla kodów ANSI czyszczących ekran terminala.
enum Clear_codes {
    CLEAR_ALL = 2
};

extern void clear_screen(); ///< Czyści ekran.

extern void clear_line();  ///< Czyści pojedynczą linię.

extern void new_window(); ///< Wyświetla nowy ekran.

extern void first_window(); ///< Wraca do pierwszego ekranu.

extern void move_to_the_top_left(); ///< Przesuwa kursor w lewy górny róg ekranu.

/** @brief Przesuwa kursor do góry o zadaną liczbę pól.
 * @param[in] positions - liczba pozycji, o którą ma zostać
 * przesunięty kursor.
 */
extern void move_up(uint32_t positions);

/** @brief Przesuwa kursor do dołu o zadaną liczbę pól.
 * @param[in] positions - liczba pozycji, o którą ma zostać
 * przesunięty kursor.
 */
extern void move_down(uint32_t positions);

/** @brief Przesuwa kursor w prawo o zadaną liczbę pól.
 * @param[in] positions - liczba pozycji, o którą ma zostać
 * przesunięty kursor.
 */
extern void move_right(uint32_t positions);

/** @brief Przesuwa kursor w lewo o zadaną liczbę pól.
 * @param[in] positions - liczba pozycji, o którą ma zostać
 * przesunięty kursor.
 */
extern void move_left(uint32_t positions);

extern void save_cursor(); ///< Zapisuje aktualną pozycję kursora.

extern void restore_saved_cursor(); ///< Przywraca wcześniej zapisaną pozycję kursora.

#endif /* ANSI_ESCAPE_H */
