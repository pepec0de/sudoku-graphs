#include <graph.h>

GraphSet* initGraph(sudoku_t sudoku) {
    GraphSet* graph_set = malloc(sizeof(GraphSet));

    memcpy(graph_set->sudoku, sudoku, sizeof(sudoku_t));

    graph_set->n_solutions = 0;

    graph_set->solutions_size = 1000;
    graph_set->solutions = malloc(graph_set->solutions_size * sizeof(adjm_t));
    
    for (uint8_t c = 0; c < GRAPH_ORDER; ++c) {
        // valueOfCell method
        uint8_t i = c / N, j = c % N;
        graph_set->labels[c] = sudoku[i][j];
    }

    uint8_t k_bits;
    for (uint8_t c = 0; c < GRAPH_ORDER - 1; ++c) {
        k_bits = 0;
        for (int j = c+1; j < GRAPH_ORDER; ++j)
            if (graph_set->labels[c] != graph_set->labels[j]) ++k_bits;
        graph_set->k_bits_vct[c] = k_bits;
    }

    return graph_set;
}

void cleanAdjm(adjm_t a1) {
    for (uint8_t i = 0; i < GRAPH_ORDER; ++i)
        for (uint8_t j = 0; j < GRAPH_ORDER; ++j)
            a1[i][j] = 0;
}

void searchGraphs(adjm_t graph, GraphSet* graph_set, uint8_t start, bool verbose) {
    // Casos base:
    /***
     * Comprobar numero de 1's por cada nodo
        (como es matriz simetrica no hay comprobar columnas aparte)
        * Lo primero que hacemos es descartar el grafo actual por si hay
        * alguna adyacencia mal puesta (puede pasar por el tema de la simetria)
    ***/

    // Comprobacion grafo invalido
    uint8_t n_ones;
    for (uint8_t i = 0; i < GRAPH_ORDER; ++i) {
        n_ones = 0;
        for (uint8_t j = 0; j < GRAPH_ORDER; ++j) {
            if (graph[i][j]) ++n_ones;
        }
        if (n_ones > graph_set->labels[i])
            // El nodo actual tiene mas aristas que las que debe tener -> descartado
            return;
    }

    // Comprobacion grafo completo
    bool grafoCompleto = true;
    for (uint8_t i = 0; i < GRAPH_ORDER; ++i) {
        n_ones = 0;
        for (uint8_t j = 0; j < GRAPH_ORDER; ++j) {
            n_ones += graph[i][j]; // = if (graph[i][j]) ++n_ones;
        }

        // Comprobacion grafo incompleto
        if (graph_set->labels[i] != n_ones) {
            grafoCompleto = false;
            break;
        }
    }
    
    // Comprobaciones combinadas (no hay mayor eficencia que lo de arriba)
    /*
    bool grafoCompleto = true;
    for (uint8_t i = 0; i < GRAPH_ORDER; ++i) {
        uint8_t n_ones = 0;
        for (uint8_t j = 0; j < GRAPH_ORDER; ++j) {
            //n_ones += graph[i][j];
            if (graph[i][j]) ++n_ones;
        }

        // Comprobacion de grafo invalido
        // Si el nodo actual tiene mas aristas que su etiqueta -> se descarta
        if (n_ones > graph_set->labels[i])
            return;

        // Comprobacion grafo incompleto
        if (n_ones != graph_set->labels[i]) {
            grafoCompleto = false;
            break;
        }
    }
    */

    if (grafoCompleto) {
        if (verbose) {
            printf("[VERBOSE] GRAPH %d\n", graph_set->n_solutions);
            printGraph(graph, graph_set);
        }
        memcpy(graph_set->solutions[graph_set->n_solutions++], graph, sizeof(adjm_t));
        
        // TODO: Aumentar capacidad conjunto de soluciones -> usar realloc
        if (graph_set->n_solutions == graph_set->solutions_size) {
            exit(-1234); // temporal
            return;
        }
    }

    /***
     Generar secuencias de GRAPH_ORDER-N bits con valueOfCell(i) ones
        La primera siempre es valueOfCell(0) = 1 (orden lexicografico)

        Aplicar secuencias al grafo

        Una vez aplicadas las secuencias (cumpliendo con la simetria de la matriz)
        Recalcular secuencias teniendo en cuenta restricciones MAX_EDGES*2

    ***/
    // Antigua version:
    /*
    // Resultados: para N = 3 N_GRAPHS = 208
    // TODO: Comprobar unicidad del conjunto grafo
    if (start == GRAPH_ORDER-1) return;
    // TRABAJAMOS DETRAS DE LA DIAGONAL (VAMOS DE MAS A MENOS)
    n_ones = graph_set->labels[start];

    // Buscamos 1's delante de la diagonal
    //  En cada fila (i) solo podemos tener tantos 1's como valueOfCell(i)
    for (int j = 0; j < start; ++j)
        if (graph[start][j]) --n_ones;

    if (n_ones == 0) {
        // Se salta a la siguiente linea
        searchGraphs(graph, graph_set, start+1, verbose);
        return;
    }
    */
    
    /*
    Hacer directamente ++start y reajustar el parametro n_ones
    para no tener que hacer la comprobacion de grafoCompleto y grafo no valido
    // Resultados: paraN = 3 N_GRAPHS = 93
    */

    if (start == GRAPH_ORDER-1)
        return;

    n_ones = 0;
    while (n_ones == 0 && start != GRAPH_ORDER-1) {
        n_ones = graph_set->labels[start];
        for (int j = 0; j < start; ++j)
            if (graph[start][j]) --n_ones;
        
        if (n_ones == 0)
            ++start;
        
        if (start == GRAPH_ORDER-1)
            return;
    }

    uint8_t k_bits = graph_set->k_bits_vct[start];

    if (k_bits != 0) {
        BinSeqs* bin_seqs = getSequences(k_bits, n_ones);
        for (int_seq_t j = 0; j < bin_seqs->n; ++j) {
            adjm_t cGraph;
            memcpy(cGraph, graph, sizeof(adjm_t));
            apply_binary_seq(cGraph, graph_set, bin_seqs->table[j], &start);
            // Caso recursivo
            searchGraphs(cGraph, graph_set, start+1, verbose);
        }
        free_BinSeqs(bin_seqs);
    }
}

void apply_binary_seq(adjm_t graph, GraphSet* graph_set, bool* bin_seq, uint8_t* idx) {
    uint8_t i_seq = 0;
    for (uint8_t j = (*idx) + 1; j < GRAPH_ORDER; ++j)
        if (graph_set->labels[*idx] != graph_set->labels[j])
            set(graph, idx, &j, bin_seq[i_seq++]);
}

void set(adjm_t graph, uint8_t* i, uint8_t* j, bool x) {
    graph[*i][*j] = x;
    graph[*j][*i] = x;
}

void printGraph(adjm_t graph, GraphSet* graph_set) {
    // Cabecera con los valores
    printf("         ");
    for (int i = 0; i < GRAPH_ORDER; i++) {
        printf("%d,", graph_set->labels[i]);
    }
    printf("\n         ");
    for (int i = 0; i < GRAPH_ORDER; i++) {
        printf("%d ", (i + 1) % 10);
    }
    printf("\n");

    // Filas del grafo
    for (int i = 0; i < GRAPH_ORDER; ++i) {
        printf("(%d), %d : ", graph_set->labels[i], (i + 1) % 10);
        for (int j = 0; j < GRAPH_ORDER; ++j) {
            if (graph[i][j] || i == j || graph_set->labels[i] == graph_set->labels[j]) {
                printf("%d ", graph[i][j]);
            } else {
                printf("  ");
            }
        }
        printf("\n");
    }

    printf("\n");
}