#include "quad_tree_thread.h"

Quad_tree_args *create_qtree_args(
    bool *run_simulation,
    double theta,
    double dt,
    Galaxy *g,
    pthread_barrier_t *barrier_quadtree_main,
    pthread_mutex_t *g_mutex
)
{
    Quad_tree_args *qtree_params = (Quad_tree_args *)malloc(sizeof(Quad_tree_args));
    qtree_params->run_simulation = run_simulation;
    qtree_params->theta = theta;
    qtree_params->dt = dt;
    qtree_params->g = g;
    qtree_params->barrier_quadtree_main = barrier_quadtree_main;
    qtree_params->g_mutex = g_mutex;
    return qtree_params;
}

void run_qtree_thread(pthread_t *thread, Quad_tree_args *qtree_args)
{
    if (pthread_create(thread, NULL, qtree_thread, (void *)qtree_args) != 0)
    {
        perror("Render thread creation error."); // affiche le message sur le canal d'erreur
        exit(0);
    }
}

void *qtree_thread(void *args)
{
    Quad_tree_args *qtree_params = (Quad_tree_args *)args;
    while (*qtree_params->run_simulation)
    {
        pthread_mutex_lock(qtree_params->g_mutex);
        reset_accelerations(qtree_params->g);
        Quad_tree *q_tree = create_quad_tree_from_galaxy(qtree_params->g);
        update_accelerations_of_all_stars(q_tree->root, qtree_params->g, qtree_params->theta);
        // simple_update_acc_of_all_stars(g);
        update_positions(qtree_params->g, qtree_params->dt);
        resize_galaxy(qtree_params->g);
        pthread_mutex_unlock(qtree_params->g_mutex);
        qtree_params->tmp_qtree = q_tree;
        pthread_barrier_wait(qtree_params->barrier_quadtree_main);
        pthread_barrier_wait(qtree_params->barrier_quadtree_main);
        free_quad_tree(q_tree);
    }
    printf("qtree thread exit !\n");
    return NULL;
}