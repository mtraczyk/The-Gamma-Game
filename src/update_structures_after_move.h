/** @file
 * Interfejs modułu funkcji pomocniczych potrzebnych do
 * wykonania ruchu/złotego ruchu.
 */

#ifndef UPDATE_STRUCTURES_AFTER_MOVE_H
#define UPDATE_STRUCTURES_AFTER_MOVE_H

#include <stdbool.h>
#include <stdint.h>

typedef struct pair pair;     ///< Para liczb całkowitych.
typedef struct gamma gamma_t; ///< Struktura reprezentująca grę.

enum Type_of_use {
    MOVE = true,
    IS_MOVE_POSSIBLE = false
};


/** @brief Uaktualnia liczbę wolnych pól po wykorzystaniu
 * złotego ruchu.
 * @param [in,out] g - struktura reprezentująca grę,
 * @param [in] player - numer gracza, który wykonał złoty ruch,
 * @param [in] x - pierwsza współrzędna pola, które zajął gracz @p player,
 * @param [in] y - druga współrzędna pola, które zajął gracz @p player.
 */
void update_number_of_free_fields_after_golden(gamma_t *g, uint32_t player, uint32_t x, uint32_t y);

/** @brief Uaktualnia liczbę wolnych pól po wykonaniu ruchu.
 * @param [in,out] g - struktura reprezentująca grę,
 * @param [in] player - numer gracza, który wykonał ruch,
 * @param [in] x - pierwsza współrzędna pola, które zajął gracz @p player,
 * @param [in] y - druga współrzędna pola, które zajął gracz @p player.
 */
void update_number_of_free_fields_after_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y);

/** @brief Przeszukiwanie w głąb, które uaktualnia stan struktury
 * zbiorów rozłącznych.
 * @param [in,out] g - struktura reprezentująca grę,
 * @param [in] player - numer gracza, którego obszary należy uaktualnić,
 * @param [in] x - pierwsza współrzędna pola, na którym obecnie znajduje się
 * dfs,
 * @param [in] y - druga współrzędna pola, na którym obecnie znajduje się dfs,
 * @param [in] father_x - pierwsza współrzędna pola, z którego dostaliśmy się
 * do obecnego,
 * @param [in] father_y - druga współrzędna pola, z którego dostaliśmy się
 * do obecnego.
 */
void dfs_to_update_dsu(gamma_t *g, uint32_t player, uint32_t x, uint32_t y, uint32_t father_x, uint32_t father_y);

/** @brief Sprawdza czy pole należące do gracza @p player, sąsiaduje
 * z innym polem tego samego gracza.
 * @param [in] g - struktura reprezentująca grę,
 * @param [in] player - gracz, do którego należy pole o współrzędnych @p x, @p
 * y,
 * @param [in] x - pierwsza współrzędna pola,
 * @param [in] y - druga współrzędna pola.
 * @return Prawda jeśli istnieje takie pole, fałsz w przeciwnym wypadku.
 */
bool check_whether_field_has_neighbour_that_belongs_to_player(gamma_t *g, uint32_t player, uint32_t x, uint32_t y);

/** @brief Przeszukiwanie w głąb.
 * Służy do sprawdzenia na ile składowych rozspójnia się graf, po wykonaniu
 * ruchu. Informacja o tym czy pole zostało odwiedzone jest przechowywana
 * w tablicy @p parents. Jeśli pole zostało odwiedzone, znajdują się tam
 * współrzędne dodatkowego wierzchołka : gamma_t.imaginary_vertex_x,
 * gamma_t.imaginary_vertex_y.
 * @param [in] g - struktura reprezentująca grę,
 * @param [in] player - numer gracza, do którego mają należeć odwiedzane pola,
 * @param [in] x - pierwsza współrzędna aktualnego wierzchołka,
 * @param [in] y - druga współrzędna aktualnego wierzchołka.
 */
void dfs(gamma_t *g, uint32_t player, uint32_t x, uint32_t y);

/** @brief Sprawdza na ile obszarów rozspójni się gracz, do którego
 * aktualnie należy pole @p x, @p y.
 * @param [in,out] g - struktura reprezentująca grę,
 * @param [in] x - pierwsza współrzędna pola,
 * @param [in] y - druga współrzędna pola.
 * @param [in] type - true jeśli wykonywany jest złoty ruch, false jeśli sprawdzana
 * jest możliwość wykonania złotego ruchu.
 * @return Prawda jeśli gracz do, którego należy pole @p x, @p y, po zabraniu
 * mu tego pola nie rozspójni się na więcej obszarów niż gamma_t.areas.
 * Fałsz w przeciwnym wypadku.
 */
bool check_number_of_areas_after_golden(gamma_t *g, uint32_t x, uint32_t y, bool type);

#endif /* UPDATE_STRUCTURES_AFTER_MOVE_H */
