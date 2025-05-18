#ifndef COMMONS_H
#define COMMONS_H

#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define N (uint8_t) 3
#define NS (uint8_t) sqrt(N) // N squared
#define GRAPH_ORDER (uint8_t) N*N
#define N_EDGES (uint8_t) ceil((N*N*(N+1))/4.0)

// tipo para binaryseq
typedef uint32_t int_seq_t;

// sizeof(uint8_t) = 1 byte
typedef uint8_t sudoku_t[N][N];
// sizeof(bool) = 1 byte
typedef bool adjm_t[GRAPH_ORDER][GRAPH_ORDER];


/*
    Idea for adjm_t type:
    bitfields

    typedef struct {
        uint64_t* mat : GRAPH_ORDER;
    } adjm_t;
*/
void copySudoku(sudoku_t g1, sudoku_t g2);
void clean(sudoku_t g);
void printSudoku(sudoku_t g);

#endif