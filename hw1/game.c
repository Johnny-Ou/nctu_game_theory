#include "game.h"
#include <stdio.h>

//  Return how much number in the set for neighbor include itself.
int v(struct graph_t *g, int i){
    int ans = 0;
    for(int j=0; j<g->n; j++){
        if(get_edge(g, i, j) || j==i){  //  M_i
            ans = (g->strategys[j]) ? ans+1 : ans;
        }
    }
    return ans;
}
//  if there is only one node in the set, then get porfit.
int gain(struct graph_t *g, int i){
    return (v(g, i)==1) ? alpha : 0;
}
//  if there are both p_i and neighbor in the set, then get penalty.
int w(struct graph_t *g, int i){
    int ans = 0.0;
    for(int j=0; j<g->n; j++){
        if(get_edge(g, i, j)){  // N_i
            ans += g->strategys[i]*g->strategys[j]*gamma(g->n);
        }
    }
    return ans;
}
//  Utility function for p_i
int u(struct graph_t *g, int i){
    if(!g->strategys[i])    return 0;

    int profit = 0, penalty = 0;
    for(int j=0; j<g->n; j++){
        if(get_edge(g, i, j) || j==i){  // M_i
            profit += gain(g, j);
        }
    }
    
    penalty = beta + w(g, i);
    return (profit - penalty);
}
//  Best response
bool br(struct graph_t *g, int i){
    int utility1 = u(g, i);
    g->strategys[i] = !g->strategys[i];
    int utility2 = u(g, i);
    g->strategys[i] = !g->strategys[i];

    return utility1 >= utility2;
}
//  NE
bool nq(struct graph_t *g){
    for(int i=0; i<g->n; i++){
        if(!br(g, i))    return false;
    }
    return true;
}
void init(struct graph_t *g){
    srand((unsigned)time(0));
    for(int i=0; i<g->n; i++){
        g->strategys[i] = rand()%2 ? true : false;
    }
}
void game(struct graph_t *g){
    init(g);
    g->count = 0;
    g->cardinality = 0;
    while(!nq(g)){
        while(1){
            int i = rand() % g->n;
            if(!br(g, i)){
                g->strategys[i] = !g->strategys[i];
                (g->count)++;
                break;
            }  
        }
    }

    for(int i=0; i<g->n; i++){
        g->cardinality = g->strategys[i] ? (g->cardinality)+1 : g->cardinality;
    }

    g->IDS = (int*)malloc(sizeof(int)*(g->cardinality));
    for(int i=0, j=0; i<g->n; i++){
        if(g->strategys[i])
            g->IDS[j++] = i;
    }

    if(!is_independent(g)){
        printf("It's not independent set!\n\n");
        exit(-1);
    }
    if(!is_domination(g)){
        printf("It's not domination set!\n\n");
        exit(-1);
    }
}