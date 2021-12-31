/*
 * Author   : Küenzi Jean-Daniel
 * Version  : 1.0.0
 * Date     : 30.05.2019
 * Update   : 30.05.2019
 * Desc.    : Entete du fichier galaxy.c
 */

#ifndef _GALAXY_H_
#define _GALAXY_H_

#include <time.h>
#include <pthread.h>
#include "../stars/stars.h"

#define R_INIT pow(10, 18)
#define M_MIN pow(10, 20)
#define M_CENTRAL pow(10, 6) * M_SOLAIRE

typedef struct __galaxy
{
    int num_bodies;
    Star *stars;
    Box b;
} Galaxy;

void create_and_init_galaxy(Galaxy *, int, Box, const double);
double get_random_mass(double, double, double);
Vec *get_random_position(double, double);
Vec *get_velocite(double, double, Vec *);
double rand_a_b(double, double);
void reset_accelerations(Galaxy *, int, int);
void simple_update_acc_of_all_stars(Galaxy *g);
void update_positions(Galaxy *, int, int, const double);
void free_galaxy(Galaxy *);

void copy_without(
    Galaxy *,
    int *i_to_remove,
    int nb_to_remove,
    int *remove_index,
    int from,
    int to,
    int nb_threads,
    int thread_id,
    pthread_barrier_t *barrier,
    pthread_mutex_t *resize_mutex);

void set_i_to_remove(
    Galaxy *g,
    int from,
    int to,
    int *nb_to_remove,
    int *i_to_remove,
    pthread_mutex_t *resize_mutex);

void clean_i_to_remove(
    int from,
    int to,
    int *nb_to_remove,
    int *i_to_remove,
    int *remove_index,
    pthread_mutex_t *resize_mutex);

#endif