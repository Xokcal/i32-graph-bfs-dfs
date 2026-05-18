//
// Created by 26432 on 2026/5/16.
//

#ifndef 手搓图_GRAPH_H
#define 手搓图_GRAPH_H

#include <stdio.h>
#include "typedef.h"
#include <stdlib.h>

//相邻邻居
typedef struct NeigNode {
    i32 neig;
    struct NeigNode *nextNeig;
} NeigNode;

//图表
/* NeigNode** = arr :
 * arr 指的是 arr[0]（头索引），而数组的每个索引就是一个地址，
 * 然后这个地址的值又存了一个NeigNode。所以用**NeigNode接收；
 * 如果用一级指针去值arr[0]那么解指针，只能拿到索引地址存的值（NeigNode地址值），
 * 是二级指针的话，双解（**）就可以再解NeigNode地址值 ，就拿到了NeigNode里面的所有属性*/
typedef struct Graph {
    i32                     topSum;
    NeigNode                **NeigArr;
} Graph;

/* 定义一个队列 :
 * *arr是队列的数组，每进一个元素都会存到这个数组里面
 * head 指向这个数组第一个元素（第一个未被列出的元素）
 * tail 指向这个队列最后一个元素
 * capacity 这个队列（数组）一共能储存多少元素*/
typedef struct Queue {
    i32                     *arr;
    i32                     head;
    i32                     tail;
    i32                     capacity;
} Queue;

Graph *createGraph(i32 topSumSet);
Graph *graph_add_neig(Graph *graph, i32 host, i32 be_neig);
void print_graph(Graph *graph);
void bfs(Graph *graph, i32 start, i32 t);
void dfs(Graph *graph, i32 start, i32 t);

#define WITHD 10
#define HEIGHT 10

void bfs_map(i32 arr[HEIGHT][WITHD], i32 start_y, i32 start_x, i32 t_y, i32 t_x);

#endif //手搓图_GRAPH_H
