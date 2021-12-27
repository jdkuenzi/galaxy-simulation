/*
 * Author   : Küenzi Jean-Daniel
 * Version  : 1.0.0
 * Date     : 30.05.2019
 * Update   : 30.05.2019
 * Desc.    : Entete du fichier quad_tree.c
 */

#ifndef _QUAD_TREE_H_
#define _QUAD_TREE_H_

#include "../galaxy/galaxy.h"

typedef struct __node {
    struct __node* children[4];
    Box b;
    Star* s;
    Star* super_s;
    bool is_empty;
} Node;

typedef struct __quad_tree {
    Node* root;
} Quad_tree;

Quad_tree* create_quad_tree_from_galaxy(const Galaxy *const);
Node* create_new_node(Box b);
void new_create_new_node(Box b, Node* n);
void insert_star(Node*, Star*);
void update_acceleration_from_node(const Node* const, Star*, double);
void update_accelerations_of_all_stars(const Node* const, Galaxy*, double);
void free_quad_tree(Quad_tree*);
void free_node(Node*);
bool is_leaf(const Node* const);

#endif