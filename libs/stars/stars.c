/*
 * Author   : Küenzi Jean-Daniel
 * Version  : 1.0.0
 * Date     : 26.05.2019
 * Update   : 30.05.2019
 * Desc.    : Fichier contenant les fonctions permettant d'utiliser les etoiles (stars)
 */

#include "stars.h"

int cpt = 0;

Star* new_star_vel(Vec pos, Vec vel, Vec acc, double mass, double dt) {
    Star* new_star = (Star*)malloc(sizeof(Star));
    if (new_star == NULL) {
        printf("Erreur lors de la creation de l'etoile !");
        exit(0);
    }
    new_star->acc = acc;
    new_star->mass = mass;
    new_star->pos_t = pos;
    Vec* dt_vel = mul_vec(dt, &vel);
    Vec* pos_t_dt = sub_vec(&pos, dt_vel);
    new_star->pos_t_dt = *pos_t_dt;
    free(dt_vel);
    free(pos_t_dt);
    return new_star;
}

Star *new_super_star(const Star *const s, const Star *const s2) {
    Star *new_super_s = (Star *)malloc(sizeof(Star));
    Vec *tmp = get_mass_center(s, s2);
    new_super_s->pos_t = *tmp;
    free(tmp);
    new_super_s->mass = s->mass + s2->mass;
    return new_super_s;
}

void increment_super_star(Star *super_s, const Star *const s) {
    Vec *tmp = get_mass_center(super_s, s);
    super_s->pos_t = *tmp;
    free(tmp);
    super_s->mass += s->mass;
}

Vec* get_mass_center(const Star *const s, const Star *const s2)
{
    double total_mass = s->mass + s2->mass;
    Vec *alpha = mul_vec(s->mass, &s->pos_t);
    Vec *delta = mul_vec(s2->mass, &s2->pos_t);
    Vec *sum = add_vec(alpha, delta);
    Vec *mass_center = mul_vec(1.0 / total_mass, sum);
    free(alpha);
    free(delta);
    free(sum);
    return mass_center;
}

void reset_acceleration(Star *s)
{
    Vec* new_acc = new_vec(0.0, 0.0);
    s->acc = *new_acc;
    free(new_acc);
}

void update_acceleration(Star* s, const Star *const s2) {
    Vec *alpha = sub_vec(&s2->pos_t, &s->pos_t);                 // alpha = r_j - r_i
    Vec *delta = mul_vec(G * s->mass * s2->mass, alpha);         // delta = G * mi * mj * alpha
    double r_ji = distance(&s2->pos_t, &s->pos_t);               // r_ji = ||r_j - r_i||
    Vec *Fij = mul_vec(1.0 / pow((r_ji + EPSILON), 3.0), delta); // (1/(r_ij + E)^3) * delta
    Vec *acc = mul_vec(1 / s->mass, Fij);
    s->acc = *acc;
    free(delta);
    free(alpha);
    free(Fij);
    free(acc);
}

Vec *simple_update_acceleration(Star* s, const Star *const s2) {
    Vec *alpha = sub_vec(&s2->pos_t, &s->pos_t);   // alpha = r_j - r_i
    Vec* delta = mul_vec(G * s->mass * s2->mass, alpha); // delta = G * mi * mj * alpha
    double r_ji = distance(&s2->pos_t, &s->pos_t); // r_ji = ||r_j - r_i||
    Vec *Fij = mul_vec(1.0 / pow((r_ji + EPSILON), 3.0), delta); // (1/(r_ij + E)^3) * delta
    Vec *acc = mul_vec(1 / s->mass, Fij);

    free(alpha);
    free(delta);
    free(Fij);
    return acc;
}

void update_position(Star* s, double dt) {
    Vec* alpha = mul_vec(2.0, &s->pos_t); // 2 * pos_t
    Vec* delta = sub_vec(alpha, &s->pos_t_dt); // (2 * pos_t) - pos_t_dt
    Vec* gamma = mul_vec(pow(dt, 2.0), &s->acc); // dt^2 * acc
    Vec* result_vec = add_vec(delta, gamma); // delta + gamma
    s->pos_t_dt = s->pos_t;
    s->pos_t = *result_vec;
    free(alpha);
    free(delta);
    free(gamma);
    free(result_vec);
}

void print_star(const Star* const s) {
    printf("+------ Star's info ------+\n");
    printf("acc      -> ");
    print_vec(&s->acc);
    printf("mass     -> %lf\n", s->mass);
    printf("pos_t    -> ");
    print_vec(&s->pos_t);
    printf("pos_t_dt -> ");
    print_vec(&s->pos_t_dt);
    printf("+-------------------------+\n");
}