#include <graph.h>

Graph initGraph(sudoku_t sudoku) {
    Graph graph;

    memcpy(graph.sudoku, sudoku, sizeof(sudoku_t));

    printf("Testing memcpy\n");
    printSudoku(graph.sudoku);

    graph.solutions_size = 1000;
    graph.solutions = malloc(graph.solutions_size * sizeof(adjm_t));
    
    for (uint8_t c = 0; c < GRAPH_ORDER; ++c) {
        uint8_t i = c / N, j = c % N;
        graph.labels[c] = sudoku[i][j];
    }

    return graph;
}

void searchGraphs(adjm_t graph, uint8_t start, bool verbose) {

}
