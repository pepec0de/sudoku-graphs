#ifndef GRAPH_H
#define GRAPH_H

#include <commons.h>

typedef struct {
    sudoku_t sudoku;
    adjm_t* solutions;
    uint16_t solutions_size;
    uint8_t labels[GRAPH_ORDER];
} Graph;

Graph initGraph(sudoku_t sudoku);
void searchGraphs(adjm_t graph, uint8_t start, bool verbose);

#endif