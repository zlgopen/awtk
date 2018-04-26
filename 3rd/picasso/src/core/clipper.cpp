/* Picasso - a vector graphics library
 *
 * Copyright (C) 2008 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#include "common.h"
#include "clipper.h"

#include "picasso.h"
#include <float.h>

namespace picasso {

/*
 * GPC  - Version 2.31
 * Author:    Alan Murta (email: gpc@cs.man.ac.uk)
 * Date:      4th June 1999
 * Copyright: (C) 1997-1999, Advanced Interfaces Group,
 *            University of Manchester.
 */

#define LEFT               0
#define RIGHT              1

#define ABOVE              0
#define BELOW              1

#define CLIP               0
#define SUBJ               1

#define EQ(a, b)           (Fabs((a) - (b)) <= FLT_TO_SCALAR(FLT_EPSILON))

#define PREV_INDEX(i, n)   ((i - 1 + n) % n)
#define NEXT_INDEX(i, n)   ((i + 1) % n)

#define OPTIMAL(v, i, n)   ((v[PREV_INDEX(i, n)].y != v[i].y) || \
                            (v[NEXT_INDEX(i, n)].y != v[i].y))

#define FWD_MIN(v, i, n)   ((v[PREV_INDEX(i, n)].vertex.y >= v[i].vertex.y) \
                         && (v[NEXT_INDEX(i, n)].vertex.y > v[i].vertex.y))

#define NOT_FMAX(v, i, n)   (v[NEXT_INDEX(i, n)].vertex.y > v[i].vertex.y)

#define REV_MIN(v, i, n)   ((v[PREV_INDEX(i, n)].vertex.y > v[i].vertex.y) \
                         && (v[NEXT_INDEX(i, n)].vertex.y >= v[i].vertex.y))

#define NOT_RMAX(v, i, n)   (v[PREV_INDEX(i, n)].vertex.y > v[i].vertex.y)

#define P_EDGE(d,e,p,i,j)  {(d)= (e); \
                            do {(d)= (d)->prev;} while (!(d)->outp[(p)]); \
                            (i)= (d)->bot.x + (d)->dx * ((j)-(d)->bot.y);}

#define N_EDGE(d,e,p,i,j)  {(d)= (e); \
                            do {(d)= (d)->next;} while (!(d)->outp[(p)]); \
                            (i)= (d)->bot.x + (d)->dx * ((j)-(d)->bot.y);}


typedef enum {   // Edge intersection classes
    NUL,         // Empty non-intersection
    EMX,         // External maximum
    ELI,         // External left intermediate
    TED,         // Top edge
    ERI,         // External right intermediate
    RED,         // Right edge
    IMM,         // Internal maximum and minimum
    IMN,         // Internal minimum
    EMN,         // External minimum
    EMM,         // External maximum and minimum
    LED,         // Left edge
    ILI,         // Internal left intermediate
    BED,         // Bottom edge
    IRI,         // Internal right intermediate
    IMX,         // Internal maximum
    FUL          // Full non-intersection
} vertex_type;

typedef enum {   // Horizontal edge states
    NH,          // No horizontal edge
    BH,          // Bottom horizontal edge
    TH           // Top horizontal edge
} h_state;

typedef enum {    // Edge bundle state
    UNBUNDLED,    // Isolated edge not within a bundle
    BUNDLE_HEAD,  // Bundle head node
    BUNDLE_TAIL   // Passive bundle tail node
} bundle_state;

typedef struct v_shape {    // Internal vertex list datatype
    scalar x;               // X coordinate component
    scalar y;               // Y coordinate component
    struct v_shape *next;   // Pointer to next vertex in list
} vertex_node;

typedef struct p_shape {    // Internal contour / tristrip type
    int active;             // Active flag / vertex count
    int hole;               // Hole / external contour flag
    vertex_node *v[2];      // Left and right vertex list ptrs
    struct p_shape *next;   // Pointer to next polygon contour
    struct p_shape *proxy;  // Pointer to actual structure used
} polygon_node;

typedef struct edge_shape {
    vertex_s vertex;                // Piggy-backed contour vertex data
    vertex_s bot;                   // Edge lower (x, y) coordinate
    vertex_s top;                   // Edge upper (x, y) coordinate
    scalar xb;                      // Scanbeam bottom x coordinate
    scalar xt;                      // Scanbeam top x coordinate
    scalar dx;                      // Change in x for a unit y increase
    int type;                       // Clip / subject edge flag
    int bundle[2][2];               // Bundle edge flags
    int bside[2];                   // Bundle left / right indicators
    bundle_state bstate[2];         // Edge bundle state
    polygon_node *outp[2];          // Output polygon / tristrip pointer
    struct edge_shape *prev;        // Previous edge in the AET
    struct edge_shape *next;        // Next edge in the AET
    struct edge_shape *pred;        // Edge connected at the lower end
    struct edge_shape *succ;        // Edge connected at the upper end
    struct edge_shape *next_bound;  // Pointer to next bound in LMT
} edge_node;

typedef struct lmt_shape {    // Local minima table
    scalar y;                 // Y coordinate at local minimum
    edge_node *first_bound;   // Pointer to bound list
    struct lmt_shape *next;   // Pointer to next local minimum
} lmt_node;

typedef struct sbt_t_shape {    // Scanbeam tree
    scalar y;                   // Scanbeam node y value
    struct sbt_t_shape *less;   // Pointer to nodes with lower y
    struct sbt_t_shape *more;   // Pointer to nodes with higher y
} sb_tree;

typedef struct it_shape {   // Intersection table
    edge_node *ie[2];       // Intersecting edge (bundle) pair
    vertex_s point;         // Point of intersection
    struct it_shape *next;  // The next intersection table node
} it_node;

typedef struct st_shape {   // Sorted edge table
    edge_node *edge;        // Pointer to AET edge
    scalar xb;              // Scanbeam bottom x coordinate
    scalar xt;              // Scanbeam top x coordinate
    scalar dx;              // Change in x for a unit y increase
    struct st_shape *prev;  // Previous edge in sorted list
} st_node;

typedef struct bbox_shape { // Contour axis-aligned bounding box
    scalar xmin;            // Minimum x coordinate
    scalar ymin;            // Minimum y coordinate
    scalar xmax;            // Maximum x coordinate
    scalar ymax;            // Maximum y coordinate
} bbox;


// Horizontal edge state transitions within scanbeam boundary
const h_state next_h_state[3][6] = {
  /*        ABOVE     BELOW     CROSS */
  /*        L   R     L   R     L   R */
  /* NH */ {BH, TH,   TH, BH,   NH, NH},
  /* BH */ {NH, NH,   NH, NH,   TH, TH},
  /* TH */ {NH, NH,   NH, NH,   BH, BH}
};


static void reset_it(it_node **it)
{
    it_node *itn;

    while (*it) {
        itn = (*it)->next;
        mem_free(*it);
        *it = itn;
    }
}


static void reset_lmt(lmt_node **lmt)
{
    lmt_node *lmtn;

    while (*lmt) {
        lmtn = (*lmt)->next;
        mem_free(*lmt);
        *lmt = lmtn;
    }
}


static void insert_bound(edge_node **b, edge_node *e)
{
    edge_node *existing_bound;

    if (!*b) {
        // Link node e to the tail of the list
        *b = e;
    } else {
        // Do primary sort on the x field
        if (e[0].bot.x < (*b)[0].bot.x) {
            // Insert a new node mid-list
            existing_bound = *b;
            *b = e;
            (*b)->next_bound = existing_bound;
        } else {
            if (e[0].bot.x == (*b)[0].bot.x) {
                // Do secondary sort on the dx field
                if (e[0].dx < (*b)[0].dx) {
                    // Insert a new node mid-list
                    existing_bound = *b;
                    *b = e;
                    (*b)->next_bound = existing_bound;
                } else {
                    // Head further down the list
                    insert_bound(&((*b)->next_bound), e);
                }
            } else {
                // Head further down the list
                insert_bound(&((*b)->next_bound), e);
            }
        }
    }
}


static edge_node **bound_list(lmt_node **lmt, scalar y)
{
    lmt_node *existing_node;

    if (!*lmt) {
        // Add node onto the tail end of the LMT
        (*lmt) = (lmt_node*)mem_malloc(sizeof(lmt_node));
        (*lmt)->y = y;
        (*lmt)->first_bound = NULL;
        (*lmt)->next = NULL;
        return &((*lmt)->first_bound);
    } else {
        if (y < (*lmt)->y) {
            // Insert a new LMT node before the current node
            existing_node = *lmt;
            (*lmt) = (lmt_node*)mem_malloc(sizeof(lmt_node));
            (*lmt)->y = y;
            (*lmt)->first_bound = NULL;
            (*lmt)->next= existing_node;
            return &((*lmt)->first_bound);
        } else {
            if (y > (*lmt)->y) {
                // Head further up the LMT
                return bound_list(&((*lmt)->next), y);
            } else {
                // Use this existing LMT node
                return &((*lmt)->first_bound);
            }
        }
    }
}


static void add_to_sbtree(int *entries, sb_tree **sbtree, scalar y)
{
    if (!*sbtree) {
        // Add a new tree node here
        (*sbtree) = (sb_tree*)mem_malloc(sizeof(sb_tree));
        (*sbtree)->y = y;
        (*sbtree)->less = NULL;
        (*sbtree)->more = NULL;
        (*entries)++;
    } else {
        if ((*sbtree)->y > y) {
            // Head into the 'less' sub-tree
            add_to_sbtree(entries, &((*sbtree)->less), y);
        } else {
            if ((*sbtree)->y < y) {
                // Head into the 'more' sub-tree
                add_to_sbtree(entries, &((*sbtree)->more), y);
            }
        }
    }
}


static void build_sbt(int *entries, scalar *sbt, sb_tree *sbtree)
{
    if (sbtree->less) {
        build_sbt(entries, sbt, sbtree->less);
    }
    sbt[*entries]= sbtree->y;
    (*entries)++;
    if (sbtree->more) {
        build_sbt(entries, sbt, sbtree->more);
    }
}


static void free_sbtree(sb_tree **sbtree)
{
    if (*sbtree) {
        free_sbtree(&((*sbtree)->less));
        free_sbtree(&((*sbtree)->more));
        mem_free(*sbtree);
    }
}


static int count_optimal_vertices(vertex_list c)
{
    int result = 0;
    // Ignore non-contributing contours
    if (c.num_vertices > 0) {
        for (int i = 0; i < c.num_vertices; i++) {
            // Ignore superfluous vertices embedded in horizontal edges
            if (OPTIMAL(c.vertex, i, c.num_vertices))
                result++;
        }
    }
    return result;
}


static edge_node *build_lmt(lmt_node **lmt, sb_tree **sbtree,
                            int *sbt_entries, polygon *p, int type, polygon_op op)
{
    int c, i, min, max, num_edges, v, num_vertices;
    int total_vertices = 0, e_index = 0;
    edge_node *e, *edge_table;

    for (c = 0; c < p->num_contours; c++)
        total_vertices+= count_optimal_vertices(p->contour[c]);

    // Create the entire input polygon edge table in one go
    edge_table = (edge_node*)mem_malloc(total_vertices * sizeof(edge_node));

    for (c = 0; c < p->num_contours; c++) {
        if (p->contour[c].num_vertices < 0) {
            // Ignore the non-contributing contour and repair the vertex count
            p->contour[c].num_vertices = -(p->contour[c].num_vertices);
        } else {
            // Perform contour optimisation
            num_vertices = 0;
            for (i = 0; i < p->contour[c].num_vertices; i++) {
                if (OPTIMAL(p->contour[c].vertex, i, p->contour[c].num_vertices)) {
                    edge_table[num_vertices].vertex.x = p->contour[c].vertex[i].x;
                    edge_table[num_vertices].vertex.y = p->contour[c].vertex[i].y;

                    // Record vertex in the scanbeam table
                    add_to_sbtree(sbt_entries, sbtree, edge_table[num_vertices].vertex.y);
                    num_vertices++;
                }
            }

            // Do the contour forward pass
            for (min = 0; min < num_vertices; min++) {
                // If a forward local minimum...
                if (FWD_MIN(edge_table, min, num_vertices)) {
                    // Search for the next local maximum...
                    num_edges= 1;
                    max = NEXT_INDEX(min, num_vertices);
                    while (NOT_FMAX(edge_table, max, num_vertices)) {
                        num_edges++;
                        max = NEXT_INDEX(max, num_vertices);
                    }

                    // Build the next edge list
                    e = &edge_table[e_index];
                    e_index += num_edges;
                    v = min;
                    e[0].bstate[BELOW]= UNBUNDLED;
                    e[0].bundle[BELOW][CLIP]= False;
                    e[0].bundle[BELOW][SUBJ]= False;

                    for (i = 0; i < num_edges; i++) {
                        e[i].xb= edge_table[v].vertex.x;
                        e[i].bot.x= edge_table[v].vertex.x;
                        e[i].bot.y= edge_table[v].vertex.y;

                        v = NEXT_INDEX(v, num_vertices);

                        e[i].top.x = edge_table[v].vertex.x;
                        e[i].top.y = edge_table[v].vertex.y;
                        e[i].dx = (edge_table[v].vertex.x - e[i].bot.x) /
                            (e[i].top.y - e[i].bot.y);
                        e[i].type = type;
                        e[i].outp[ABOVE] = NULL;
                        e[i].outp[BELOW] = NULL;
                        e[i].next = NULL;
                        e[i].prev = NULL;
                        e[i].succ = ((num_edges > 1) && (i < (num_edges - 1))) ?
                            &(e[i + 1]) : NULL;
                        e[i].pred = ((num_edges > 1) && (i > 0)) ? &(e[i - 1]) : NULL;
                        e[i].next_bound = NULL;
                        e[i].bside[CLIP] = (op == POLY_DIFF) ? RIGHT : LEFT;
                        e[i].bside[SUBJ] = LEFT;
                    }
                    insert_bound(bound_list(lmt, edge_table[min].vertex.y), e);
                }
            }

            // Do the contour reverse pass
            for (min = 0; min < num_vertices; min++) {
                // If a reverse local minimum...
                if (REV_MIN(edge_table, min, num_vertices)) {
                    // Search for the previous local maximum...
                    num_edges = 1;
                    max = PREV_INDEX(min, num_vertices);
                    while (NOT_RMAX(edge_table, max, num_vertices)) {
                        num_edges++;
                        max = PREV_INDEX(max, num_vertices);
                    }

                    // Build the previous edge list
                    e = &edge_table[e_index];
                    e_index += num_edges;
                    v = min;
                    e[0].bstate[BELOW] = UNBUNDLED;
                    e[0].bundle[BELOW][CLIP] = False;
                    e[0].bundle[BELOW][SUBJ] = False;

                    for (i = 0; i < num_edges; i++) {
                        e[i].xb = edge_table[v].vertex.x;
                        e[i].bot.x = edge_table[v].vertex.x;
                        e[i].bot.y = edge_table[v].vertex.y;

                        v = PREV_INDEX(v, num_vertices);

                        e[i].top.x = edge_table[v].vertex.x;
                        e[i].top.y = edge_table[v].vertex.y;
                        e[i].dx = (edge_table[v].vertex.x - e[i].bot.x) /
                            (e[i].top.y - e[i].bot.y);
                        e[i].type = type;
                        e[i].outp[ABOVE] = NULL;
                        e[i].outp[BELOW] = NULL;
                        e[i].next = NULL;
                        e[i].prev = NULL;
                        e[i].succ = ((num_edges > 1) && (i < (num_edges - 1))) ?
                            &(e[i + 1]) : NULL;
                        e[i].pred = ((num_edges > 1) && (i > 0)) ? &(e[i - 1]) : NULL;
                        e[i].next_bound = NULL;
                        e[i].bside[CLIP] = (op == POLY_DIFF) ? RIGHT : LEFT;
                        e[i].bside[SUBJ] = LEFT;
                    }
                    insert_bound(bound_list(lmt, edge_table[min].vertex.y), e);
                }
            }
        }
    }
    return edge_table;
}


static void add_edge_to_aet(edge_node **aet, edge_node *edge, edge_node *prev)
{
    if (!*aet) {
        // Append edge onto the tail end of the AET
        *aet = edge;
        edge->prev = prev;
        edge->next = NULL;
    } else {
        // Do primary sort on the xb field
        if (edge->xb < (*aet)->xb) {
            // Insert edge here (before the AET edge)
            edge->prev = prev;
            edge->next = *aet;
            (*aet)->prev = edge;
            *aet = edge;
        } else {
            if (edge->xb == (*aet)->xb) {
                // Do secondary sort on the dx field
                if (edge->dx < (*aet)->dx) {
                    // Insert edge here (before the AET edge)
                    edge->prev = prev;
                    edge->next = *aet;
                    (*aet)->prev = edge;
                    *aet = edge;
                } else {
                    // Head further into the AET
                    add_edge_to_aet(&((*aet)->next), edge, *aet);
                }
            } else {
                // Head further into the AET
                add_edge_to_aet(&((*aet)->next), edge, *aet);
            }
        }
    }
}


static void add_intersection(it_node **it, edge_node *edge0,
                                edge_node *edge1, scalar x, scalar y)
{
    it_node *existing_node;

    if (!*it) {
        // Append a new node to the tail of the list
        (*it) = (it_node*)mem_malloc(sizeof(it_node));
        (*it)->ie[0] = edge0;
        (*it)->ie[1] = edge1;
        (*it)->point.x = x;
        (*it)->point.y = y;
        (*it)->next = NULL;
    } else {
        if ((*it)->point.y > y) {
            // Insert a new node mid-list
            existing_node= *it;
            (*it) = (it_node*)mem_malloc(sizeof(it_node));
            (*it)->ie[0] = edge0;
            (*it)->ie[1] = edge1;
            (*it)->point.x = x;
            (*it)->point.y = y;
            (*it)->next = existing_node;
        } else {
            // Head further down the list
            add_intersection(&((*it)->next), edge0, edge1, x, y);
        }
    }
}


static void add_st_edge(st_node **st, it_node **it, edge_node *edge, scalar dy)
{
    st_node *existing_node;
    scalar den, r, x, y;

    if (!*st) {
        // Append edge onto the tail end of the ST
        (*st) = (st_node*)mem_malloc(sizeof(st_node));
        (*st)->edge = edge;
        (*st)->xb = edge->xb;
        (*st)->xt = edge->xt;
        (*st)->dx = edge->dx;
        (*st)->prev = NULL;
    } else {
        den = ((*st)->xt - (*st)->xb) - (edge->xt - edge->xb);

        // If new edge and ST edge don't cross
        if ((edge->xt >= (*st)->xt) || (edge->dx == (*st)->dx) ||
                (Fabs(den) <= FLT_EPSILON))
        {
            // No intersection - insert edge here (before the ST edge)
            existing_node = *st;
            (*st) = (st_node*)mem_malloc(sizeof(st_node));
            (*st)->edge = edge;
            (*st)->xb = edge->xb;
            (*st)->xt = edge->xt;
            (*st)->dx = edge->dx;
            (*st)->prev = existing_node;
        } else {
            // Compute intersection between new edge and ST edge
            r = (edge->xb - (*st)->xb) / den;
            x = (*st)->xb + r * ((*st)->xt - (*st)->xb);
            y = r * dy;

            // Insert the edge pointers and the intersection point in the IT
            add_intersection(it, (*st)->edge, edge, x, y);

            // Head further into the ST
            add_st_edge(&((*st)->prev), it, edge, dy);
        }
    }
}


static void build_intersection_table(it_node **it, edge_node *aet, scalar dy)
{
    st_node *st = NULL;
    // Build intersection table for the current scanbeam
    reset_it(it);

    // Process each AET edge
    for (edge_node *edge = aet; edge; edge = edge->next) {
        if ((edge->bstate[ABOVE] == BUNDLE_HEAD) ||
                edge->bundle[ABOVE][CLIP] || edge->bundle[ABOVE][SUBJ])
            add_st_edge(&st, it, edge, dy);
    }

    st_node *stp = NULL;
    // Free the sorted edge table
    while (st) {
        stp = st->prev;
        mem_free(st);
        st = stp;
    }
}

static int count_contours(polygon_node *polygon)
{
    int nc, nv;
    vertex_node *v, *nextv;

    for (nc = 0; polygon; polygon = polygon->next) {
        if (polygon->active) {
            // Count the vertices in the current contour
            nv = 0;
            for (v = polygon->proxy->v[LEFT]; v; v = v->next)
                nv++;

            // Record valid vertex counts in the active field
            if (nv > 2) {
                polygon->active = nv;
                nc++;
            } else {
                // Invalid contour: just free the heap
                for (v = polygon->proxy->v[LEFT]; v; v = nextv) {
                    nextv = v->next;
                    mem_free(v);
                }
                polygon->active = 0;
            }
        }
    }
    return nc;
}


static void add_left(polygon_node *p, scalar x, scalar y)
{
    // Create a new vertex node and set its fields
    vertex_node *nv = (vertex_node*)mem_malloc(sizeof(vertex_node));
    nv->x = x; nv->y = y;

    // Add vertex nv to the left end of the polygon's vertex list
    nv->next = p->proxy->v[LEFT];

    // Update proxy->[LEFT] to point to nv
    p->proxy->v[LEFT] = nv;
}


static void merge_left(polygon_node *p, polygon_node *q, polygon_node *list)
{
    polygon_node *target;

    // Label contour as a hole
    q->proxy->hole = True;

    if (p->proxy != q->proxy) {
        // Assign p's vertex list to the left end of q's list
        p->proxy->v[RIGHT]->next = q->proxy->v[LEFT];
        q->proxy->v[LEFT] = p->proxy->v[LEFT];

        // Redirect any p->proxy references to q->proxy
        for (target = p->proxy; list; list = list->next) {
            if (list->proxy == target) {
                list->active = False;
                list->proxy = q->proxy;
            }
        }
    }
}


static void add_right(polygon_node *p, scalar x, scalar y)
{
    // Create a new vertex node and set its fields
    vertex_node *nv = (vertex_node*)mem_malloc(sizeof(vertex_node));
    nv->x= x; nv->y= y; nv->next= NULL;

    // Add vertex nv to the right end of the polygon's vertex list
    p->proxy->v[RIGHT]->next= nv;

    // Update proxy->v[RIGHT] to point to nv
    p->proxy->v[RIGHT]= nv;
}


static void merge_right(polygon_node *p, polygon_node *q, polygon_node *list)
{
    // Label contour as external
    q->proxy->hole= False;

    if (p->proxy != q->proxy) {
        // Assign p's vertex list to the right end of q's list
        q->proxy->v[RIGHT]->next= p->proxy->v[LEFT];
        q->proxy->v[RIGHT]= p->proxy->v[RIGHT];

        // Redirect any p->proxy references to q->proxy
        for (polygon_node *target = p->proxy; list; list = list->next) {
            if (list->proxy == target) {
                list->active = False;
                list->proxy = q->proxy;
            }
        }
    }
}


static void add_local_min(polygon_node **p, edge_node *edge, scalar x, scalar y)
{
    polygon_node *existing_min = *p;

    (*p) = (polygon_node*)mem_malloc(sizeof(polygon_node));

    // Create a new vertex node and set its fields
    vertex_node *nv = (vertex_node*)mem_malloc(sizeof(vertex_node));
    nv->x = x; nv->y = y; nv->next = NULL;

    // Initialise proxy to point to p itself
    (*p)->proxy = (*p);
    (*p)->active = True;
    (*p)->next = existing_min;

    // Make v[LEFT] and v[RIGHT] point to new vertex nv
    (*p)->v[LEFT] = nv;
    (*p)->v[RIGHT] = nv;

    // Assign polygon p to the edge
    edge->outp[ABOVE] = *p;
}

static bbox *create_contour_bboxes(polygon *p)
{
    bbox *box = (bbox*)mem_malloc(p->num_contours * sizeof(bbox));

    // Construct contour bounding boxes
    for (int c = 0; c < p->num_contours; c++) {
        // Initialise bounding box extent
        box[c].xmin = FLT_MAX;
        box[c].ymin = FLT_MAX;
        box[c].xmax = -FLT_MAX;
        box[c].ymax = -FLT_MAX;

        for (int v = 0; v < p->contour[c].num_vertices; v++) {
            // Adjust bounding box
            if (p->contour[c].vertex[v].x < box[c].xmin)
                box[c].xmin = p->contour[c].vertex[v].x;

            if (p->contour[c].vertex[v].y < box[c].ymin)
                box[c].ymin = p->contour[c].vertex[v].y;

            if (p->contour[c].vertex[v].x > box[c].xmax)
                box[c].xmax = p->contour[c].vertex[v].x;

            if (p->contour[c].vertex[v].y > box[c].ymax)
                box[c].ymax = p->contour[c].vertex[v].y;
        }
    }
    return box;
}


static void minimax_test(polygon *subj, polygon *clip, polygon_op op)
{
    bbox *s_bbox, *c_bbox;
    int s, c, *o_table, overlap;

    s_bbox = create_contour_bboxes(subj);
    c_bbox = create_contour_bboxes(clip);

    o_table = (int*)mem_malloc(subj->num_contours * clip->num_contours * sizeof(int));

    // Check all subject contour bounding boxes against clip boxes
    for (s = 0; s < subj->num_contours; s++) {
        for (c = 0; c < clip->num_contours; c++) {
            o_table[c * subj->num_contours + s] =
                (!((s_bbox[s].xmax < c_bbox[c].xmin) ||
                   (s_bbox[s].xmin > c_bbox[c].xmax))) &&
                (!((s_bbox[s].ymax < c_bbox[c].ymin) ||
                   (s_bbox[s].ymin > c_bbox[c].ymax)));
        }
    }

    // For each clip contour, search for any subject contour overlaps
    for (c = 0; c < clip->num_contours; c++) {
        overlap = 0;
        for (s = 0; (!overlap) && (s < subj->num_contours); s++) {
            overlap = o_table[c * subj->num_contours + s];
        }

        if (!overlap) // Flag non contributing status by negating vertex count
            clip->contour[c].num_vertices = -clip->contour[c].num_vertices;
    }

    if (op == POLY_INTERSECT) {
        // For each subject contour, search for any clip contour overlaps
        for (s = 0; s < subj->num_contours; s++) {
            overlap = 0;
            for (c = 0; (!overlap) && (c < clip->num_contours); c++) {
                overlap= o_table[c * subj->num_contours + s];
            }

            if (!overlap) // Flag non contributing status by negating vertex count
                subj->contour[s].num_vertices = -subj->contour[s].num_vertices;
        }
    }

    mem_free(s_bbox);
    mem_free(c_bbox);
    mem_free(o_table);
}


void free_polygon(polygon *p)
{
    for (int c = 0; c < p->num_contours; c++) {
        mem_free(p->contour[c].vertex);
    }
    mem_free(p->contour);
    p->num_contours= 0;
}


void polygon_clip(polygon_op op, polygon *subj, polygon *clip, polygon *result)
{
    sb_tree *sbtree = NULL;
    it_node *it = NULL, *intersect;
    edge_node *edge, *prev_edge, *next_edge, *succ_edge, *e0, *e1;
    edge_node *aet = NULL, *c_heap = NULL, *s_heap = NULL;
    lmt_node *lmt = NULL, *local_min;
    polygon_node *out_poly = NULL, *p, *q, *poly, *npoly, *cf = NULL;
    vertex_node *vtx, *nv;
    h_state horiz[2];
    int in[2], exists[2], parity[2] = {LEFT, LEFT};
    int c, v, contributing = 0, search, scanbeam = 0, sbt_entries = 0;
    int vclass, bl = 0, br = 0, tl = 0, tr = 0;
    scalar *sbt = NULL, xb, px, yb, yt = 0, dy = 0, ix, iy;

    // Test for trivial NULL result cases
    if (((subj->num_contours == 0) && (clip->num_contours == 0))
            || ((subj->num_contours == 0) && ((op == POLY_INTERSECT) || (op == POLY_DIFF)))
            || ((clip->num_contours == 0) &&  (op == POLY_INTERSECT)))
    {
        result->num_contours = 0;
        result->contour = NULL;
        return;
    }

    // Identify potentialy contributing contours
    if (((op == POLY_INTERSECT) || (op == POLY_DIFF))
            && (subj->num_contours > 0) && (clip->num_contours > 0))
        minimax_test(subj, clip, op);

    // Build LMT
    if (subj->num_contours > 0)
        s_heap = build_lmt(&lmt, &sbtree, &sbt_entries, subj, SUBJ, op);
    if (clip->num_contours > 0)
        c_heap = build_lmt(&lmt, &sbtree, &sbt_entries, clip, CLIP, op);

    // Return a NULL result if no contours contribute
    if (lmt == NULL) {
        result->num_contours = 0;
        result->contour = NULL;
        reset_lmt(&lmt);
        mem_free(s_heap);
        mem_free(c_heap);
        return;
    }

    // Build scanbeam table from scanbeam tree
    sbt = (float*)mem_malloc(sbt_entries * sizeof(float));
    build_sbt(&scanbeam, sbt, sbtree);
    scanbeam = 0;
    free_sbtree(&sbtree);

    // Allow pointer re-use without causing memory leak
    if (subj == result)
        free_polygon(subj);
    if (clip == result)
        free_polygon(clip);

    // Invert clip polygon for difference operation
    if (op == POLY_DIFF)
        parity[CLIP] = RIGHT;

    local_min = lmt;

    // Process each scanbeam */
    while (scanbeam < sbt_entries) {
        // Set yb and yt to the bottom and top of the scanbeam
        yb = sbt[scanbeam++];
        if (scanbeam < sbt_entries) {
            yt = sbt[scanbeam];
            dy = yt - yb;
        }

        // If LMT node corresponding to yb exists
        if (local_min) {
            if (local_min->y == yb) {
                // Add edges starting at this local minimum to the AET
                for (edge = local_min->first_bound; edge; edge = edge->next_bound)
                    add_edge_to_aet(&aet, edge, NULL);

                local_min = local_min->next;
            }
        }

        // Set dummy previous x value
        px = -FLT_MAX;

        // Create bundles within AET
        e0 = aet;
        e1 = aet;

        // Set up bundle fields of first edge
        aet->bundle[ABOVE][aet->type] = (aet->top.y != yb);
        aet->bundle[ABOVE][!aet->type] = False;
        aet->bstate[ABOVE] = UNBUNDLED;

        for (next_edge = aet->next; next_edge; next_edge = next_edge->next) {
            // Set up bundle fields of next edge
            next_edge->bundle[ABOVE][next_edge->type] = (next_edge->top.y != yb);
            next_edge->bundle[ABOVE][!next_edge->type] = False;
            next_edge->bstate[ABOVE] = UNBUNDLED;

            // Bundle edges above the scanbeam boundary if they coincide
            if (next_edge->bundle[ABOVE][next_edge->type]) {
                if (EQ(e0->xb, next_edge->xb) && EQ(e0->dx, next_edge->dx)
                        && (e0->top.y != yb))
                {
                    next_edge->bundle[ABOVE][ next_edge->type] ^=
                        e0->bundle[ABOVE][ next_edge->type];
                    next_edge->bundle[ABOVE][!next_edge->type] =
                        e0->bundle[ABOVE][!next_edge->type];
                    next_edge->bstate[ABOVE] = BUNDLE_HEAD;
                    e0->bundle[ABOVE][CLIP] = False;
                    e0->bundle[ABOVE][SUBJ] = False;
                    e0->bstate[ABOVE] = BUNDLE_TAIL;
                }
                e0 = next_edge;
            }
        }

        horiz[CLIP]= NH;
        horiz[SUBJ]= NH;

        // Process each edge at this scanbeam boundary
        for (edge = aet; edge; edge = edge->next) {
            exists[CLIP]= edge->bundle[ABOVE][CLIP] +
                (edge->bundle[BELOW][CLIP] << 1);
            exists[SUBJ]= edge->bundle[ABOVE][SUBJ] +
                (edge->bundle[BELOW][SUBJ] << 1);

            if (exists[CLIP] || exists[SUBJ]) {
                // Set bundle side
                edge->bside[CLIP]= parity[CLIP];
                edge->bside[SUBJ]= parity[SUBJ];

                // Determine contributing status and quadrant occupancies
                switch (op) {
                    case POLY_DIFF:
                    case POLY_INTERSECT:
                        contributing = (exists[CLIP] && (parity[SUBJ] || horiz[SUBJ]))
                            || (exists[SUBJ] && (parity[CLIP] || horiz[CLIP]))
                            || (exists[CLIP] && exists[SUBJ]
                                    && (parity[CLIP] == parity[SUBJ]));
                        br = (parity[CLIP])
                            && (parity[SUBJ]);
                        bl = (parity[CLIP] ^ edge->bundle[ABOVE][CLIP])
                            && (parity[SUBJ] ^ edge->bundle[ABOVE][SUBJ]);
                        tr = (parity[CLIP] ^ (horiz[CLIP] != NH))
                            && (parity[SUBJ] ^ (horiz[SUBJ] != NH));
                        tl = (parity[CLIP] ^ (horiz[CLIP] != NH) ^ edge->bundle[BELOW][CLIP])
                            && (parity[SUBJ] ^ (horiz[SUBJ] != NH) ^ edge->bundle[BELOW][SUBJ]);
                        break;
                    case POLY_XOR:
                        contributing= exists[CLIP] || exists[SUBJ];
                        br = (parity[CLIP])
                            ^ (parity[SUBJ]);
                        bl = (parity[CLIP] ^ edge->bundle[ABOVE][CLIP])
                            ^ (parity[SUBJ] ^ edge->bundle[ABOVE][SUBJ]);
                        tr = (parity[CLIP] ^ (horiz[CLIP] != NH))
                            ^ (parity[SUBJ] ^ (horiz[SUBJ] != NH));
                        tl = (parity[CLIP] ^ (horiz[CLIP] != NH) ^ edge->bundle[BELOW][CLIP])
                            ^ (parity[SUBJ] ^ (horiz[SUBJ] != NH) ^ edge->bundle[BELOW][SUBJ]);
                        break;
                    case POLY_UNION:
                        contributing= (exists[CLIP] && (!parity[SUBJ] || horiz[SUBJ]))
                            || (exists[SUBJ] && (!parity[CLIP] || horiz[CLIP]))
                            || (exists[CLIP] && exists[SUBJ]
                                    && (parity[CLIP] == parity[SUBJ]));
                        br = (parity[CLIP])
                            || (parity[SUBJ]);
                        bl = (parity[CLIP] ^ edge->bundle[ABOVE][CLIP])
                            || (parity[SUBJ] ^ edge->bundle[ABOVE][SUBJ]);
                        tr = (parity[CLIP] ^ (horiz[CLIP] != NH))
                            || (parity[SUBJ] ^ (horiz[SUBJ] != NH));
                        tl = (parity[CLIP] ^ (horiz[CLIP] != NH) ^ edge->bundle[BELOW][CLIP])
                            || (parity[SUBJ] ^ (horiz[SUBJ] != NH) ^ edge->bundle[BELOW][SUBJ]);
                        break;
                }

                // Update parity
                parity[CLIP] ^= edge->bundle[ABOVE][CLIP];
                parity[SUBJ] ^= edge->bundle[ABOVE][SUBJ];

                // Update horizontal state
                if (exists[CLIP])
                    horiz[CLIP] = next_h_state[horiz[CLIP]]
                        [((exists[CLIP] - 1) << 1) + parity[CLIP]];

                if (exists[SUBJ])
                    horiz[SUBJ] = next_h_state[horiz[SUBJ]]
                        [((exists[SUBJ] - 1) << 1) + parity[SUBJ]];

                vclass = tr + (tl << 1) + (br << 2) + (bl << 3);

                if (contributing) {
                    xb = edge->xb;

                    switch (vclass) {
                        case EMN:
                        case IMN:
                            add_local_min(&out_poly, edge, xb, yb);
                            px = xb;
                            cf = edge->outp[ABOVE];
                            break;
                        case ERI:
                            if (xb != px) {
                                add_right(cf, xb, yb);
                                px = xb;
                            }
                            edge->outp[ABOVE]= cf;
                            cf = NULL;
                            break;
                        case ELI:
                            add_left(edge->outp[BELOW], xb, yb);
                            px = xb;
                            cf = edge->outp[BELOW];
                            break;
                        case EMX:
                            if (xb != px) {
                                add_left(cf, xb, yb);
                                px = xb;
                            }
                            merge_right(cf, edge->outp[BELOW], out_poly);
                            cf = NULL;
                            break;
                        case ILI:
                            if (xb != px) {
                                add_left(cf, xb, yb);
                                px = xb;
                            }
                            edge->outp[ABOVE] = cf;
                            cf = NULL;
                            break;
                        case IRI:
                            add_right(edge->outp[BELOW], xb, yb);
                            px = xb;
                            cf = edge->outp[BELOW];
                            edge->outp[BELOW] = NULL;
                            break;
                        case IMX:
                            if (xb != px) {
                                add_right(cf, xb, yb);
                                px = xb;
                            }
                            merge_left(cf, edge->outp[BELOW], out_poly);
                            cf = NULL;
                            edge->outp[BELOW] = NULL;
                            break;
                        case IMM:
                            if (xb != px) {
                                add_right(cf, xb, yb);
                                px = xb;
                            }
                            merge_left(cf, edge->outp[BELOW], out_poly);
                            edge->outp[BELOW] = NULL;
                            add_local_min(&out_poly, edge, xb, yb);
                            cf = edge->outp[ABOVE];
                            break;
                        case EMM:
                            if (xb != px) {
                                add_left(cf, xb, yb);
                                px = xb;
                            }
                            merge_right(cf, edge->outp[BELOW], out_poly);
                            edge->outp[BELOW] = NULL;
                            add_local_min(&out_poly, edge, xb, yb);
                            cf = edge->outp[ABOVE];
                            break;
                        case LED:
                            if (edge->bot.y == yb)
                                add_left(edge->outp[BELOW], xb, yb);
                            edge->outp[ABOVE] = edge->outp[BELOW];
                            px = xb;
                            break;
                        case RED:
                            if (edge->bot.y == yb)
                                add_right(edge->outp[BELOW], xb, yb);
                            edge->outp[ABOVE] = edge->outp[BELOW];
                            px = xb;
                            break;
                        default:
                            break;
                    } // End of switch
                } // End of contributing conditional
            } // End of edge exists conditional
        } // End of AET loop

        // Delete terminating edges from the AET, otherwise compute xt
        for (edge = aet; edge; edge= edge->next) {
            if (edge->top.y == yb) {
                prev_edge = edge->prev;
                next_edge = edge->next;

                if (prev_edge)
                    prev_edge->next = next_edge;
                else
                    aet = next_edge;

                if (next_edge)
                    next_edge->prev = prev_edge;

                // Copy bundle head state to the adjacent tail edge if required
                if ((edge->bstate[BELOW] == BUNDLE_HEAD) && prev_edge) {
                    if (prev_edge->bstate[BELOW] == BUNDLE_TAIL) {
                        prev_edge->outp[BELOW]= edge->outp[BELOW];
                        prev_edge->bstate[BELOW]= UNBUNDLED;
                        if (prev_edge->prev) {
                            if (prev_edge->prev->bstate[BELOW] == BUNDLE_TAIL)
                                prev_edge->bstate[BELOW]= BUNDLE_HEAD;
                        }
                    }
                }
            } else {
                if (edge->top.y == yt)
                    edge->xt = edge->top.x;
                else
                    edge->xt = edge->bot.x + edge->dx * (yt - edge->bot.y);
            }
        }

        if (scanbeam < sbt_entries) {
            build_intersection_table(&it, aet, dy);

            // Process each node in the intersection table
            for (intersect = it; intersect; intersect = intersect->next) {
                e0 = intersect->ie[0];
                e1 = intersect->ie[1];

                // Only generate output for contributing intersections
                if ((e0->bundle[ABOVE][CLIP] || e0->bundle[ABOVE][SUBJ])
                        && (e1->bundle[ABOVE][CLIP] || e1->bundle[ABOVE][SUBJ]))
                {
                    p = e0->outp[ABOVE];
                    q = e1->outp[ABOVE];
                    ix = intersect->point.x;
                    iy = intersect->point.y + yb;

                    in[CLIP] = ( e0->bundle[ABOVE][CLIP] && !e0->bside[CLIP])
                        || ( e1->bundle[ABOVE][CLIP] &&  e1->bside[CLIP])
                        || (!e0->bundle[ABOVE][CLIP] && !e1->bundle[ABOVE][CLIP]
                                && e0->bside[CLIP] && e1->bside[CLIP]);
                    in[SUBJ] = ( e0->bundle[ABOVE][SUBJ] && !e0->bside[SUBJ])
                        || ( e1->bundle[ABOVE][SUBJ] &&  e1->bside[SUBJ])
                        || (!e0->bundle[ABOVE][SUBJ] && !e1->bundle[ABOVE][SUBJ]
                                && e0->bside[SUBJ] && e1->bside[SUBJ]);

                    /* Determine quadrant occupancies */
                    switch (op)
                    {
                        case POLY_DIFF:
                        case POLY_INTERSECT:
                            tr = (in[CLIP])
                                && (in[SUBJ]);
                            tl = (in[CLIP] ^ e1->bundle[ABOVE][CLIP])
                                && (in[SUBJ] ^ e1->bundle[ABOVE][SUBJ]);
                            br = (in[CLIP] ^ e0->bundle[ABOVE][CLIP])
                                && (in[SUBJ] ^ e0->bundle[ABOVE][SUBJ]);
                            bl = (in[CLIP] ^ e1->bundle[ABOVE][CLIP] ^ e0->bundle[ABOVE][CLIP])
                                && (in[SUBJ] ^ e1->bundle[ABOVE][SUBJ] ^ e0->bundle[ABOVE][SUBJ]);
                            break;
                        case POLY_XOR:
                            tr = (in[CLIP])
                                ^ (in[SUBJ]);
                            tl = (in[CLIP] ^ e1->bundle[ABOVE][CLIP])
                                ^ (in[SUBJ] ^ e1->bundle[ABOVE][SUBJ]);
                            br = (in[CLIP] ^ e0->bundle[ABOVE][CLIP])
                                ^ (in[SUBJ] ^ e0->bundle[ABOVE][SUBJ]);
                            bl = (in[CLIP] ^ e1->bundle[ABOVE][CLIP] ^ e0->bundle[ABOVE][CLIP])
                                ^ (in[SUBJ] ^ e1->bundle[ABOVE][SUBJ] ^ e0->bundle[ABOVE][SUBJ]);
                            break;
                        case POLY_UNION:
                            tr = (in[CLIP])
                                || (in[SUBJ]);
                            tl = (in[CLIP] ^ e1->bundle[ABOVE][CLIP])
                                || (in[SUBJ] ^ e1->bundle[ABOVE][SUBJ]);
                            br = (in[CLIP] ^ e0->bundle[ABOVE][CLIP])
                                || (in[SUBJ] ^ e0->bundle[ABOVE][SUBJ]);
                            bl = (in[CLIP] ^ e1->bundle[ABOVE][CLIP] ^ e0->bundle[ABOVE][CLIP])
                                || (in[SUBJ] ^ e1->bundle[ABOVE][SUBJ] ^ e0->bundle[ABOVE][SUBJ]);
                            break;
                    }

                    vclass = tr + (tl << 1) + (br << 2) + (bl << 3);

                    switch (vclass) {
                        case EMN:
                            add_local_min(&out_poly, e0, ix, iy);
                            e1->outp[ABOVE] = e0->outp[ABOVE];
                            break;
                        case ERI:
                            if (p) {
                                add_right(p, ix, iy);
                                e1->outp[ABOVE] = p;
                                e0->outp[ABOVE] = NULL;
                            }
                            break;
                        case ELI:
                            if (q) {
                                add_left(q, ix, iy);
                                e0->outp[ABOVE] = q;
                                e1->outp[ABOVE] = NULL;
                            }
                            break;
                        case EMX:
                            if (p && q) {
                                add_left(p, ix, iy);
                                merge_right(p, q, out_poly);
                                e0->outp[ABOVE] = NULL;
                                e1->outp[ABOVE] = NULL;
                            }
                            break;
                        case IMN:
                            add_local_min(&out_poly, e0, ix, iy);
                            e1->outp[ABOVE] = e0->outp[ABOVE];
                            break;
                        case ILI:
                            if (p) {
                                add_left(p, ix, iy);
                                e1->outp[ABOVE] = p;
                                e0->outp[ABOVE] = NULL;
                            }
                            break;
                        case IRI:
                            if (q) {
                                add_right(q, ix, iy);
                                e0->outp[ABOVE] = q;
                                e1->outp[ABOVE] = NULL;
                            }
                            break;
                        case IMX:
                            if (p && q) {
                                add_right(p, ix, iy);
                                merge_left(p, q, out_poly);
                                e0->outp[ABOVE] = NULL;
                                e1->outp[ABOVE] = NULL;
                            }
                            break;
                        case IMM:
                            if (p && q) {
                                add_right(p, ix, iy);
                                merge_left(p, q, out_poly);
                                add_local_min(&out_poly, e0, ix, iy);
                                e1->outp[ABOVE] = e0->outp[ABOVE];
                            }
                            break;
                        case EMM:
                            if (p && q) {
                                add_left(p, ix, iy);
                                merge_right(p, q, out_poly);
                                add_local_min(&out_poly, e0, ix, iy);
                                e1->outp[ABOVE] = e0->outp[ABOVE];
                            }
                            break;
                        default:
                            break;
                    } // End of switch
                } // End of contributing intersection conditional

                // Swap bundle sides in response to edge crossing
                if (e0->bundle[ABOVE][CLIP])
                    e1->bside[CLIP] = !e1->bside[CLIP];
                if (e1->bundle[ABOVE][CLIP])
                    e0->bside[CLIP] = !e0->bside[CLIP];
                if (e0->bundle[ABOVE][SUBJ])
                    e1->bside[SUBJ] = !e1->bside[SUBJ];
                if (e1->bundle[ABOVE][SUBJ])
                    e0->bside[SUBJ] = !e0->bside[SUBJ];

                // Swap e0 and e1 bundles in the AET
                prev_edge = e0->prev;
                next_edge = e1->next;
                if (next_edge)
                    next_edge->prev = e0;

                if (e0->bstate[ABOVE] == BUNDLE_HEAD) {
                    search = True;
                    while (search) {
                        prev_edge = prev_edge->prev;
                        if (prev_edge) {
                            if (prev_edge->bstate[ABOVE] != BUNDLE_TAIL)
                                search = False;
                        } else {
                            search= False;
                        }
                    }
                }

                if (!prev_edge) {
                    aet->prev = e1;
                    e1->next = aet;
                    aet = e0->next;
                } else {
                    prev_edge->next->prev = e1;
                    e1->next = prev_edge->next;
                    prev_edge->next = e0->next;
                }
                e0->next->prev = prev_edge;
                e1->next->prev = e1;
                e0->next = next_edge;
            } // End of IT loop

            // Prepare for next scanbeam
            for (edge = aet; edge; edge = next_edge) {
                next_edge = edge->next;
                succ_edge = edge->succ;

                if ((edge->top.y == yt) && succ_edge) {
                    // Replace AET edge by its successor
                    succ_edge->outp[BELOW] = edge->outp[ABOVE];
                    succ_edge->bstate[BELOW] = edge->bstate[ABOVE];
                    succ_edge->bundle[BELOW][CLIP] = edge->bundle[ABOVE][CLIP];
                    succ_edge->bundle[BELOW][SUBJ] = edge->bundle[ABOVE][SUBJ];
                    prev_edge = edge->prev;

                    if (prev_edge)
                        prev_edge->next = succ_edge;
                    else
                        aet = succ_edge;

                    if (next_edge)
                        next_edge->prev = succ_edge;

                    succ_edge->prev = prev_edge;
                    succ_edge->next = next_edge;
                } else {
                    // Update this edge
                    edge->outp[BELOW] = edge->outp[ABOVE];
                    edge->bstate[BELOW] = edge->bstate[ABOVE];
                    edge->bundle[BELOW][CLIP] = edge->bundle[ABOVE][CLIP];
                    edge->bundle[BELOW][SUBJ] = edge->bundle[ABOVE][SUBJ];
                    edge->xb = edge->xt;
                }
                edge->outp[ABOVE] = NULL;
            }
        }
    }

    // Generate result polygon from out_poly
    result->contour = NULL;
    result->num_contours = count_contours(out_poly);
    if (result->num_contours > 0) {
        result->contour = (vertex_list*)mem_malloc(result->num_contours * sizeof(vertex_list));

        c = 0;
        for (poly = out_poly; poly; poly = npoly) {
            npoly = poly->next;
            if (poly->active) {
                result->contour[c].num_vertices = poly->active;
                result->contour[c].vertex = (vertex_s*)mem_malloc(result->contour[c].num_vertices * sizeof(vertex_s));

                v = result->contour[c].num_vertices - 1;
                for (vtx = poly->proxy->v[LEFT]; vtx; vtx = nv) {
                    nv = vtx->next;
                    result->contour[c].vertex[v].x = vtx->x;
                    result->contour[c].vertex[v].y = vtx->y;
                    mem_free(vtx);
                    v--;
                }
                c++;
            }
            mem_free(poly);
        }
    } else {
        for (poly = out_poly; poly; poly = npoly) {
            npoly = poly->next;
            mem_free(poly);
        }
    }

    // Tidy up
    reset_it(&it);
    reset_lmt(&lmt);
    mem_free(c_heap);
    mem_free(s_heap);
    mem_free(sbt);
}

}
