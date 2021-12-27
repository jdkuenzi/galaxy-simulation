/*
 * Author   : Küenzi Jean-Daniel
 * Version  : 1.0.0
 * Date     : 23.05.2019
 * Update   : 26.05.2019
 * Desc.    : Fichier de tests pour les vecteurs
 */

#include "test_vectors.h"

int main() { 
    test_add_vec();
    test_sub_vec();
    test_mul_vec();
    test_norm();
    test_distance();
    return 1;
}

void test_add_vec() {
    Vec* v1 = new_vec(1, -6);
    Vec* v2 = new_vec(7, 2);
    Vec* v_result = new_vec(8, -4);

    Vec* v3 = add_vec(v1, v2);
    assert(v3->x == v_result->x);
    assert(v3->y == v_result->y);
    printf("test_add_vec : OK\n");
    free(v1);
    free(v2);
    free(v_result);
    free(v3);

}

void test_sub_vec() {
    Vec* v1 = new_vec(1, -6);
    Vec* v2 = new_vec(7, 2);
    Vec* v_result = new_vec(-6, -8);

    Vec* v3 = sub_vec(v1, v2);
    assert(v3->x == v_result->x);
    assert(v3->y == v_result->y);
    printf("test_sub_vec : OK\n");
    free(v1);
    free(v2);
    free(v_result);
    free(v3);
}

void test_mul_vec() {
    Vec* v1 = new_vec(1, -6);
    double alpha = 4;
    Vec* v_result = new_vec(4, -24);

    Vec* v3 = mul_vec(alpha, v1);
    assert(v3->x == v_result->x);
    assert(v3->y == v_result->y);
    printf("test_mul_vec : OK\n");
    free(v1);
    free(v_result);
    free(v3);
}

void test_norm() {
    Vec* v1 = new_vec(3, 4);
    double result = 5;

    double norm_result = norm(v1);
    assert(norm_result == result);
    printf("test_norm : OK\n");
    free(v1);
}

void test_distance() {
    Vec* v1 = new_vec(5, 0);
    Vec* v2 = new_vec(11, 8);
    double result = 10;

    double distance_result = distance(v1, v2);
    assert(result == distance_result);
    printf("test_distance : OK\n");
    free(v1);
    free(v2);
}