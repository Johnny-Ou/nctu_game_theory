#include <stdlib.h>
#include <stdio.h>
#include "graph.h"
#include "game.h"

int main(int argc, char *argv[]){
    if(argc<3){
        fprintf(stderr, "Usage: ./hw1 [num_vertices] [edges_per_vertex]\n");
        exit(-1);
    }
    int n = atoi(argv[1]);
    int k = atoi(argv[2]);
    fprintf(stderr,
        "Configuration:\n"
        "  Vertices:         %d\n"
        "  Edges per vertex: %d\n\n",
        n, k);
    
    float *counts = (float*)malloc(sizeof(float) * 5);
    float *cardinalitys = (float*)malloc(sizeof(float) * 5);
    int index = 0;
    int loop =100;
    for(float p=0.0; p<=0.81; p+=0.2, index++){
        counts[index] = 0.0;
        cardinalitys[index] = 0;
        for(int i=0; i<loop; i++){
            struct graph_t *g = create_graph(n, k, p);
            ring_topology(g);
            rewire_graph(g);
            if(!check_graph(g))    printf("Graph edge false!\n");
            
            game(g);
            counts[index] += (float)g->count/(float)g->n;
            cardinalitys[index] += (float)g->cardinality;

            free(g->strategys);
            free(g->edges);
            free(g->IDS);
            free(g);
        }
        counts[index] /= (float)loop;
        cardinalitys[index] /= (float)loop;
        printf("[prob_of_swap:%f ] move count=%.2f    cardinality=%.2f\n", p, counts[index], cardinalitys[index]);
    }
    plot_counts(counts);
    plot_cardinalitys(cardinalitys);

    return 0;
}