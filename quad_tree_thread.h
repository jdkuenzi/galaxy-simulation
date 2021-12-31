#ifndef _QUAD_TREE_THREAD_H_
#define _QUAD_TREE_THREAD_H_

#include "./libs/quadtree/quad_tree.h"
#include "./libs/gfx/gfx.h"
#include <pthread.h>

typedef struct __quad_tree_args
{
    bool *run_simulation;
    double theta;
    double dt;
    int thread_id;
    int nb_threads;
    int **i_to_remove;
    int *nb_to_remove;
    int *remove_index;
    Galaxy *g;
    Quad_tree *q_tree;
    struct gfx_context_t *context;
    pthread_barrier_t *barrier;
    pthread_mutex_t *resize_mutex;
} Quad_tree_args;

void create_qtree_args(Quad_tree_args *, bool *, double, double, int, int, int **, int *, int *, Galaxy *, Quad_tree *, struct gfx_context_t *, pthread_barrier_t *, pthread_mutex_t *);

void run_qtree_thread(pthread_t *, Quad_tree_args *qtree_args);

void *qtree_thread(void *args);

void show_pixels(struct gfx_context_t *, Galaxy *, int, int);

void update_galaxy();

void draw();

#endif