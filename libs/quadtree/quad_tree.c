/*
 * Author   : Küenzi Jean-Daniel
 * Version  : 1.0.0
 * Date     : 30.05.2019
 * Update   : 08.06.2019
 * Desc.    : Fichier contenant les fonctions permettant d'utiliser le quad tree
 */

#include "quad_tree.h"

Quad_tree *create_quad_tree_from_galaxy(const Galaxy *const g)
{
    Quad_tree *new_quad_tree = (Quad_tree *)malloc(sizeof(Quad_tree));
    if (new_quad_tree == NULL)
    {
        printf("Erreur lors de la création du quad tree !");
        exit(0);
    }
    new_quad_tree->root = create_new_node(g->b);
    for (int i = 0; i < g->num_bodies; i++)
    {
        insert_star(new_quad_tree->root, &g->stars[i]);
    }
    return new_quad_tree;
}

Node *create_new_node(Box b)
{
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL)
    {
        printf("Erreur lors de la création du node (root) !");
        exit(0);
    }
    for (int i = 0; i < 4; i++)
    {
        node->children[i] = NULL;
    }
    node->b = b;
    node->s = NULL;
    node->super_s = NULL;
    node->is_empty = true;
    return node;
}

void insert_star(Node *n, Star *s)
{
    if (n != NULL && is_inside(n->b, s->pos_t))
    {
        if (is_leaf(n))
        {
            if (n->is_empty)
            {
                n->s = s;
                n->is_empty = false;
            }
            else
            {
                Box b_4[4];
                divide_in_four(n->b, b_4);
                for (int i = 0; i < 4; i++)
                {
                    n->children[i] = create_new_node(b_4[i]);
                }
                for (int i = 0; i < 4; i++)
                {
                    insert_star(n->children[i], n->s);
                }
                for (int i = 0; i < 4; i++)
                {
                    insert_star(n->children[i], s);
                }
                n->super_s = new_super_star(n->s, s);
                n->is_empty = true;
                n->s = NULL;
            }
        }
        else
        {
            increment_super_star(n->super_s, s);
            for (int i = 0; i < 4; i++)
            {
                insert_star(n->children[i], s);
            }
        }
    }
}

void update_acceleration_from_node(const Node *const n, Star *s, double theta)
{
    if (is_leaf(n))
    {
        if (!n->is_empty && !is_inside(n->b, s->pos_t))
        {
            update_acceleration(s, n->s);
        }
    }
    else if (compute_length(n->b) / distance(&n->super_s->pos_t, &s->pos_t) < theta)
    {
        update_acceleration(s, n->super_s);
    }
    else
    {
        for (int i = 0; i < 4; i++)
        {
            update_acceleration_from_node(n->children[i], s, theta);
        }
    }
}

void update_accelerations_of_all_stars(const Node *const n, Galaxy *g, double theta)
{
    for (int i = 0; i < g->num_bodies; i++)
    {
        update_acceleration_from_node(n, &g->stars[i], theta);
    }
}

void free_quad_tree(Quad_tree *qt)
{
    free_node(qt->root);
    free(qt);
}

void free_node(Node *n)
{
    if (!is_leaf(n))
    {
        for (int i = 0; i < 4; ++i)
        {
            free_node(n->children[i]);
        }
        free(n->super_s);
    }
    free(n);
}

bool is_leaf(const Node *const n)
{
    return (n->children[0] == NULL) ? true : false;
}