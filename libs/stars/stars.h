/*
 * Author   : Küenzi Jean-Daniel
 * Version  : 1.0.0
 * Date     : 23.05.2019
 * Update   : 26.05.2019
 * Desc.    : Entete du fichier stars.c
 */

#ifndef _STARS_H_
#define _STARS_H_

#include <stdint.h>
#include "../box/box.h"

#define MAKE_COLOR(r, g, b) ((uint32_t)b | ((uint32_t)g << 8) | ((uint32_t)r << 16))

#define G 6.67 * pow(10, -11)
#define M_SOLAIRE 1.98892 * pow(10, 30)
#define EPSILON 3 * pow(10, 4)
#define MIN_HEAT 2000.0
#define RED_HEAT 3000.0
#define ORANGE_HEAT 4500.0
#define YELLOW_HEAT 6500.0
#define WHITE_HEAT 10000.0
#define LIGHT_BLUE_HEAT 20000.0
#define MAX_HEAT 30000.0

typedef struct __star
{
    Vec pos_t, pos_t_dt, acc;
    double mass, heat;
    uint32_t color;
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
Star *new_star_vel(Vec, Vec, Vec, double, double, double);
Star *new_super_star(const Star *const, const Star *const);
void increment_super_star(Star *, const Star *const);
uint32_t get_color(const Star *const s);
Vec *get_mass_center(const Star *const, const Star *const);
void reset_acceleration(Star *);
void update_acceleration(Star *, const Star *const);
Vec *simple_update_acceleration(Star *s, const Star *const s2);
void update_position(Star *, double);
void print_star(const Star *const s);

#endif