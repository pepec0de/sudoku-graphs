#include <sudokugen.h>

// Devuelve true si el valor k puede colocarse en la celda (x, y)
bool isPossible(sudoku_t sudoku, uint8_t x, uint8_t y, uint8_t k) {
    for (uint8_t i = 0; i < N; ++i)  {
        if (sudoku[i][y] == k || sudoku[x][i] == k)
            return false;
    }

    uint8_t x0 = (x / NS) * NS; // floor division
    uint8_t y0 = (y / NS) * NS;
    for (uint8_t i = 0; i < NS; ++i) {
        for (uint8_t j = 0; j < NS; ++j) {
            if (sudoku[x0 + i][y0 + j] == k)
                return false;
        }
    }
    return true;
}

uint8_t getPossibleSols(sudoku_t sudoku, uint8_t x, uint8_t y, uint8_t* vres) {
    uint8_t count = 0;
    for (uint8_t k = 1; k <= N; ++k) {
        if (isPossible(sudoku, x, y, k)) {
            vres[count++] = k;
        }
    }
    return count;
}

bool findSolutions(sudoku_t sudoku, SudokuSet solutions_set, uint16_t* sol_cont) {
    // Factor humano: solucion trivial de cada celda
    for (uint8_t i = 0; i < N; ++i) {
        for (uint8_t j = 0; j < N; ++j) {
            if (sudoku[i][j] == 0) {
                uint8_t pSols[N];
                uint8_t pSols_sz = getPossibleSols(sudoku, i, j, pSols);
                if (pSols_sz == 1) {
                    sudoku[i][j] = pSols[0];
                }
            }
        }
    }

    for (uint8_t i = 0; i < N; ++i) {
        for (uint8_t j = 0; j < N; ++j) {
            if (sudoku[i][j] != 0)
                continue;
            
            uint8_t pSols[N];
            uint8_t pSols_sz = getPossibleSols(sudoku, i, j, pSols);
            switch (pSols_sz) {
            case 0:
                return false;
            
            case 1:
                sudoku[i][j] = pSols[0];
                continue;
            }

            // Caso recursivo
            uint8_t failCont = 0;
            for (uint8_t k = 0; k < pSols_sz; ++k) {
                sudoku_t copy;

                copySudoku(copy, sudoku);
                copy[i][j] = pSols[k];

                if (!findSolutions(copy, solutions_set, sol_cont))
                    ++failCont;
            }

            // Si todas las posibles soluciones han dado una solucion invalida es necesario descartar el sudoku actual
            if (failCont == pSols_sz)
                return false;

            return true;
        }
    }
    // Reallocar conjunto antes de que sea realmente necesario
    if ( *sol_cont == solutions_set.size ) {
        sudoku_t* ptr = realloc(solutions_set.solutions, solutions_set.size + 100);
        if (ptr == NULL) {
            printf("[ERROR] Memory reallocation failed. findSolutions returns false");
            return false;
        }
        
        solutions_set.solutions = ptr;
        solutions_set.size += 100;
    }
    // Solucion encontrada
    copySudoku(solutions_set.solutions[*sol_cont], sudoku);
    ++(*sol_cont);
    return true;
}
