/** @file
 * Interfejs modułu przygotowującego potrzebne struktury.
 */

#ifndef GAMMA_INIT_DEINIT_H
#define GAMMA_INIT_DEINIT_H

#include <stdbool.h>
#include <stdint.h>

/** @brief Wypełnia strukturę.
 * Wypełnia strukturę NULLAMI i częścią potrzebnych wartości.
 * @param [in,out] new_board - struktura reprezentująca grę,
 * @param [in] width - szerokość planszy,
 * @param [in] height - długość planszy,
 * @param [in] players - liczba graczy,
 * @param [in] areas - maksymalna ilość obszarów jakie może
 * posiadać jeden gracz.
 */
void fill_the_structure_with_starting_values(gamma_t *new_board, uint32_t width,
                                             uint32_t height, uint32_t players,
                                             uint32_t areas);

/** @brief Alokuje pamięć potrzebną na nową grę.
 * @param [in,out] new_board - struktura reprezentująca grę.
 * @return Prawda jeśli udało się zaalokować, fałsz
 * w przeciwnym wypadku.
 */
bool allocate_the_board(gamma_t *new_board);

/** @brief Inicjalizuje grę wartościami początkowymi.
 * @param [in,out] new_board - struktura reprezentująca grę.
 */
void initialize_the_board(gamma_t *new_board);

/** @brief Przygotowuje strukturę zbiorów rozłącznych.
 * @param [in,out] new_board - struktura reprezentująca grę.
 * @return Prawda jeśli udało się zaalokować potrzebną
 * pamięć, fałsz w przeciwnym wypadku.
 */
bool prepare_dsu(gamma_t *new_board);

/** Zwalnia pamięć, która została zaalokowana
 * w przypadku, kiedy nie udało się zaalokować całej potrzebnej pamięci.
 * @param [in,out] g - struktura reprezentująca grę.
 */
void free_memory_exception(gamma_t *g);

#endif /* GAMMA_INIT_DEINIT_H */
