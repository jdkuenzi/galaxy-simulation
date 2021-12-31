/*
 * Author   : Küenzi Jean-Daniel
 * Version  : 1.0.0
 * Date     : 30.05.2019
 * Update   : 08.06.2019
 * Desc.    : Fichier contenant les fonctions permettant d'utiliser les galaxies (galaxy)
 */

#include "galaxy.h"

void create_and_init_galaxy(Galaxy *g, int num_bodies, Box b, const double dt)
{
    g->num_bodies = num_bodies;
    g->stars = (Star *)malloc(num_bodies * sizeof(Star));
    if (g->stars == NULL)
    {
        printf("Erreur lors de la creation du tableau d'etoile pour galaxie !");
        exit(0);
    }
    g->b = b;

    Vec *zero_vec = new_vec(0.0, 0.0);

    Star *central_star = new_star_vel(*zero_vec, *zero_vec, *zero_vec, M_CENTRAL, dt); // Etoile Initiale, au centre
    g->stars[0] = *central_star;
    free(central_star);

    srand(time(NULL)); // Initialise rand

    for (int i = 1; i < num_bodies; i++)
    {
        double m_star = get_random_mass(0, 10, M_SOLAIRE);
        Vec *pos_star = get_random_position(0, 1);
        Vec *vel_vec = get_velocite(m_star, M_CENTRAL, pos_star);
        Star *new_star = new_star_vel(*pos_star, *vel_vec, *zero_vec, m_star, dt);
        g->stars[i] = *new_star;
        free(pos_star);
        free(vel_vec);
        free(new_star);
    }

    free(zero_vec);
}

double get_random_mass(double a, double b, double m)
{
    double m_prim = rand_a_b(a, b) * m;
    double m_final = m_prim + M_MIN;
    return m_final;
}

Vec *get_random_position(double a, double b)
{
    double alpha = R_INIT * (log(1.0 - rand_a_b(a, b)) / 1.8);
    Vec *delta = new_vec(0.5 - rand_a_b(a, b), 0.5 - rand_a_b(a, b));
    Vec *position = mul_vec(alpha, delta);
    free(delta);
    return position;
}

Vec *get_velocite(double m_star, double m_ini, Vec *pos_star)
{
    double theta = atan2(pos_star->y, pos_star->x);
    double r_i = norm(pos_star);
    double alpha = sqrt((G * (m_star + m_ini)) / r_i);
    Vec *delta = new_vec(-sin(theta), cos(theta));
    Vec *velocite = mul_vec(alpha, delta);
    free(delta);
    return velocite;
}

double rand_a_b(double min, double max)
{
    return ((double)rand() / RAND_MAX) * (max - min) + min;
}

void simple_update_acc_of_all_stars(Galaxy *g)
{
    for (int i = 1; i < g->num_bodies; i++)
    {
        Vec *acc_final = new_vec(0.0, 0.0);
        Vec *tmp;
        for (int j = 0; j < g->num_bodies; j++)
        {
            if (i != j)
            {
                tmp = acc_final;
                Vec *acc = simple_update_acceleration(&g->stars[i], &g->stars[j]);
                acc_final = add_vec(tmp, acc);
                free(acc);
                free(tmp);
            }
        }
        g->stars[i].acc = *acc_final;
        free(acc_final);
    }
}

void reset_accelerations(Galaxy *g, int from, int to)
{
    for (int i = from; i < to; i++)
    {
        reset_acceleration(&g->stars[i]);
    }
}

void update_positions(Galaxy *g, int from, int to, const double dt)
{
    for (int i = from; i < to; i++)
    {
        update_position(&g->stars[i], dt);
    }
}

void free_galaxy(Galaxy *g)
{
    free(g->stars);
    // free(g);
}

void copy_without(Galaxy *g, int *i_to_remove, int nb_to_remove, int *remove_index, int local_nb_to_remove, int from, int to, int nb_threads, int thread_id, pthread_barrier_t *barrier, pthread_mutex_t *resize_mutex)
{
    if (nb_to_remove > 0)
    {
        // int old_size = g->num_bodies;
        int new_size = g->num_bodies - nb_to_remove;
        int local_size = to - from - local_nb_to_remove;
        // Wait on all threads to init tmp ptr
        // printf("BEFORE thread_id: %d g->stars: %p tmp: %p\n", thread_id, g->stars, tmp);
        Star *tmp = (Star *)malloc(local_size * sizeof(Star));

        // printf("AFTER thread_id: %d g->stars: %p tmp: %p\n", thread_id, g->stars, tmp);

        // int j_step = new_size / nb_threads;
        // int j_from = thread_id * j_step;

        // if (thread_id == 1)
        // {
        //     printf("===================\n");
        // }
        int j = 0;
        for (int i = from; i < to; i++)
        {
            if (i_to_remove[i] == 0)
            {
                // if (thread_id == 1)
                // {
                //     printf("old_size:%d new_size:%d i:%d j_from:%d\n", old_size, new_size, i, j_from);
                // }
                // pthread_mutex_lock(resize_mutex);
                tmp[j] = g->stars[i];
                j++;
                // *remove_index += 1;
                // pthread_mutex_unlock(resize_mutex);
            }
        }

        // Wait on all threads
        pthread_barrier_wait(barrier);
        if (thread_id == 0)
        {
            g->stars = (Star *)realloc((void *)g->stars, new_size * sizeof(Star));
            g->num_bodies = new_size;
        }
        // Wait on threads 0 to init new stars ptr
        pthread_barrier_wait(barrier);
        // if (thread_id == 1)
        // {
        //     printf("===================\n");
        // }

        pthread_mutex_lock(resize_mutex);
        // printf("threads:%d copy %d stars from %d\n", thread_id, local_size, *remove_index);
        memcpy((void *)&g->stars[*remove_index], (void *)tmp, local_size * sizeof(Star));
        *remove_index += local_size;
        pthread_mutex_unlock(resize_mutex);

        // // Wait on all threads to stop finish copy
        // pthread_barrier_wait(barrier);
        // if (thread_id == 0)
        // {
        free(tmp);
        // }
    }
}

// void resize_galaxy(Galaxy *g)
// {
//     int i_to_remove[g->num_bodies];
//     int n_to_remove = 0;
//     for (int i = 0; i < g->num_bodies; i++)
//     {
//         if (!is_inside(g->b, g->stars[i].pos_t))
//         {
//             i_to_remove[n_to_remove] = i;
//             n_to_remove++;
//         }
//     }

//     if (n_to_remove > 0)
//     {
//         copy_without(g, i_to_remove, n_to_remove);
//     }
// }

int set_i_to_remove(Galaxy *g, int from, int to, int *nb_to_remove, int *i_to_remove, pthread_mutex_t *resize_mutex)
{
    int local_nb_to_remove = 0;
    for (int i = from; i < to; i++)
    {
        if (!is_inside(g->b, g->stars[i].pos_t))
        {
            i_to_remove[i] = 1;
            local_nb_to_remove += 1;
        }
    }
    pthread_mutex_lock(resize_mutex);
    *nb_to_remove += local_nb_to_remove;
    pthread_mutex_unlock(resize_mutex);
    return local_nb_to_remove;
}

void clean_i_to_remove(int from, int to, int *nb_to_remove, int *i_to_remove, int *remove_index, pthread_mutex_t *resize_mutex)
{
    // pthread_mutex_lock(resize_mutex);
    *nb_to_remove = 0;
    *remove_index = 0;
    // pthread_mutex_unlock(resize_mutex);
    for (int i = from; i < to; i++)
    {
        i_to_remove[i] = 0;
    }
}