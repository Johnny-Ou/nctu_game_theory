#include "graph.h"


struct graph_t *create_graph(int n, int k, float p){
    struct graph_t *g = (struct graph_t*)malloc(sizeof(struct graph_t));
    g->n = n;
    g->k = k;
    g->p = p;
    size_t count = (size_t)n * (size_t)n;
    g->edges = (bool*)calloc(count, sizeof(bool));
    assert(g->edges);
    g->strategys = (bool*)calloc((size_t)n, sizeof(bool));
    assert(g->strategys);

    return g;
}
void ring_topology(struct graph_t *g){
    int temp1, temp2 = g->n - 1 - (g->k>>1);
    for(int i=0; i<g->n; i++){
        for(int j=0; j<g->n; j++){
            temp1 = abs(i-j) % temp2;
            if(temp1>0 && temp1<=(g->k>>1))
                set_edge(g, i, j, true);
        }
    }
}
void rewire_graph(struct graph_t *g){
    srand((unsigned)time(0));
    int j, new;
    for(int k=1; k<=(g->k>>1); k++){
        for(int i=0; i<g->n; i++){
            j = (i+k) % g->n;

            if(get_edge(g, i, j) && (float)rand()/(float)RAND_MAX < g->p && get_degree(g, j)!=1 && get_degree(g, i)!=g->n-1){
                set_edge(g, i, j, false);
                do{
                    new = (int)rand() % g->n;
                }while(new==i || get_edge(g, i, new));
                set_edge(g, i, new, true);
                // printf("[%d, %d] -> [%d, %d]\n", i, j, i, new);
            }
        }
    }
}
void set_edge(struct graph_t *g, int i, int j, bool b){
    g->edges[i*g->n + j] = b;
    g->edges[j*g->n + i] = b;
}
bool get_edge(struct graph_t *g, int i, int j){
    return g->edges[i*g->n + j];
}
int get_degree(struct graph_t *g, int i){
    int degree = 0;
    for(int j=0; j<g->n; j++){
        degree = get_edge(g, i, j) ? degree+1 : degree;
    }
    return degree;
}
bool is_independent(struct graph_t *g){
    for(int i=0; i<g->cardinality; i++){
        for(int j=0; j<i; j++){
            if(get_edge(g, g->IDS[i], g->IDS[j]))   return false;
        }
        for(int j=i+1; j<g->cardinality; j++){
            if(get_edge(g, g->IDS[i], g->IDS[j]))   return false;
        }
    }
    return true;
}
bool is_domination(struct graph_t *g){
    bool flag;
    for(int i=0; i<g->cardinality; i++){
        flag = false;
        for(int j=0; j<g->n; j++){
            if(get_edge(g, g->IDS[i], j)){
                flag = true;
                break;
            }
        }
        if(!flag)   return false;
    }
    return true;
}
// The graph must be n*k/2 edge.
bool check_graph(struct graph_t *g){
    int edge = 0;
    for(int i=0; i<g->n; i++){
        for(int j=i+1; j<g->n; j++){
            edge = get_edge(g, i, j) ? edge+1 : edge;
        }
    }
    return edge==(g->n * g->k)/2;
}
void plot_counts(float *counts){
    FILE *fp = popen("gnuplot -persistent", "w");
    if(!fp){
        perror("popen error\n");
        exit(-1);
    }

    int index = 0;
    
    char *set_command[] = {
        "set output 'counts.png' \n",
        "set xlabel \"Rewiring Probability\" \n",
        "set ylabel \"Avg # of moves per node\" \n",
        "set yrange [0:1] \n",
        "set title \"Avg. #moves per node\" \n",
        "set style line 1 \
            linecolor rgb '#0060ad' \
            linetype 1 linewidth 2 \
            pointtype 7 pointsize 1.5 \n"
    };
    int command_size = 6;
    for(int i=0; i<command_size; i++){
        fprintf(fp, "%s", set_command[i]);
        fflush(fp);
    }
    fprintf(fp, "plot '-' with linespoints linestyle 1 title \"Symm MDS\" \n");
    fflush(fp);
    for(float p=0.0; p<0.81; p+=0.2, index++){
        fprintf(fp, "%.2f %.2f \n", p, counts[index]);
    }
    fprintf(fp, "e \n");
    pclose(fp);
}
void plot_cardinalitys(float *cardinalitys){
    FILE *fp = popen("gnuplot -persistent", "w");
    if(!fp){
        perror("popen error\n");
        exit(-1);
    }

    int index = 0;
    char *set_command[] = {
        "set output 'cardinalitys.png' \n",
        "set xlabel \"Rewiring Probability\" \n",
        "set ylabel \"Cardinality of IDS\" \n",
        "set yrange [0:10] \n",
        "set title \"Avg. #moves per node\" \n",
        "set style line 1 \
            linecolor rgb '#dd181f' \
            linetype 1 linewidth 2 \
            pointtype 7 pointsize 1.5 \n"
    };
    int command_size = 6;
    for(int i=0; i<command_size; i++){
        fprintf(fp, "%s", set_command[i]);
        fflush(fp);
    }
    fprintf(fp, "plot '-' with linespoints linestyle 1 title \"Symm MDS\" \n");
    fflush(fp);
    for(float p=0.0; p<0.81; p+=0.2, index++){
        fprintf(fp, "%.2f %.2f \n", p, cardinalitys[index]);
    }
    fprintf(fp, "e \n");
    pclose(fp);
}