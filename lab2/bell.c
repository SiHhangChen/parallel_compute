#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <limits.h>

#define INF INT_MAX

struct edeg{
    int s, d, w;
};

struct graph{
    int V, E;
    struct edeg* edge;
};

int main(){
    omp_set_num_threads(4);
    int vnum, eNum, src;
    scanf("%d %d %d", &vnum, &eNum, &src);
    struct graph* g = (struct graph*)malloc(sizeof(struct graph));
    g->V = vnum;
    g->E = eNum;
    g->edge = (struct edeg*)malloc(g->E * sizeof(struct edeg));
    for(int i = 0; i < eNum; i++){
        int s, d, w;
        scanf("%d %d %d", &s, &d, &w);
        g->edge[i].s = s;
        g->edge[i].d = d;
        g->edge[i].w = w;
    }

    int* dist = (int*)malloc(vnum * sizeof(int));
    #pragma omp parallel for
    for(int i = 0; i < vnum; i++){
        dist[i] = INF;
    }
    dist[src] = 0;

    for(int i = 0; i < vnum - 1; i++){
        int change = 0;
        #pragma omp parallel for reduction(||:change)
        for(int j = 0; j < eNum; j++){
            int u = g->edge[j].s;
            int v = g->edge[j].d;
            int w = g->edge[j].w;
            #pragma omp critical
            if(dist[v] > dist[u] + w && dist[u] != INF){
                dist[v] = dist[u] + w;
                change = 1;
            }
            else if (dist[u] > dist[v] + w && dist[v] != INF){
                dist[u] = dist[v] + w;
                change = 1;
            }
        }
        if(change == 0) break;
    }

    for(int i = 0; i < vnum; i++){
        if(dist[i] == INF) printf("INF ");
        else printf("%d ", dist[i]);
    }
    printf("\n");

    free(g->edge);
    free(g);
    free(dist);
    return 0;
}