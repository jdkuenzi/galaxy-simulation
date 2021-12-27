/*
 * Author   : Küenzi Jean-Daniel
 * Version  : 1.0.0
 * Date     : 30.05.2019
 * Update   : 08.06.2019
 * Desc.    : Le main du programme galaxy_simulation qui va gérer les galaxies et l'affichage
 */

#include "galaxy_simulation.h"

int main(int argc, char **argv)
{
    int n_bodies;
    double theta;

    if (argc >= 2)
    {
        n_bodies = atoi(argv[1]);
        theta = atof(argv[2]);
        if (n_bodies <= 0 || theta <= 0) {
            printf("num_stars et/ou theta doivent être plus grand que 0\n");
            return EXIT_FAILURE;
        }
    }
    else
    {
        printf("galaxy_simulation <num_stars> <theta>\n");
        printf("    - num_stars est un entier représentant le nombre d'étoiles dans la galaxie et theta la valeur définissant la distance minimale entre les nœuds et les feuilles du quad_tree.\n");
        printf("Exemple : ./galaxy_simulation 10000 10\n");
        return EXIT_FAILURE;
    }

    printf("theta %lf\n", theta);

    Box b = new_box(-R_INIT, R_INIT, -R_INIT, R_INIT);
    const double dt = pow(10, 10);
    Galaxy *g = create_and_init_galaxy(n_bodies, b, dt);
    struct gfx_context_t *context = create_gfx();

    bool show_quad_tree = false;
    bool show_super_s = false;

    SDL_ShowCursor(SDL_ENABLE);  // needed to se the cursor
    SDL_Keycode key_pressed = 0; // escape key needed
    SDL_Keycode old_key_pressed = key_pressed;
    while (true)
    {
        SDL_PumpEvents();
        key_pressed = gfx_keypressed();
        if (key_pressed != old_key_pressed) {
            if (key_pressed == SDLK_ESCAPE)
            {
                break;
            } else if(key_pressed == SDLK_1) {
                show_quad_tree = !show_quad_tree;
            }
            else if (key_pressed == SDLK_2)
            {
                show_super_s = !show_super_s;
            } else if (key_pressed == SDLK_r) {
                free_galaxy(g);
                g = create_and_init_galaxy(n_bodies, b, dt);
            }
            old_key_pressed = key_pressed;
        }
        resize_galaxy(g);
        gfx_clear(context, COLOR_BLACK);
        show_pixels(context, g);
        reset_accelerations(g);
        Quad_tree *q_tree = create_quad_tree_from_galaxy(g);
        update_accelerations_of_all_stars(q_tree->root, g, theta);
        // simple_update_acc_of_all_stars(g);
        update_positions(g, dt);
        if (show_quad_tree || show_super_s) {
            draw_quad_tree(context, q_tree->root, show_quad_tree, show_super_s);
        }
        free_quad_tree(q_tree);
        gfx_present(context);
    }
    gfx_destroy(context);
    free_galaxy(g);

    return EXIT_SUCCESS;
}

void show_pixels(struct gfx_context_t *context, Galaxy *g)
{
    for (int i = 0; i < g->num_bodies; i++)
    {
        int x = (int)(g->stars[i].pos_t.x * (context->width / (R_INIT * 2)) + context->width / 2);
        int y = (int)(g->stars[i].pos_t.y * (context->height / (R_INIT * 2)) + context->height / 2);
        gfx_putpixel(
            context,
            x,
            y,
            COLOR_WHITE);
    }
}

void draw_quad_tree(struct gfx_context_t *context, Node *n, bool show_quad_tree, bool show_super_s)
{
    if (show_quad_tree) {
        draw_box(context, n->b);
    }
    if (!is_leaf(n))
    {
        if (show_super_s) {
            draw_super_s(context, n->super_s);
        }
        for (int i = 0; i < 4; i++)
        {
            draw_quad_tree(context, n->children[i], show_quad_tree, show_super_s);
        }
    }
}

void draw_super_s(struct gfx_context_t *context, Star *super_s)
{
    int x = (int)(super_s->pos_t.x * (context->width / (R_INIT * 2)) + context->width / 2);
    int y = (int)(super_s->pos_t.y * (context->height / (R_INIT * 2)) + context->height / 2);

    if (x > 0) {
        gfx_putpixel(context, x - 1, y, COLOR_RED);
        if (y > 0) {
            gfx_putpixel(context, x - 1, y - 1, COLOR_RED);
        }
        if (y < NY) {
            gfx_putpixel(context, x - 1, y + 1, COLOR_RED);
        }
    }
    
    if (x < NX) {
        gfx_putpixel(context, x + 1, y, COLOR_RED);
        if (y > 0) {
            gfx_putpixel(context, x + 1, y - 1, COLOR_RED);
        }
        if (y < NY) {
            gfx_putpixel(context, x + 1, y + 1, COLOR_RED);
        }
    }

    if(y > 0) {
        gfx_putpixel(context, x, y - 1, COLOR_RED);
    }
    if(y < NY) {
        gfx_putpixel(context, x, y + 1, COLOR_RED);
    }
    gfx_putpixel(context, x, y, COLOR_RED);
}

void draw_box(struct gfx_context_t *context, Box b)
{
    int x0 = (int)(b.x0 * (context->width / (R_INIT * 2)) + context->width / 2);
    int y0 = (int)(b.y0 * (context->height / (R_INIT * 2)) + context->height / 2);
    int x1 = (int)(b.x1 * (context->width / (R_INIT * 2)) + context->width / 2);
    int y1 = (int)(b.y1 * (context->height / (R_INIT * 2)) + context->height / 2);
    draw_square(context, x0, x1, y0, y1, COLOR_GREEN);
}

void draw_square(struct gfx_context_t *context, int x0, int x1, int y0, int y1, uint32_t color)
{
    for (int i = x0; i < x1; i++)
    {
        gfx_putpixel(
            context,
            i,
            y0,
            color);
        gfx_putpixel(
            context,
            i,
            y1,
            color);
    }

    for (int i = y0; i < y1; i++)
    {
        gfx_putpixel(
            context,
            x0,
            i,
            color);
        gfx_putpixel(
            context,
            x1,
            i,
            color);
    }
}

struct gfx_context_t *create_gfx()
{
    struct gfx_context_t *context = gfx_create("Barnes Hut Galaxy Simulation", NX + 1, NY + 1);
    if (!context)
    {
        fprintf(stderr, "Graphic mode initialization failed!\n");
        EXIT_FAILURE;
    }
    return context;
}