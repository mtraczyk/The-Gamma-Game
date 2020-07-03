/** @file
 * Interfejs modułu pozwalającego wczytywać znaki wpisywane
 * na klawiaturze w trybie no echo.
 */

#include <inttypes.h>

#ifndef NO_ECHO_READ_H
#define NO_ECHO_READ_H

/** @brief Umożliwia wczytanie znaku w trybie noecho.
 * @return Kod ASCII wczytanego znaku.
 */
extern int32_t getch();

#endif /* NO_ECHO_READ_H */
