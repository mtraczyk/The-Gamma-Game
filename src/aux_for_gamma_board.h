/** @file
 * Interfejs modułu funkcji pomocniczych potrzebnych do
 * przygotowania bufora z reprezentacją planszy.
 */

#ifndef AUX_FOR_GAMMA_BOARD_H
#define AUX_FOR_GAMMA_BOARD_H

#include <stdbool.h>
#include <stdint.h>

/** @brief Sprawdza poprawność argumentów.
 * @param [in] g - struktura reprezentująca grę,
 * @param [in] player - numer gracza,
 * @return Prawda, jeśli @p g nie jest NULLEM i numer gracza
 * jest dodatnią liczbą całkowitą nie większą od gamma_t.players.
 * Fałsz w przeciwnym wypadku.
 */
bool are_arguments_correct(gamma_t *g, uint32_t player);

/** @brief Zwraca bufor z reprezentacją planszy.
 * Wywoływana w przypadku, gdy gamma_t.players >= 10.
 * @param [in] g - struktura reprezentująca grę.
 * @return Bufor, jeśli starczyło pamięci na jego zaalokowanie.
 * NULL w przeciwnym wypadku.
 */
char *gamma_board_with_many_players(gamma_t *g);

/** @brief Zwraca bufor z reprezentacją planszy.
 * Wywoływana w przypadku, gdy gamma_t.players < 10.
 * @param [in] g - struktura reprezentująca grę.
 * @return Bufor, jeśli starczyło pamięci na jego zaalokowanie.
 * NULL w przeciwnym wypadku.
 */
char *gamma_board_with_less_than_10_players(gamma_t *g);

#endif /* AUX_FOR_GAMMA_BOARD_H */

