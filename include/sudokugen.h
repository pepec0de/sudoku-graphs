#ifndef SUDOKUGEN_H
#define SUDOKUGEN_H

#include <commons.h>

typedef struct {
    sudoku_t* solutions;
    uint16_t size;
} SudokuSet;

bool isPossible(sudoku_t sudoku, uint8_t x, uint8_t y, uint8_t k);
uint8_t getPossibleSols(sudoku_t sudoku, uint8_t x, uint8_t y, uint8_t* vres);
bool findSolutions(sudoku_t sudoku, SudokuSet solutions_set, uint16_t* sol_cont);

#endif