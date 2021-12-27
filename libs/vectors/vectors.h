/*
 * Author   : Küenzi Jean-Daniel
 * Version  : 1.0.0
 * Date     : 23.05.2019
 * Update   : 23.05.2019
 * Desc.    : Entete du fichier vectors.c
 */

#ifndef _VECTORS_H_
#define _VECTORS_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

typedef struct __vec {
    double x, y;
}Vec;

/***
 * Sert a instancier un nouveau Vec
 * @Param x : la coordonnee X
 * @Param y : la coordonnee Y
 * @Return : Un pointeur sur une struct __vec
 */ 
Vec* new_vec(double, double);
/***
 * Sert a additioner deux Vec entre eux
 * @Param v1 : une struct __vec
 * @Param v2 : une struct __vec
 * @Return : une struct __vec qui est le resultat de l'addition
 */ 
Vec* add_vec(const Vec *const, const Vec *const);
/***
 * Sert a soustraire deux Vec entre eux
 * @Param v1 : une struct __vec
 * @Param v2 : une struct __vec
 * @Return : une struct __vec qui est le resultat de la soustraction
 */ 
Vec* sub_vec(const Vec *const, const Vec *const);
/***
 * Sert a multiplier un Vec par un scalaire (double)
 * @Param alpha : un scalaire de type double
 * @Param v1 : une struct __vec
 * @Return : une struct __vec qui est le resultat de la multiplication
 */ 
Vec* mul_vec(double, const Vec *const);
/***
 * Sert a calculer la norme d'un Vec
 * @Param v1 : une struct __vec
 * @Return : la norme du Vec
 */ 
double norm(const Vec *const);
/***
 * Sert a calculer la distance entre deux Vec
 * @Param v1 : une struct __vec
 * @Param v2 : une struct __vec
 * @Return : la distance entre les deux Vec
 */ 
double distance(const Vec *const, const Vec *const);
/***
 * Affiche le Vec dans la console / terminal
 * @Param v1 : une struct __vec
 */ 
void print_vec(const Vec *const);

#endif