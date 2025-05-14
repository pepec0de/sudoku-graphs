#include <commons.h>
#include <sudokugen.h>

int main() {
    printf("Hello, World!\n");
    printf("N = %d\nNS = %d\n", N, NS);

    SudokuSet solutions_set;

    sudoku_t base;
    clean(base);

    solutions_set.size = 288;
    solutions_set.solutions = malloc(solutions_set.size*sizeof(sudoku_t));
    
    uint16_t* sol_cont = malloc(sizeof(uint16_t));
    *(sol_cont) = 0;

    findSolutions(base, solutions_set, sol_cont);

    printf("N SUDOKUS = %d\n\n", *sol_cont);

    for (uint16_t i = 0; i < *sol_cont; ++i) {
        printf("SUDOKU %d\n", i);
        printSudoku(solutions_set.solutions[i]);
        
    }

    free(solutions_set.solutions);
    free(sol_cont);

    return 0;
}
