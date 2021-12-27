/*
 * Author   : Küenzi Jean-Daniel
 * Version  : 1.0.0
 * Date     : 23.05.2019
 * Update   : 23.05.2019
 * Desc.    : Entete du fichier box.c
 */

#ifndef _BOX_H_
#define _BOX_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../vectors/vectors.h"

typedef struct __box {
    double x0, x1, y0, y1;
}Box;

/**
 * Créer une Box
 * @param x0 La coordonnée x ou la box commence
 * @param x1 La coordonnée x ou la box se termine
 * @param y0 La coordonnée y ou la box commence
 * @param y1 La coordonnée y ou la box se termine
 * @return La Box 
 */
Box new_box(double, double, double, double);

/**
 * Divise la Box passée en parametre en 4 struct Box
 * @param b Une Box
 * @param b_4 Un pointeur de box
 */ 
void divide_in_four(Box, Box*);

/**
 * Regarde si un Vec est dans une Box
 * @param b une struct Box
 * @param v une struct Vec
 * @return true si il est dedans | sinon false
 */
bool is_inside(Box, Vec);
/***
 * Renvoie le plus grand cote de la Box
 * @Param b : une struct __box
 * @Return : le plus grand cote de la Box
 */ 
double compute_length(Box);

#endif