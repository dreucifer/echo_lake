#ifndef GRAPH_H
#define GRAPH_H

typedef struct graph_s graph_type, *graph_p;
typedef struct node_s node_type, *node_p;
typedef struct edge_s edge_type, *edge_p;

/* This is a test docstring */
struct graph_s {
    node_p nodes;
};

struct node_s {
    void * data;
    edge_p edges;
    node_p next;
};

struct edge_s {
    node_p sink;
    edge_p next;
};

graph_p graph_constructor();
int graph_initialize(graph_p self);
void graph_destructor(graph_p self);
int graph_add_nodes(graph_p self, void * data);
edge_p graph_add_edge(node_p source, node_p sink);

#endif
