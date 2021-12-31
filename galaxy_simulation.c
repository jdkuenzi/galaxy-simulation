/*
 * Author   : Küenzi Jean-Daniel
 * Version  : 1.0.0
 * Date     : 30.05.2019
 * Update   : 08.06.2019
 * Desc.    : Le main du programme galaxy_simulation qui va gérer les galaxies et l'affichage
 */

#include "galaxy_simulation.h"

int main(int argc, char **argv)
{
    int n_bodies;
    double theta;
    int nb_threads;

    if (argc > 3)
    {
        nb_threads = atoi(argv[1]);
        n_bodies = atoi(argv[2]);
        theta = atof(argv[3]);
        if (nb_threads <= 0 || n_bodies <= 0 || theta <= 0)
        {
            printf("nb_threads, num_stars et/ou theta doivent être plus grand que 0\n");
            return EXIT_FAILURE;
        }
    }
    else
    {
        printf("galaxy_simulation <nb_threads> <num_stars> <theta>\n");
        printf("    - <nb_threads> est un entier représentant le nombre de threads effecutant les calculs et l'affichage\n");
        printf("    - <num_stars> est un entier représentant le nombre d'étoiles dans la galaxie\n");
        printf("    - <theta> est un double représentant la distance minimale entre les noeuds et les feuilles du quad_tree.\n");
        printf("Exemple : ./galaxy_simulation 4 10000 10\n");
        return EXIT_FAILURE;
    }

    Box b = new_box(-R_INIT, R_INIT, -R_INIT, R_INIT);
    Galaxy *g = (Galaxy *)malloc(sizeof(Galaxy));
    create_and_init_galaxy(g, n_bodies, b, DT);
    Quad_tree *q_tree = (Quad_tree *)malloc(sizeof(Quad_tree));

    int *i_to_remove = (int *)malloc(n_bodies * sizeof(int));
    int nb_to_remove = 0;
    int remove_index = 0;

    struct gfx_context_t *context = create_gfx();

    bool show_quad_tree = false;
    bool show_super_s = false;
    bool run_simulation = true;

    pthread_mutex_t resize_mutex;
    pthread_mutex_init(&resize_mutex, NULL);

    pthread_t qtree_threads[nb_threads];
    Quad_tree_args qtree_params[nb_threads];
    pthread_barrier_t barrier;
    pthread_barrier_init(&barrier, NULL, nb_threads);
    for (int i = 0; i < nb_threads - 1; i++)
    {
        int thread_id = i + 1;
        create_qtree_args(
            &qtree_params[i],
            &run_simulation,
            theta,
            DT,
            thread_id,
            nb_threads,
            &i_to_remove,
            &nb_to_remove,
            &remove_index,
            g,
            q_tree,
            context,
            &barrier,
            &resize_mutex);
        if (pthread_create(&qtree_threads[i], NULL, qtree_thread, (void *)&qtree_params[i]) != 0)
        {
            perror("Thread creation error."); // affiche le message sur le canal d'erreur
            return EXIT_FAILURE;
        }
    }
    // Quad_tree_args *qtree_params = create_qtree_args(
    //     &run_simulation,
    //     theta,
    //     DT,
    //     g,
    //     &barrier_quadtree_main,
    //     &g_mutex);

    // run_qtree_thread(&qtree_thread, qtree_params);

    SDL_ShowCursor(SDL_ENABLE);  // needed to se the cursor
    SDL_Keycode key_pressed = 0; // escape key needed
    SDL_Keycode old_key_pressed = key_pressed;

    int thread_id = 0;
    int step = ceil((double)n_bodies / (double)nb_threads);
    int from = thread_id * step;
    int to = from + step;
    while (run_simulation)
    {
        gfx_clear(context, COLOR_BLACK);
        SDL_PumpEvents();
        key_pressed = gfx_keypressed();
        if (key_pressed != old_key_pressed)
        {
            if (key_pressed == SDLK_ESCAPE)
            {
                run_simulation = false;
            }
            else if (key_pressed == SDLK_1)
            {
                show_quad_tree = !show_quad_tree;
            }
            else if (key_pressed == SDLK_2)
            {
                show_super_s = !show_super_s;
            }
            // else if (key_pressed == SDLK_r)
            // {
            //     free_galaxy(g);
            //     create_and_init_galaxy(g, n_bodies, b, DT);
            // }
            old_key_pressed = key_pressed;
        }
        reset_accelerations(g, from, to);
        create_quad_tree_from_galaxy(q_tree, g);
        // Wait on all threads to be ready
        pthread_barrier_wait(&barrier);

        update_accelerations_of_all_stars(q_tree->root, g, from, to, theta);
        update_positions(g, from, to, DT);
        clean_i_to_remove(from, to, &nb_to_remove, i_to_remove, &remove_index, &resize_mutex);
        // Wait on all threads to stop update and clean
        pthread_barrier_wait(&barrier);

        int local_nb_to_remove = set_i_to_remove(g, from, to, &nb_to_remove, i_to_remove, &resize_mutex);
        // Wait on all threads to stop setting stars not in box
        pthread_barrier_wait(&barrier);

        copy_without(g, i_to_remove, nb_to_remove, &remove_index, local_nb_to_remove, from, to, nb_threads, thread_id, &barrier, &resize_mutex);
        i_to_remove = (int *)realloc((void *)i_to_remove, g->num_bodies * sizeof(int));
        // Wait while threads 0 resize galaxy
        pthread_barrier_wait(&barrier);
        // run_simulation = false;
        step = ceil((double)g->num_bodies / (double)nb_threads);
        from = thread_id * step;
        to = from + step;
        show_pixels(context, g, from, to);
        // Wait on all threads to stop modify context
        pthread_barrier_wait(&barrier);

        if (show_quad_tree || show_super_s)
        {
            draw_quad_tree(context, q_tree->root, show_quad_tree, show_super_s);
        }
        free_node(q_tree->root);

        gfx_present(context);
        // printf("asdasdasd\n");
    }

    for (int i = 0; i < nb_threads - 1; i++)
    {
        if (pthread_join(qtree_threads[i], NULL) != 0)
        { // attente que le thread se termine
            perror("Thread join error");
            return EXIT_FAILURE;
        }
    }

    pthread_barrier_destroy(&barrier);
    pthread_mutex_destroy(&resize_mutex);

    gfx_destroy(context);
    free(i_to_remove);
    free(q_tree);
    free_galaxy(g);
    free(g);

    return EXIT_SUCCESS;
}

void draw_quad_tree(struct gfx_context_t *context, Node *n, bool show_quad_tree, bool show_super_s)
{
    if (show_quad_tree)
    {
        draw_box(context, n->b);
    }
    if (!is_leaf(n))
    {
        if (show_super_s)
        {
            draw_super_s(context, n->super_s);
        }
        for (int i = 0; i < 4; i++)
        {
            draw_quad_tree(context, n->children[i], show_quad_tree, show_super_s);
        }
    }
}

void draw_super_s(struct gfx_context_t *context, Star *super_s)
{
    int x = (int)(super_s->pos_t.x * (context->width / (R_INIT * 2)) + context->width / 2);
    int y = (int)(super_s->pos_t.y * (context->height / (R_INIT * 2)) + context->height / 2);
    uint32_t color = super_s->color;
    if (x > 0)
    {
        gfx_putpixel(context, x - 1, y, color);
        if (y > 0)
        {
            gfx_putpixel(context, x - 1, y - 1, color);
        }
        if (y < NY)
        {
            gfx_putpixel(context, x - 1, y + 1, color);
        }
    }

    if (x < NX)
    {
        gfx_putpixel(context, x + 1, y, color);
        if (y > 0)
        {
            gfx_putpixel(context, x + 1, y - 1, color);
        }
        if (y < NY)
        {
            gfx_putpixel(context, x + 1, y + 1, color);
        }
    }

    if (y > 0)
    {
        gfx_putpixel(context, x, y - 1, color);
    }
    if (y < NY)
    {
        gfx_putpixel(context, x, y + 1, color);
    }
    gfx_putpixel(context, x, y, color);
}

void draw_box(struct gfx_context_t *context, Box b)
{
    int x0 = (int)(b.x0 * (context->width / (R_INIT * 2)) + context->width / 2);
    int y0 = (int)(b.y0 * (context->height / (R_INIT * 2)) + context->height / 2);
    int x1 = (int)(b.x1 * (context->width / (R_INIT * 2)) + context->width / 2);
    int y1 = (int)(b.y1 * (context->height / (R_INIT * 2)) + context->height / 2);
    draw_square(context, x0, x1, y0, y1, COLOR_GREEN);
}

void draw_square(struct gfx_context_t *context, int x0, int x1, int y0, int y1, uint32_t color)
{
    for (int i = x0; i < x1; i++)
    {
        gfx_putpixel(
            context,
            i,
            y0,
            color);
        gfx_putpixel(
            context,
            i,
            y1,
            color);
    }

    for (int i = y0; i < y1; i++)
    {
        gfx_putpixel(
            context,
            x0,
            i,
            color);
        gfx_putpixel(
            context,
            x1,
            i,
            color);
    }
}

struct gfx_context_t *create_gfx()
{
    struct gfx_context_t *context = gfx_create("Barnes Hut Galaxy Simulation", NX + 1, NY + 1);
    if (!context)
    {
        fprintf(stderr, "Graphic mode initialization failed!\n");
        EXIT_FAILURE;
    }
    return context;
}