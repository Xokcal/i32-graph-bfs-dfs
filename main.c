//
// Created by 26432 on 2026/5/16.
//
#include <stdio.h>
#include "graph/typedef.h"
#include "graph/graph.h"

#define WITHD 10
#define HEIGHT 10

int main(){
    i32 map[WITHD][HEIGHT] = {
            {0, 0, 0, 0, 0, 0, 1, 1, 1, 0},
            {0, 0, 1, 0, 1, 0, 0, 0, 1, 0},
            {0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
            {0, 0, 1, 0, 0, 0, 1, 1, 0, 1},
            {0, 0, 0, 0, 1, 0, 1, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
            {0, 1, 0, 1, 0, 0, 0, 0, 0, 0},
            {0, 1, 0, 1, 0, 0, 1, 1, 1, 1},
            {0, 1, 0, 1, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 1, 0, 1, 0}
    };
    Graph *myG = createGraph(9);
    graph_add_neig(myG , 0 , 2);
    graph_add_neig(myG , 0 , 3);
    graph_add_neig(myG , 0 , 1);
    graph_add_neig(myG , 1 , 4);
    graph_add_neig(myG , 2 , 4);
    graph_add_neig(myG , 3 , 7);
    graph_add_neig(myG , 7 , 6);
    graph_add_neig(myG , 6 , 8);

    print_graph(myG);

    bfs(myG , 0 , 8);

    printf("\n");
    for (int i = 0; i < WITHD; ++i) {
        printf("[");
        for (int j = 0; j < HEIGHT; ++j) {
            if (i == WITHD - 1 && j == HEIGHT - 1){
                printf("%d" , map[i][j]);
            }
            else if (j == HEIGHT - 1){
                printf("%d]\n" , map[i][j]);
            } else {
                printf("%d," , map[i][j]);
            }
        }
    }
    printf("]\n");

//    bfs_map(map , 0 , 0 , 6 , 9);

//    dfs(myG , 0 , 8);

    //                         x   y
    dfs_map_path(map , 0 , 0 , 9 , 3);
    return 0;
}

/*
[0,0,0,0,0,0,1,1,1,0]
[0,0,1,0,1,0,0,0,1,0]
[0,0,1,0,0,0,0,0,0,1]
[0,0,1,0,0,0,1,1,0,1]
[0,0,0,0,1,0,1,0,0,0]
[0,0,0,0,0,0,0,1,0,0]
[0,1,0,1,0,0,0,0,0,0]
[0,1,0,1,0,0,1,1,1,1]
[0,1,0,1,0,0,0,0,1,0]
[0,0,0,0,0,0,1,0,1,0]
 */