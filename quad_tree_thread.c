#include "quad_tree_thread.h"

void create_qtree_args(
    Quad_tree_args *qtree_params,
    bool *run_simulation,
    double theta,
    double dt,
    int thread_id,
    int nb_threads,
    int **i_to_remove,
    int *nb_to_remove,
    int *remove_index,
    Galaxy *g,
    Quad_tree *q_tree,
    struct gfx_context_t *context,
    pthread_barrier_t *barrier,
    pthread_mutex_t *resize_mutex)
{
    qtree_params->run_simulation = run_simulation;
    qtree_params->theta = theta;
    qtree_params->dt = dt;
    qtree_params->thread_id = thread_id;
    qtree_params->nb_threads = nb_threads;
    qtree_params->i_to_remove = i_to_remove;
    qtree_params->nb_to_remove = nb_to_remove;
    qtree_params->remove_index = remove_index;
    qtree_params->g = g;
    qtree_params->q_tree = q_tree;
    qtree_params->context = context;
    qtree_params->barrier = barrier;
    qtree_params->resize_mutex = resize_mutex;
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
    int thread_id = qtree_params->thread_id;
    int nb_threads = qtree_params->nb_threads;
    int step = ceil((double)qtree_params->g->num_bodies / (double)nb_threads);
    int from = thread_id * step;
    int to;
    if (thread_id == nb_threads - 1)
    {
        to = qtree_params->g->num_bodies;
    }
    else
    {
        to = from + step;
    }
    printf("qtree thread id:%d running !\n", thread_id);
    while (*qtree_params->run_simulation)
    {
        reset_accelerations(qtree_params->g, from, to);
        // Wait on all threads to be ready
        pthread_barrier_wait(qtree_params->barrier);

        update_accelerations_of_all_stars(qtree_params->q_tree->root, qtree_params->g, from, to, qtree_params->theta);
        update_positions(qtree_params->g, from, to, qtree_params->dt);
        clean_i_to_remove(from, to, qtree_params->nb_to_remove, *qtree_params->i_to_remove, qtree_params->remove_index, qtree_params->resize_mutex);
        // Wait on all threads to stop update and clean
        pthread_barrier_wait(qtree_params->barrier);

        int local_nb_to_remove = set_i_to_remove(qtree_params->g, from, to, qtree_params->nb_to_remove, *qtree_params->i_to_remove, qtree_params->resize_mutex);
        // Wait on all threads to stop setting stars not in box
        pthread_barrier_wait(qtree_params->barrier);
        copy_without(qtree_params->g, *qtree_params->i_to_remove, *qtree_params->nb_to_remove, qtree_params->remove_index, local_nb_to_remove, from, to, nb_threads, thread_id, qtree_params->barrier, qtree_params->resize_mutex);
        // Wait while threads 0 resize galaxy
        pthread_barrier_wait(qtree_params->barrier);

        step = ceil((double)qtree_params->g->num_bodies / (double)nb_threads);
        from = thread_id * step;
        if (thread_id == nb_threads - 1)
        {
            to = qtree_params->g->num_bodies;
        }
        else
        {
            to = from + step;
        }

        show_pixels(qtree_params->context, qtree_params->g, from, to);
        // Wait on all threads to stop modify context
        pthread_barrier_wait(qtree_params->barrier);
    }
    printf("qtree thread id:%d exit !\n", thread_id);
    return NULL;
}

void show_pixels(struct gfx_context_t *context, Galaxy *g, int from, int to)
{
    for (int i = from; i < to; i++)
    {
        int x = (int)(g->stars[i].pos_t.x * (context->width / (R_INIT * 2)) + context->width / 2);
        int y = (int)(g->stars[i].pos_t.y * (context->height / (R_INIT * 2)) + context->height / 2);
        // printf("==== Writing to x : %d --- y : %d ====\n", x, y);
        gfx_putpixel(
            context,
            x,
            y,
            g->stars[i].color);
    }
}