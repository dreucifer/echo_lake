#include <stdlib.h>
#include "graph.h"

graph_p graph_constructor() {
    graph_p self = malloc(sizeof(struct graph_s));
    return self;
}

void graph_destructor(graph_p self) {
    free(self);
}

int graph_initialize(graph_p self) {
    self->nodes = NULL;

    return 0;
}
