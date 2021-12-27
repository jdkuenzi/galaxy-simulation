#ifndef _QUAD_TREE_THREAD_H_
#define _QUAD_TREE_THREAD_H_

#include "./libs/quadtree/quad_tree.h"
#include <pthread.h>

typedef struct __quad_tree_args {
    bool *run_simulation;
    double theta;
    double dt;
    Galaxy *g;
    Quad_tree *tmp_qtree;
    pthread_barrier_t *barrier_quadtree_main;
    pthread_mutex_t *g_mutex;
} Quad_tree_args;

Quad_tree_args *create_qtree_args(bool *, double, double, Galaxy *, pthread_barrier_t*, pthread_mutex_t*);

void run_qtree_thread(pthread_t *, Quad_tree_args *qtree_args);

void *qtree_thread(void *args);

#endif