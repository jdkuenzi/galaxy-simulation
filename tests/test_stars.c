/*
 * Author   : Küenzi Jean-Daniel
 * Version  : 1.0.0
 * Date     : 23.05.2019
 * Update   : 26.05.2019
 * Desc.    : Fichier de tests pour les etoiles (stars)
 */

#include "test_stars.h"

int main() { 
    test_update_acc();
    test_update_pos();
    test_get_mass_center();
    test_increment_super_s();
    return 1;
}

void test_update_acc() {
    Vec* zero = new_vec(0, 0);
    Vec* pos_1 = new_vec(1, 1);
    Vec* pos_2 = new_vec(1, 4);
    double mass_1 = 2.0 * pow(10, 25);
    double mass_2 = 3.0 * pow(10, 25);

    Star* s1 = new_star_vel(*pos_1, *zero, *zero, mass_1, 0);
    Star* s2 = new_star_vel(*pos_2, *zero, *zero, mass_2, 0);

    update_acceleration(s1, s2);

    Vec *acc_result = new_vec(0.0, 222.266647);
    // print_star(s1);
    assert((int)s1->acc.x == (int)acc_result->x);
    assert((int)s1->acc.y == (int)acc_result->y);
    printf("test_update_acc : OK\n");
    free(pos_1);
    free(pos_2);
    free(zero);
    free(s1);
    free(s2);
    free(acc_result);
}

void test_update_pos() {
    Vec *pos_1 = new_vec(1, 1);
    Vec *pos_2 = new_vec(1, 4);
    Vec *acc_1 = new_vec(1, 0);
    Vec *acc_2 = new_vec(2, 3);
    Vec *vel = new_vec(1, 1);
    double mass_1 = 2.0;
    double mass_2 = 3.0;
    double dt = 3.0;
    Star *s1 = new_star_vel(*pos_1, *vel, *acc_1, mass_1, dt);

    Star *s2 = new_star_vel(*pos_2, *vel, *acc_2, mass_2, dt);

    // print_star(s2);
    update_position(s1, dt);
    Vec *acc_result = new_vec(13.0, 4.0);
    // print_star(s1);
    assert(s1->pos_t.x == acc_result->x);
    assert(s1->pos_t.y == acc_result->y);
    free(acc_result);

    update_position(s2, dt);
    acc_result = new_vec(22.0, 34.0);
    // print_star(s2);
    assert(s2->pos_t.x == acc_result->x);
    assert(s2->pos_t.y == acc_result->y);
    printf("test_update_pos : OK\n");
    free(acc_result);
    free(pos_1);
    free(pos_2);
    free(acc_1);
    free(acc_2);
    free(vel);
    free(s1);
    free(s2);
    // free(acc_result);
}

void test_get_mass_center() {
    Vec *zero = new_vec(0, 0);
    Vec *pos_1 = new_vec(0, 0);
    Vec *pos_2 = new_vec(1, 0);
    Vec *pos_3 = new_vec(0.5, 0);
    Vec *pos_4 = new_vec(0.5, 1);
    double mass_1 = 2.0;
    double mass_2 = 2.0;
    double mass_3 = 4.0;
    double mass_4 = 2.0;
    double dt = 3.0;
    Star *s1 = new_star_vel(*pos_1, *zero, *zero, mass_1, dt);
    Star *s2 = new_star_vel(*pos_2, *zero, *zero, mass_2, dt);
    Star *s3 = new_star_vel(*pos_3, *zero, *zero, mass_3, dt);
    Star *s4 = new_star_vel(*pos_4, *zero, *zero, mass_4, dt);

    Vec *correct = new_vec(0.5, 0);
    Vec *res = get_mass_center(s1, s2);
    // print_vec(res);
    assert(res->x == correct->x);
    assert(res->y == correct->y);

    Vec *correct_2 = new_vec(0.5, 1.0/3.0);
    Vec *res_2 = get_mass_center(s3, s4);
    // print_vec(res_2);
    assert(res_2->x == correct_2->x);
    assert(res_2->y == correct_2->y);
    printf("test_get_mass_center : OK\n");
    free(zero);
    free(pos_1);
    free(pos_2);
    free(pos_3);
    free(pos_4);
    free(s1);
    free(s2);
    free(s3);
    free(s4);
    free(correct);
    free(res);
    free(correct_2);
    free(res_2);
}

void test_increment_super_s()
{
    Vec *zero = new_vec(0, 0);
    Vec *pos_1 = new_vec(0.5, 0);
    Vec *pos_2 = new_vec(0.5, 1.0);
    double mass_1 = 4.0;
    double mass_2 = 2.0;
    double dt = 3.0;
    Star *s1 = new_star_vel(*pos_1, *zero, *zero, mass_1, dt);
    Star *s2 = new_star_vel(*pos_2, *zero, *zero, mass_2, dt);

    Vec *correct = new_vec(0.5, 1.0/3.0);
    increment_super_star(s1, s2);
    // print_vec(res);
    assert(s1->pos_t.x == correct->x);
    assert(s1->pos_t.y == correct->y);

    printf("test_increment_super_s : OK\n");
    free(zero);
    free(pos_1);
    free(pos_2);
    free(s1);
    free(s2);
    free(correct);
}