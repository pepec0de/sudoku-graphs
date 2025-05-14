#include <commons.h>
#include <sudokugen.h>
#include <stdlib.h>

int main() {
    printf("Hello, World!\n");
    printf("N = %d\nNS = %d\n", N, NS);

    SudokuSet gen;

    sudoku_t base;
    clean(base);

    gen.size = 288;
    gen.solutions = malloc(gen.size*sizeof(sudoku_t));
    
    uint16_t* sol_cont = malloc(sizeof(uint16_t));
    *(sol_cont) = 0;

    findSolutions(base, gen.solutions, sol_cont);

    printf("N SUDOKUS = %d\n\n", *sol_cont);

    for (uint16_t i = 0; i < *sol_cont; ++i) {
        printf("SUDOKU %d\n", i);
        printSudoku(gen.solutions[i]);
        
    }


    free(gen.solutions);
    free(sol_cont);

    return 0;
}
