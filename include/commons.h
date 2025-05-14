#ifndef COMMONS_H
#define COMMONS_H

#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define N (uint8_t) 4
#define NS (uint8_t)sqrt(N) // N squared

typedef uint8_t sudoku_t[N][N];

void copySudoku(sudoku_t g1, sudoku_t g2);
void clean(sudoku_t g);
void printSudoku(sudoku_t g);

#endif