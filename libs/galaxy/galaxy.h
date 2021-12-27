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
#include "../stars/stars.h"

#define R_INIT pow(10, 18)
#define M_MIN pow(10, 20)
#define M_CENTRAL pow(10, 6) * M_SOLAIRE

typedef struct __galaxy {
    int num_bodies;
    Star* stars;
    Box b;
} Galaxy;

Galaxy* create_and_init_galaxy(int, Box, const double);
double get_random_mass(double, double, double);
Vec *get_random_position(double, double);
Vec *get_velocite(double, double, Vec*);
double rand_a_b(double, double);
void reset_accelerations(Galaxy*);
void simple_update_acc_of_all_stars(Galaxy *g);
void update_positions(Galaxy *, const double);
void free_galaxy(Galaxy*);
void resize_galaxy(Galaxy*);

#endif