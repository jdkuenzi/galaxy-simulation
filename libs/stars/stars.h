/*
 * Author   : Küenzi Jean-Daniel
 * Version  : 1.0.0
 * Date     : 23.05.2019
 * Update   : 26.05.2019
 * Desc.    : Entete du fichier stars.c
 */

#ifndef _STARS_H_
#define _STARS_H_

#include "../box/box.h"

#define G 6.67 * pow(10, -11)
#define M_SOLAIRE 1.98892 * pow(10, 30)
#define EPSILON 3 * pow(10, 4)

typedef struct __star {
    Vec pos_t, pos_t_dt, acc;
    double mass;
} Star;

/***
 * Sert a instancier une nouvelle Star
 * @Param pos : 
 * @Param vel : 
 * @Param acc : 
 * @Param mass : 
 * @Param dt :
 * @Return : Un pointeur sur une struct __vec
 */ 
Star* new_star_vel(Vec, Vec, Vec, double, double);
Star *new_super_star(const Star *const, const Star *const);
void increment_super_star(Star *, const Star *const);
Vec *get_mass_center(const Star *const, const Star *const);
void reset_acceleration(Star *);
void update_acceleration(Star*, const Star *const);
Vec *simple_update_acceleration(Star *s, const Star *const s2);
void update_position(Star *, double);
void print_star(const Star *const s);

#endif