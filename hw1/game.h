#ifndef GAME_H
#define GAME_H

#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "graph.h"

#define alpha       15
#define gamma(n)    n * alpha + 3
#define beta        10


int v(struct graph_t *g, int i);
int gain(struct graph_t *g, int i);
int w(struct graph_t *g, int i);
int u(struct graph_t *g, int i);
bool br(struct graph_t *g, int i);
bool nq(struct graph_t *g);
void init(struct graph_t *g);
void game(struct graph_t *g);

#endif