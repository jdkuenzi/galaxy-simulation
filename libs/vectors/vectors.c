/*
 * Author   : Küenzi Jean-Daniel
 * Version  : 1.0.0
 * Date     : 23.05.2019
 * Update   : 26.05.2019
 * Desc.    : Fichier contenant les fonctions permettant d'utiliser les vecteurs
 */

#include "vectors.h"

Vec* new_vec(double x, double y) {
    Vec* new_vec = (Vec*)malloc(sizeof(Vec));
    if (new_vec == NULL) {
        printf("Erreur lors de la creation du vecteur !");
        exit(0);
    }
    new_vec->x = x;
    new_vec->y = y;
    return new_vec;
}

Vec* add_vec(const Vec *const v1, const Vec *const v2) {
    double result_x = v1->x + v2->x;
    double result_y = v1->y + v2->y;
    return new_vec(result_x, result_y);
}

Vec* sub_vec(const Vec *const v1, const Vec *const v2) {
    double result_x = v1->x - v2->x;
    double result_y = v1->y - v2->y;
    return new_vec(result_x, result_y);
}

Vec* mul_vec(double alpha, const Vec *const v1) {
    double result_x = alpha * v1->x;
    double result_y = alpha * v1->y;
    return new_vec(result_x, result_y);
}

double norm(const Vec *const v1) {
    double norm = sqrt(pow(v1->x, 2.0) + pow(v1->y, 2.0));
    return norm;
}

double distance(const Vec *const v1, const Vec *const v2) {
    Vec *alpha = sub_vec(v1, v2);
    double distance = norm(alpha);
    free(alpha);
    return distance;
}

void print_vec(const Vec *const v1) {
    printf("Vec(%lf ; %lf)\n", v1->x, v1->y);
}
