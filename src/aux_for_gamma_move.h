/** @file
 * Interfejs modułu funkcji pomocniczych potrzebnych do
 * wykonania ruchu/złotego ruchu.
 */

#ifndef AUX_FOR_GAMMA_MOVE_H
#define AUX_FOR_GAMMA_MOVE_H

#include "gamma_structures.h"
#include <stdbool.h>
#include <stdint.h>

/** @brief Uaktualnia stan gry po wykonaniu ruchu.
 * @param [in,out] g - struktura reprezentująca grę,
 * @param [in] player - numer gracza, który wykonał ruch,
 * @param [in] x - pierwsza współrzędna pola,
 * @param [in] y - druga współrzędna pola.
 */
void update_the_structure_after_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y);

/** @brief Uaktualnia stan gry po wykonaniu złotego ruchu.
 * @param [in,out] g - struktura reprezentująca grę,
 * @param [in] player - numer gracza, który wykonał ruch,
 * @param [in] x - pierwsza współrzędna pola,
 * @param [in] y - druga współrzędna pola.
 */
void update_the_structure_after_golden_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y);

/** @brief Sprawdza czy złoty ruch przejmujący pole o współrzędnych (x, y) jest możliwy.
 * @param [in,out] g - struktura reprezentująca grę,
 * @param [in] player - numer gracza, który chce wykonać złoty ruch,
 * @param [in] x - pierwsza współrzędna pola,
 * @param [in] y - druga współrzędna pola.
 * @param [in] type - true jeśli wykonywany jest złoty ruch, false jeśli sprawdzana
 * jest możliwość wykonania złotego ruchu.
 * @return Zwraca prawdę jeśli @p player nie wykonał jeszcze
 * złotego ruchu oraz ruch nie spowoduje, że któryś z graczy będzie
 * miał więcej obszarów niż gamma_t.areas.
 */
bool is_golden_possible(gamma_t *g, uint32_t player, uint32_t x, uint32_t y, bool type);

/** @brief Sprawdza czy @p player może wykonać pewien (jakikolwiek) złoty ruch.
 *  @param [in] g - struktura reprezentująca grę.
 *  @param [in] player - numer gracza.
 *  @return Zwraca prawdę jeśli istnieje takie pole, które @p player może przejąć
 *  bez naruszania żadnych zasad gry. Zwraca fałsz jeśli takie pole nie istnieje.
 */
bool check_whether_gamma_golden_is_possible(gamma_t *g, uint32_t player);

#endif /* AUX_FOR_GAMMA_MOVE_H */
