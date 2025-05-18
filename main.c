#include <commons.h>
#include <sudokugen.h>
#include <graph.h>

int sudoku_graph_test() {
    bool verbose = true;
    if (verbose) 
        printf("N = %d\nNS = %d\n", N, NS);

    SudokuSet* solutions_set;
    solutions_set = malloc(sizeof(SudokuSet));
    sudoku_t base;
    clean(base);

    solutions_set->size = 300;
    solutions_set->solutions = malloc(solutions_set->size * sizeof(sudoku_t));
    
    uint16_t* sol_cont = malloc(sizeof(uint16_t));
    *(sol_cont) = 0;

    findSolutions(base, solutions_set, sol_cont, verbose);

    printf("N SUDOKUS = %d\n\n", *sol_cont);
    
    memcpy(base, solutions_set->solutions[0], sizeof(sudoku_t));

    free(solutions_set->solutions);
    free(sol_cont);
    free(solutions_set);

    GraphSet* graph = initGraph(base);
    adjm_t adjm;

    cleanAdjm(adjm);

    searchGraphs(adjm, graph, 0, verbose);

    printf("\n\nN GRAPHS = %d\n\n", graph->n_solutions);

    free(graph->solutions);
    free(graph);
    return 0;
}

int test_binseqs() {

    uint8_t n, k;
    scanf("%hhu", &n);
    scanf("%hhu", &k);

    BinSeqs* bin_seqs = getSequences(n, k);

    for (int_seq_t idx = 0; idx < bin_seqs->n; ++idx) {
        for (uint8_t i = 0; i < n; ++i) {
            printf("%d", bin_seqs->table[idx][i]);
        }
        printf("\n");
    }

    printf("N = %d\n", bin_seqs->n);

    free_BinSeqs(bin_seqs);
    return 0;
}


int main() {
    return sudoku_graph_test();
}