#include <graph.h>

GraphSet* initGraph(sudoku_t sudoku) {
    GraphSet* graph = malloc(sizeof(GraphSet));

    memcpy(graph->sudoku, sudoku, sizeof(sudoku_t));

    printf("Testing memcpy\n");
    printSudoku(graph->sudoku);

    graph->n_solutions = 0;

    graph->solutions_size = 150;
    graph->solutions = malloc(graph->solutions_size * sizeof(adjm_t));
    
    for (uint8_t c = 0; c < GRAPH_ORDER; ++c) {
        // valueOfCell method
        uint8_t i = c / N, j = c % N;
        graph->labels[c] = sudoku[i][j];
    }

    return graph;
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
    for (uint8_t i = 0; i < GRAPH_ORDER; ++i) {
        uint8_t n_ones = 0;
        for (uint8_t j = 0; j < GRAPH_ORDER; ++j) {
            if (graph[i][j]) ++n_ones;
        }
        if (n_ones > graph_set->labels[i])
            // El nodo actual tiene mas aristas que las que debe tener -> descartado
            return;
    } // Todo esta comprobacion se podria combinar con la de abajo para mayor eficiencia
    // TODO : probar idea de arriba

    // Comprobacion grafo completo
    bool grafoCompleto = true;
    for (uint8_t i = 0; i < GRAPH_ORDER; ++i) {
        uint8_t grado = graph_set->labels[i];
        uint8_t cont = 0;
        for (uint8_t j = 0; j < GRAPH_ORDER; ++j) {
            cont += graph[i][j];
        }
        if (grado != cont) {
            grafoCompleto = false;
            break;
        }
    }

    if (grafoCompleto) {
        if (verbose) {
            printf("[VERBOSE] GRAPH %d\n", graph_set->n_solutions);
            printGraph(graph, graph_set);
            //system("pause");
        }
        memcpy(graph_set->solutions[graph_set->n_solutions++], graph, sizeof(adjm_t));
        
        // TODO: Aumentar capacidad conjunto de soluciones
        if (graph_set->n_solutions == graph_set->solutions_size) {
            exit(0); // temporal
            return;
        }
    }

    if (start != GRAPH_ORDER-1) {
        /***
         Generar secuencias de GRAPH_ORDER-N bits con valueOfCell(i) ones
            La primera siempre es valueOfCell(0) = 1 (orden lexicografico)

            Aplicar secuencias al grafo

            Una vez aplicadas las secuencias (cumpliendo con la simetria de la matriz)
            Recalcular secuencias teniendo en cuenta restricciones MAX_EDGES*2

        ***/
        // TRABAJAMOS DETRAS DE LA DIAGONAL (VAMOS DE MAS A MENOS)
        int n_ones = graph_set->labels[start], k_bits = 0;

        // Buscamos 1's delante de la diagonal
        //  En cada fila (i) solo podemos tener tantos 1's como valueOfCell(i)
        for (int j = 0; j < start; ++j)
            if (graph[start][j]) --n_ones;

        if (n_ones == 0) {
            // Se salta a la siguiente fila
            /*
                TODO : Probar idea de hacer directamente ++start y reajustar el parametro n_ones
                para no tener que hacer la comprobacion de grafoCompleto y grafo no valido
            */
           searchGraphs(graph, graph_set, start+1, verbose);
            return;
        }

        // Esto se puede calcular una vez y guardar en una matriz
        for (int j = start+1; j < GRAPH_ORDER; ++j)
            if (graph_set->labels[start] != graph_set->labels[j]) ++k_bits;

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

void printGraph(adjm_t graph, GraphSet* graph_data) {
    // Cabecera con los valores
    printf("         ");
    for (int i = 0; i < GRAPH_ORDER; i++) {
        printf("%d,", graph_data->labels[i]);
    }
    printf("\n         ");
    for (int i = 0; i < GRAPH_ORDER; i++) {
        printf("%d ", (i + 1) % 10);
    }
    printf("\n");

    // Filas del grafo
    for (int i = 0; i < GRAPH_ORDER; ++i) {
        printf("(%d), %d : ", graph_data->labels[i], (i + 1) % 10);
        for (int j = 0; j < GRAPH_ORDER; ++j) {
            if (graph[i][j] || i == j || graph_data->labels[i] == graph_data->labels[j]) {
                printf("%d ", graph[i][j]);
            } else {
                printf("  ");
            }
        }
        printf("\n");
    }

    printf("\n");
}