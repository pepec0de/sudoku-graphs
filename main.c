#include <commons.h>
#include <sudokugen.h>
#include <graph.h>

int main() {
    printf("Hello, World! %ld\n", sizeof(uint8_t));
    printf("N = %d\nNS = %d\n", N, NS);

    SudokuSet* solutions_set;
    solutions_set = malloc(sizeof(SudokuSet));
    sudoku_t base;
    clean(base);

    solutions_set->size = 300;
    solutions_set->solutions = malloc(solutions_set->size * sizeof(sudoku_t));
    
    uint16_t* sol_cont = malloc(sizeof(uint16_t));
    *(sol_cont) = 0;

    printf("findSolutions\n");
    findSolutions(base, solutions_set, sol_cont, true);

    printf("N SUDOKUS = %d\n\n", *sol_cont);

    for (uint16_t i = 0; i < 1/**sol_cont*/; ++i) {
        printf("SUDOKU %d\n", i);
        printSudoku(solutions_set->solutions[i]);
        
    }
    
    memcpy(base, solutions_set->solutions[0], sizeof(sudoku_t));

    free(solutions_set->solutions);
    free(sol_cont);
    free(solutions_set);

    Graph graph = initGraph(base);
    printSudoku(graph.sudoku);

    free(graph.solutions);
    return 0;
}
