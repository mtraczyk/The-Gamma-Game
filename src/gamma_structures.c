/** @file
 * Implementacja funkcji potrzebnych do różnych
 * operacji na strukturach wykorzystywanych w programie.
 */

#include "gamma_structures.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

pair make_pair(uint32_t x, uint32_t y) {
    pair p = {x, y};

    return p;
}

bool compare(pair a, pair b) {
    if (a.f == b.f && a.s == b.s) {
        return true;
    }

    return false;
}

pair find(gamma_t *g, pair x) {
    pair to_check = (g->parents)[x.f][x.s];

    if (to_check.f != x.f || to_check.s != x.s)
        ((g->parents)[x.f][x.s] = find(g, (g->parents)[x.f][x.s]));

    return (g->parents)[x.f][x.s];
}

void merge(gamma_t *g, pair x, pair y) {
    pair a = find(g, x);
    pair b = find(g, y);

    (g->parents)[b.f][b.s] = a;
}
