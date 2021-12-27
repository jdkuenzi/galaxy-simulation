/*
 * Author   : Küenzi Jean-Daniel
 * Version  : 1.0.0
 * Date     : 14.10.2021
 * Update   : 14.10.2021
 * Desc.    : Fichier de tests pour les box (box)
 */

#include "test_box.h"

int main()
{
    test_is_inside();
    test_divide_in_four();
    return 1;
}

void test_is_inside() {
    Box b = new_box(-4, 4, -4, 4);
    Vec *v1 = new_vec(-2, 3);
    Vec *v2 = new_vec(-6, -6);
    Vec *v3 = new_vec(-4, 4);
    Vec *v4 = new_vec(-4, 7);

    assert(is_inside(b, *v1) == true);
    assert(is_inside(b, *v2) == false);
    assert(is_inside(b, *v3) == true);
    assert(is_inside(b, *v4) == false);

    printf("test_is_inside : OK\n");

    free(v1);
    free(v2);
    free(v3);
    free(v4);
}

void test_divide_in_four() {
    Box b1 = new_box(-4, 3, -3, 4);
    Box b_4[4];

    divide_in_four(b1, b_4);

    assert(
        b_4[0].x0 == -4.0 &&
        b_4[0].x1 == -0.5 &&
        b_4[0].y0 == 0.5 &&
        b_4[0].y1 == 4.0
    );
    assert(
        b_4[1].x0 == -0.5 &&
        b_4[1].x1 == 3.0 &&
        b_4[1].y0 == 0.5 &&
        b_4[1].y1 == 4.0
    );
    assert(
        b_4[2].x0 == -4.0 &&
        b_4[2].x1 == -0.5 &&
        b_4[2].y0 == -3.0 &&
        b_4[2].y1 == 0.5
    );
    assert(
        b_4[3].x0 == -0.5 &&
        b_4[3].x1 == 3.0 &&
        b_4[3].y0 == -3.0 &&
        b_4[3].y1 == 0.5
    );

    printf("test_divide_in_four : OK\n");
}