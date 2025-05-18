#ifndef GRAPH_H
#define GRAPH_H

#include <commons.h>
#include <binaryseq.h>

typedef struct {
    sudoku_t sudoku;
    adjm_t* solutions;
    uint16_t n_solutions;
    uint16_t solutions_size;
    uint8_t labels[GRAPH_ORDER];
    uint8_t k_bits_vct[GRAPH_ORDER-1];
    // Se omite el ultimo nodo -> no tiene nada a su izquierda
} GraphSet;

GraphSet* initGraph(sudoku_t sudoku);
void searchGraphs(adjm_t graph, GraphSet* graph_set, uint8_t start, bool verbose);
void apply_binary_seq(adjm_t graph, GraphSet* graph_set, bool* bin_seq, uint8_t* idx);
void set(adjm_t graph, uint8_t* i, uint8_t* j, bool x);
void printGraph(adjm_t graph, GraphSet* graph_data);
void cleanAdjm(adjm_t a1);
#endif