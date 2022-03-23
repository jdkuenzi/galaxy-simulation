/*
 * Author   : Küenzi Jean-Daniel
 * Version  : 1.0.0
 * Date     : 27.05.2019
 * Update   : 27.05.2019
 * Desc.    : Fichier contenant les fonctions permettant d'utiliser les box
 */

#include "box.h"

Box new_box(double x0, double x1, double y0, double y1)
{
    Box new_box;
    new_box.x0 = x0;
    new_box.x1 = x1;
    new_box.y0 = y0;
    new_box.y1 = y1;
    return new_box;
}

void divide_in_four(Box b, Box *b_4)
{
    double middle_y = (b.y0 + b.y1) / 2;
    double middle_x = (b.x0 + b.x1) / 2;

    b_4[0] = new_box(b.x0, middle_x, middle_y, b.y1); // left-upper
    b_4[1] = new_box(middle_x, b.x1, middle_y, b.y1); // right-upper
    b_4[2] = new_box(b.x0, middle_x, b.y0, middle_y); // left-lower
    b_4[3] = new_box(middle_x, b.x1, b.y0, middle_y); // right-lower
}

bool is_inside(Box b, Vec v)
{
    return (v.x > b.x0 && v.x < b.x1 && v.y > b.y0 && v.y < b.y1) ? true : false;
}

double compute_length(Box b)
{
    double length_h = b.x1 - b.x0;
    double length_v = b.y1 - b.y0;
    return (length_h >= length_v) ? length_h : length_v;
}