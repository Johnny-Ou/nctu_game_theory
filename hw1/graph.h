#ifndef GRAPH_H
#define GRAPH_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>

struct graph_t{
    bool *edges;
    bool *strategys;
    int n;
    int k;
    float p;
    int *IDS;
    int count;
    int cardinality;
};

struct graph_t *create_graph(int n, int k, float p);
void ring_topology(struct graph_t *g);
void rewire_graph(struct graph_t *g);
void set_edge(struct graph_t *g, int i, int j, bool b);
bool get_edge(struct graph_t *g, int i, int j);
int get_degree(struct graph_t *g, int i);
bool is_independent(struct graph_t *g);
bool is_domination(struct graph_t *g);
bool check_graph(struct graph_t *g);
void plot_counts(float *counts);
void plot_cardinalitys(float *cardinalitys);

#endif