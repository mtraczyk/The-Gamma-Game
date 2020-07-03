/** @file
 * Definicje struktur wykorzystywanych w programie.
 * Deklaracje funkcji dotyczących
 * wyżej zdefiniowanych struktur.
 */

#ifndef GAMMA_STRUCTERS_H
#define GAMMA_STRUCTERS_H

#include <stdbool.h>
#include <stdint.h>

/**
 * Struktura przechowująca pary dodatnich liczb całkowitych.
 */
typedef struct pair pair;
/**
 * Struktura przechowująca stan gry.
 */
typedef struct gamma gamma_t;

/** @brief Para liczb całkowitych.
 */
struct pair {
    uint32_t f; ///< Pierwsza liczba z pary.
    uint32_t s; ///< Druga liczba z pary.
};

/** @brief Struktura reprezentująca grę.
 */
struct gamma {
    uint32_t width;       ///< Szerokość planszy.
    uint32_t height;      ///< Wysokość planszy.
    uint32_t players;     ///< Liczba graczy.
    uint32_t areas;       ///< Liczba maksymalnej ilości obszarów.
    uint64_t free_fields; ///< Liczba wolnych pól.
    /// Pierwsza współrzędna dodatkowego wierzchołka.
    uint32_t imaginary_vertex_x;
    /// Druga współrzędna dodatkowego wierzchołka.
    uint32_t imaginary_vertex_y;
    /**
     * Tablica, w której przechowywana jest informacja
     * o tym, z iloma wolnymi polami sąsiadauje dany gracz.
     */
    uint64_t *free_neighbouring_fields;
    uint32_t **board; ///< Do kogo należy pole.
    /**
     * Tablica, w której przechowywana jest informacja o rodzicu
     * danego pola. Jest wykorzystywana w implementacji struktury
     * zbiorów rozłącznych.
     */
    pair **parents;
    /// Ile pól należy do danego gracza.
    uint64_t *number_of_player_fields;
    /// Ile obszarów należy do danego gracza.
    uint32_t *number_of_player_areas;
    /// Czy gracz użył już złotego ruchu.
    bool *is_golden_move_used;
    /// Tablica pomocnicza.
    bool *is_the_state_accurate;
};

/** @brief Tworzy parę.
 * @param [in] x - pierwsza współrzędna pary, liczba dodatnia,
 * @param [in] y - druga współrzędna pary, liczba dodatnia.
 * @return Utworzona para.
 */
extern pair make_pair(uint32_t x, uint32_t y);

/** @brief Porównuje dwie pary.
 * @param [in] a - pierwsza para,
 * @param [in] b - druga para.
 * @return Prawda jeśli pary są takie same,
 * fałsz w przeciwnym wypadku.
 */
extern bool compare(pair a, pair b);

/** @brief Znajduje reprezentanta danej składowej.
 * @param [in] g - struktura reprezentująca grę,
 * @param [in] x - wierzchołek w grafie.
 * @return Para reprezentująca składową, do której należy
 * wierzchołek @p x.
 */
extern pair find(gamma_t *g, pair x);

/** @brief Łączy dwie składowe grafu.
 * @param [in,out] g - struktura reprezentująca grę,
 * @param [in] x - reprezentant pierwszej składowej,
 * @param [in] y - reprezentant drugiej składowej.
 */
extern void merge(gamma_t *g, pair x, pair y);

#endif /* GAMMA_STRUCTERS_H */
