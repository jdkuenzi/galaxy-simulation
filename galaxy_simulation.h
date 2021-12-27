/*
 * Author   : Küenzi Jean-Daniel
 * Version  : 1.0.0
 * Date     : 30.05.2019
 * Update   : 08.06.2019
 * Desc.    : 
 */

#ifndef _GALAXY_SIMULATION_H_
#define _GALAXY_SIMULATION_H_

#include "./libs/quadtree/quad_tree.h"
#include "./libs/gfx/gfx.h"
#include "quad_tree_thread.h"

#define NX 900
#define NY 900
#define MAXCOLOR 255
#define DT pow(10, 10)

struct gfx_context_t* create_gfx();
void show_pixels(struct gfx_context_t *, Galaxy *);
void draw_quad_tree(struct gfx_context_t*, Node*, bool, bool);
void draw_box(struct gfx_context_t*, Box);
void draw_super_s(struct gfx_context_t*, Star*);
void draw_square(struct gfx_context_t*, int, int, int, int, uint32_t);

#endif