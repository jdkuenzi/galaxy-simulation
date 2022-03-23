/*
 * Author   : Küenzi Jean-Daniel
 * Version  : 1.0.0
 * Date     : 30.05.2019
 * Update   : 08.06.2019
 * Desc.    : Fichier contenant les fonctions permettant d'utiliser les galaxies (galaxy)
 */

#include "galaxy.h"

Galaxy *create_and_init_galaxy(int num_bodies, Box b, double dt)
{
    Galaxy *new_galaxy = (Galaxy *)malloc(sizeof(Galaxy));
    if (new_galaxy == NULL)
    {
        printf("Erreur lors de la creation de la galaxie !");
        exit(0);
    }
    new_galaxy->num_bodies = num_bodies;
    new_galaxy->stars = (Star *)malloc(num_bodies * sizeof(Star));
    if (new_galaxy->stars == NULL)
    {
        printf("Erreur lors de la creation du tableau d'etoile pour galaxie !");
        exit(0);
    }
    new_galaxy->b = b;

    Vec *zero_vec = new_vec(0.0, 0.0);
    srand(time(NULL)); // Initialise rand
    double heat = rand_a_b(MIN_HEAT, MAX_HEAT);

    Star *central_star = new_star_vel(*zero_vec, *zero_vec, *zero_vec, M_CENTRAL, heat, dt); // Etoile Initiale, au centre
    new_galaxy->stars[0] = *central_star;
    free(central_star);

    for (int i = 1; i < num_bodies; i++)
    {
        double m_star = get_random_mass(0, 10, M_SOLAIRE);
        heat = rand_a_b(MIN_HEAT, MAX_HEAT);
        Vec *pos_star = get_random_position(0, 1);
        Vec *vel_vec = get_velocite(m_star, M_CENTRAL, pos_star);
        Star *new_star = new_star_vel(*pos_star, *vel_vec, *zero_vec, m_star, heat, dt);
        new_galaxy->stars[i] = *new_star;
        free(pos_star);
        free(vel_vec);
        free(new_star);
    }

    free(zero_vec);

    return new_galaxy;
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

void reset_accelerations(Galaxy *g)
{
    for (int i = 0; i < g->num_bodies; i++)
    {
        reset_acceleration(&g->stars[i]);
    }
}

void update_positions(Galaxy *g, double dt)
{
    for (int i = 0; i < g->num_bodies; i++)
    {
        update_position(&g->stars[i], dt);
    }
}

void free_galaxy(Galaxy *g)
{
    free(g->stars);
    free(g);
}

void copy_without(Galaxy *g, int *i_to_remove, int n_to_remove)
{
    Star *tmp = g->stars;
    int new_size = g->num_bodies - n_to_remove;
    Star *new_stars = (Star *)malloc(new_size * sizeof(Star));

    int j = 0;
    int n = 0;

    for (int i = 0; i < g->num_bodies; i++)
    {
        if (i != i_to_remove[n])
        {
            new_stars[j] = tmp[i];
            j++;
        }
        else if (n < n_to_remove - 1)
        {
            n++;
        }
    }

    g->stars = new_stars;
    g->num_bodies = new_size;
    free(tmp);
}

void resize_galaxy(Galaxy *g)
{
    int i_to_remove[g->num_bodies];
    int n_to_remove = 0;
    for (int i = 0; i < g->num_bodies; i++)
    {
        if (!is_inside(g->b, g->stars[i].pos_t))
        {
            i_to_remove[n_to_remove] = i;
            n_to_remove++;
        }
    }

    if (n_to_remove > 0)
    {
        copy_without(g, i_to_remove, n_to_remove);
    }
}