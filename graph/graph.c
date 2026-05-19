#include <stdio.h>
#include "typedef.h"
#include <stdlib.h>
#include "graph.h"
#include <math.h>

/* 这里是创建一个Graph，这个图可以设置有多少个“朋友”（数组长度） */
Graph *createGraph(i32 topSumSet) {
    Graph *graph = (Graph *) malloc(sizeof(Graph));
    graph->topSum = topSumSet;
    /*graph->NeigArr 是NeigNode**类型，也就是存储*NeigNode数组的头索引，
     * 让graph->NeigArr = 数组头索引，就拿到了这个数组所有数据*/
    graph->NeigArr = (NeigNode **) malloc(topSumSet * sizeof(NeigNode *));
    /* 默认每个索引先为NULL */
    for (int i = 0; i < topSumSet; ++i) {
        graph->NeigArr[i] = NULL;
    }
    return graph;
}

/* 添加邻居：比如我要让0朋友（0索引）和2朋友（2索引）互相成为邻居，
 * 那么我就要新建一个节点（NeigNode），然后让这个节点的邻居等于2，
 * 再将这个节点用哈希表的头插法一样插在0朋友那里（相当于写了一个便条，
 * 上面记录了0朋友的邻居->2，），然后因为2被1记录了，那么2也要写一个
 * 便条（NeigNode）上面记录邻居为1，然后头插法，插入索引2。
 *
 *          +---------------------------------------------------+
 *          |   0   |     1    |      2    |     3    |     4   |
 *          +---------------------------------------------------+
 *            Node1                 Node1                 Node1
 *           |     \               |     \               |     \
 *         Node2    2             NULL    1            NULL     1
 *        |     4
 *       NULL
 * */
Graph *graph_add_neig(Graph *graph, i32 host, i32 be_neig) {
    /* host是主动加be_neig的朋友（索引），be_neig是被添加的（他们其实是相互关系） */
    NeigNode *host_node = (NeigNode *) malloc(sizeof(NeigNode)); //开辟一个新节点
    /* 记录这个节点的邻居是be_neig */
    host_node->neig = be_neig;
    /* 头插法插入图的数组host索引 */
    host_node->nextNeig = graph->NeigArr[host];
    graph->NeigArr[host] = host_node;

    /* 此时host已经插入完毕，接下来需要让be_neig去记录host */
    NeigNode *be_neig_node = (NeigNode *) malloc(sizeof(NeigNode)); //同样新建一个便条
    be_neig_node->neig = host; //记录host
    /* 头插法，插入be_neig索引 */
    be_neig_node->nextNeig = graph->NeigArr[be_neig];
    graph->NeigArr[be_neig] = be_neig_node;
    return graph;
}

/* 打印图：通过图数组的每个索引去遍历，然后每遍历到一个索引，
 * 都打印这个索引里面的每个节点的邻居。
 * 例如：
 *      Graph top point: 0 :1 -> 3 -> 2 -> NULL
 *      Graph top point: 1 :4 -> 0 -> NULL
 *      Graph top point: 2 :0 -> NULL
 *      Graph top point: 3 :0 -> NULL
 *      Graph top point: 4 :1 -> NULL
 * */
void print_graph(Graph *graph) {
    if (graph == NULL)return;
    for (int i = 0; i < graph->topSum; ++i) {
        NeigNode *curr = graph->NeigArr[i];
        printf("Graph top point: %d :", i);
        while (curr != NULL) {
            printf("%d -> ", curr->neig);
            curr = curr->nextNeig;
        }
        printf("NULL\n");
    }
}

/* BFS */
/* 创建一个队列，并且初始化 */
Queue *create_queue(i32 capacity) {
    Queue *queue = (Queue *) malloc(sizeof(Queue));
    queue->arr = (i32 *) malloc(capacity * sizeof(i32));
    queue->head = 0;
    queue->tail = 0;
    queue->capacity = capacity;
    return queue;
}

/* 队列的进列
 * 重点：arr[queue->tail++]
 * tail++指的是先拿到这个tail的值，然后再++。
 * 例如：
 *     假设此数组长度未5（capacity）
 *        0          1          2          3          4
 *    +---------------------------------------------------+
 *    |   tail     |          |          |         |      |   <--- new = a
 *    +---------------------------------------------------+
 *        |
 *     tail=0
 *
 * 执行 queue->arr[queue->tail++] = new :
 *        0          1          2          3          4
 *    +---------------------------------------------------+
 *    |   a     |   tail   |         |           |        |
 *    +---------------------------------------------------+
 *                   |
 *                tail=1
 * 或者也可以这样写：
 *    static void
 *    enqueue(Queue* queue , i32 new){
 *       queue->arr[queue->tail] = new;
 *       queue->tail++;
 *    }
 * */
static void enqueue(Queue *queue, i32 new) {
    queue->arr[queue->tail++] = new;
}

/* 依次拿到队列元素
 * 同样！通过head++去将head指向的元素拿（返回）出去之后舍弃。
 * 例如：
 *     假设此数组长度未5（capacity）
 *        0          1          2          3          4
 *    +---------------------------------------------------+
 *    |   a     |     b     |      c    |    d     |      |
 *    +---------------------------------------------------+
 *        |                                  |
 *     head=0                             tail=1
 *
 * 执行 queue->arr[queue->tail++] = new :
 *                0          1          2          3          4
 *            +---------------------------------------------------+
 *   <--- a   |  a（无用）|     b     |     c     |    d     |      |
 *            +---------------------------------------------------+
 *                            |                       |
 *                           |                    tail=1
 *                          |
 *                       head=1
 * */
static i32 dequeue(Queue *queue) {
    return queue->arr[queue->head++];
}

/*head和tail相等的话，就说明head已经指向了最后一个元素，
 * 并且tail后面也没有新的元素了。
 * 例如：
 *                0          1          2          3          4
 *            +---------------------------------------------------+
 *   <--- d   |  a（无用）|     b     |     c     |    d     |      |
 *            +---------------------------------------------------+
 *                                                   |
 *                                                tail=1
 *                                                  |
 *                                               head=1
 * */
static i32 is_queue_empty(Queue *queue) {
    return queue->head == queue->tail ? 1 : 0;
}

/* 栈处理：
 * 入栈 ：
 *                  0          1          2          3          4
 *            +---------------------------------------------------+
 *   <--- a   |  a（无用）|     b     |     c     |    d     |      |
 *            +---------------------------------------------------+
 *                                                   |        |
 *                                                 tail     tail++
 * */
static void stack_push(Queue *queue, i32 new) {
    queue->arr[queue->tail++] = new;
}

/* 出栈：
 *                  0          1          2          3          4
 *            +---------------------------------------------------+
 *   <--- a   |  a（原点）|     b     |     c     |    d     |      |
 *            +---------------------------------------------------+
 *                |
 *              tail
 *
 *                  0          1          2          3          4
 *            +---------------------------------------------------+
 *   <--- a   |  a（无用）|     b     |     c     |    d     |      |
 *            +---------------------------------------------------+
 *                                        |         |  |
 *                                      tail--    tail `---------------->  d
 *
 * */
static i32 stack_pop(Queue *queue){
    if (queue->tail == 0)return -1;
    return queue->arr[--queue->tail];
}

/* BFS:深度搜索
 * 核心：普通的搜索，是遍历所有索引找到目标，而 BFS：就是先看0索引，
 * 如果有就退出，如果没有就把0的所有邻居找出来，然后放进队列，
 * 然后再依次看这些邻居的邻居*/
void bfs(Graph *graph, i32 start, i32 t) {
    /* 这里定义一个布尔类型的数组，用于记录graph里面的每个元素是否已经被查询过，
     * 例如：
     *        0          1          2          3          4
     *    +---------------------------------------------------+
     *    |   0     |     1     |      0    |    1     |   0  |
     *    +---------------------------------------------------+
     *                                |
     *                              bol[2]
     * 这里0代表false，1代表true，用于记录graph的元素哪些被查找了，
     * 哪些没有被查找。
     * */
    i32 *bol = (i32 *) calloc(graph->topSum, sizeof(i32));
    /* 创建一个队列，用于查找所有的邻居 */
    Queue *q = create_queue(graph->topSum);
    /* 由于从start开始查找，所以start先被登记为已查找 */
    bol[start] = 1;
    /* start进队列 */
    enqueue(q, start);
    printf("\nBFS from %d start\n", start);
    printf("BFS target : %d\n", t);
    /* 队列不为空，就一直循环 */
    while (!is_queue_empty(q)) {
        /* 拿到队列的元素 */
        i32 curr = dequeue(q);
        if (curr == t) {
            printf("[BFS RESULT] : %d\n", curr);
            free(bol);
            free(q->arr);
            free(q);
            return;
        }
        printf("curr element is : %d\n", curr);
        /* 通过当前元素拿到graph对应的索引,
         * 这里拿到了存到队列里面的第一个元素的对应graph图里的节点*/
        NeigNode *node = graph->NeigArr[curr];
        /* 节点不能为NULL */
        while (node != NULL) {
            /* 核心重点：要判断bol里面此节点有没有被查询过，
             * 如果被查询过（=0）那么就退出*/
            if (bol[node->neig] == 0) {
                bol[node->neig] = 1;
                enqueue(q, node->neig);
            }
            node = node->nextNeig;
        }
    }
    printf("[BFS RESULT] : NOT FIND!!\n");
    free(bol);
    free(q->arr);
    free(q);
    return;
}

static void
dfs_method(Graph *graph, i32 start, i32 t, i32 *bol) {
    bol[start] = 1;
    NeigNode *node = graph->NeigArr[start];
    while (node != NULL) {
        if (start == t) {
            printf("[DFS RESULT] : %d\n", node->neig);
            return;
        }
        if (bol[node->neig] == 0) {
            bol[node->neig] = 1;
            //就是每进一个新的递归都是一个新的节点，直接把它当可能的目标处理就行了
            dfs_method(graph, node->neig, t, bol);
        }
        node = node->nextNeig;
    }
}

void dfs(Graph *graph, i32 start, i32 t) {
    i32 *bol = (i32 *) calloc(graph->topSum, sizeof(i32));
    dfs_method(graph, start, t, bol);
    free(bol);
}

#define WITHD 10
#define HEIGHT 10

void bfs_map(i32 arr[HEIGHT][WITHD], i32 start_y, i32 start_x, i32 t_y, i32 t_x) {
    i32 bol[HEIGHT][WITHD] = {0};
    i32 nx[] = {-1, 1, 0, 0};
    i32 ny[] = {0, 0, -1, 1};
    Queue *q = create_queue((WITHD * HEIGHT));
    bol[start_y][start_x] = 1;
    enqueue(q, (start_y * WITHD) + start_x);
    while (!is_queue_empty(q)) {
        i32 curr = dequeue(q);     //注意这里的举例12是在bol数组里面的位置，实际上指的是数字 “13”！！！
        i32 width = curr % WITHD;  // 12 % 10 = 2
        i32 height = floor(curr / 10);  // 12 / 10 = 1
        printf("current place is [x=%d,y=%d] v = %d", width, height, arr[height][width]);
        if (height == t_y && width == t_x) {
            printf("\n[BFS MAP RESULT] : x = %d , y = %d , v=%d\n", width, height, arr[height][width]);
            return;
        }
        for (int i = 0; i < 4; ++i) {
            i32 new_x = width + nx[i];
            i32 new_y = height + ny[i];
            if (arr[new_y][new_x] != 1 && bol[new_y][new_x] == 0 &&
                new_x < WITHD && new_y < HEIGHT && //不能超过最大高宽度
                new_x >= 0 && new_y >= 0 //不能小于0
                    ) { //不能等于1（墙），bol没记录过
                bol[new_y][new_x] = 1;
                enqueue(q, (new_y * WITHD) + new_x);
            }
        }
    }
    free(q->arr);
    free(q);
    printf("\n[BFS MAP RESULT] : NOT FIND RESULT !!!\n");
    return;
}

static i32 is_find = 0;

static void
dfs_map_method(i32 map[HEIGHT][WITHD], i32 *bol, i32 start_x, i32 start_y, i32 t_y, i32 t_x) {
    printf("[DFS MAP] current place is [%d][%d]\n", start_x, start_y);
    i32 n_y[] = {-1, 1, 0, 0};
    i32 n_x[] = {0, 0, -1, 1};
    bol[start_y * WITHD + start_x] = 1;
    //判断新节点是否是目标节点
    if (start_x == t_y && start_y == t_x) {
        printf("\n[DFS MAP RESULT] result is [%d][%d]\n", start_x, start_y);
        is_find = 1;
        return;
    }

    for (int i = 0; i < 4; ++i) {
        //拿到新节点的坐标（有四个）
        i32 new_height = start_y + n_y[i]; // [-1][0]
        i32 new_width = start_x + n_x[i]; // [0][0]
        if (new_width < WITHD && new_width >= 0 && new_height < HEIGHT && new_height >= 0 &&
            map[new_height][new_width] != 1 && bol[new_height * WITHD + new_width] == 0) {
            bol[new_height * WITHD + new_width] = 1;
            //每拿到一个都直接递归
//            printf("[DFS TEST] current place is [%d][%d]\n" , new_width , new_height);
            if (is_find == 1)return;
            dfs_map_method(map, bol, new_width, new_height, t_y, t_x);
        }
        //接下来他会自己退出，并且循环拿到新坐标..
    }
}

void dfs_map(i32 map[HEIGHT][WITHD], i32 start_x, i32 start_y, i32 t_y, i32 t_x) {
    i32 *bol = (i32 *) calloc((HEIGHT * WITHD), sizeof(i32)); // all is "0";
    dfs_map_method(map, bol, start_x, start_y, t_y, t_x);
    free(bol);
}

static void
dfs_map_method_test(i32 map[HEIGHT][WITHD], Queue *q, i32 *bol, i32 start_x, i32 start_y, i32 t_y, i32 t_x) {
    printf("[DFS MAP] current place is [%d][%d]\n", start_x, start_y);
    i32 n_y[] = {-1, 1, 0, 0};
    i32 n_x[] = {0, 0, -1, 1};
    bol[start_y * WITHD + start_x] = 1;
    //判断新节点是否是目标节点
    if (start_y == t_y && start_x == t_x) {
        printf("\n[DFS MAP RESULT] result is [%d][%d]\n", start_x, start_y);
        is_find = 1;
        record_map_path(map, q, start_x, start_y);
        return;
    }

    for (int i = 0; i < 4; ++i) {
        //拿到新节点的坐标（有四个）
        i32 new_height = start_y + n_y[i]; // [-1][0]
        i32 new_width = start_x + n_x[i]; // [0][0]
        if (new_width < WITHD && new_width >= 0 && new_height < HEIGHT && new_height >= 0 &&
            map[new_height][new_width] != 1 && bol[new_height * WITHD + new_width] == 0) {
            bol[new_height * WITHD + new_width] = 1;
            //每拿到一个都直接递归
//            printf("[DFS TEST] current place is [%d][%d]\n" , new_width , new_height);
            if (is_find == 1)return;
            stack_push(q, (new_height * WITHD + new_width));
            dfs_map_method_test(map, q, bol, new_width, new_height, t_y, t_x);
            stack_pop(q);
        }
        //接下来他会自己退出，并且循环拿到新坐标..
    }
}

void dfs_map_path
        (i32 map[HEIGHT][WITHD], i32 start_x, i32 start_y, i32 t_y, i32 t_x) {
    i32 *bol = (i32 *) calloc((HEIGHT * WITHD), sizeof(i32)); // all is "0";
    Queue *q = create_queue(HEIGHT * WITHD);
    dfs_map_method_test(map, q, bol, start_x, start_y, t_y, t_x);
    free(bol);
}

static void record_map_path(i32 map[HEIGHT][WITHD], Queue *q, i32 start_x, i32 start_y) {
    while (q->tail > 0) {
        i32 currI = stack_pop(q);
        i32 height = currI / HEIGHT;
        i32 width = currI % HEIGHT;
        map[height][width] = 5;
    }
    printf("\n");
    for (int i = 0; i < WITHD; ++i) {
        printf("[");
        for (int j = 0; j < HEIGHT; ++j) {
            if (start_y == i && start_x == j) {
                if (j == WITHD - 1 && i != HEIGHT - 1){
                    map[i][j] = 9;
                    printf("%d]\n", map[i][j]);
                } else if (j == WITHD - 1){
                    map[i][j] = 9;
                    printf("%d", map[i][j]);
                } else {
                    map[i][j] = 9;
                    printf("%d,", map[i][j]);
                }
            } else if (i == WITHD - 1 && j == HEIGHT - 1) {
                printf("%d", map[i][j]);
            } else if (j == HEIGHT - 1) {
                printf("%d]\n", map[i][j]);
            } else {
                printf("%d,", map[i][j]);
            }
        }
    }
    printf("]\n");
}

//(9 , 7)

//for (int i = 0; i < WITHD; ++i) {
//printf("[");
//for (int j = 0; j < HEIGHT; ++j) {
//if (start_y == i && start_x == j) {
//if (j == WITHD - 1 && i != HEIGHT - 1){
//map[i][j] = 9;
//printf("%d]\n", map[i][j]);
//} else if (i == HEIGHT - 1){
//map[i][j] = 9;
//printf("%d", map[i][j]);
//} else {
//map[i][j] = 9;
//printf("%d,", map[i][j]);
//}
//} else if (i == WITHD - 1 && j == HEIGHT - 1) {
//printf("%d", map[i][j]);
//} else if (j == HEIGHT - 1) {
//printf("%d]\n", map[i][j]);
//} else {
//printf("%d,", map[i][j]);
//}
//}
//}

