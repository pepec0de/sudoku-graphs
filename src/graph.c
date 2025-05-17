#include <graph.h>

GraphSet* initGraph(sudoku_t sudoku) {
    GraphSet* graph = malloc(sizeof(GraphSet));

    memcpy(graph->sudoku, sudoku, sizeof(sudoku_t));

    printf("Testing memcpy\n");
    printSudoku(graph->sudoku);

    graph->solutions_size = 1000;
    graph->solutions = malloc(graph->solutions_size * sizeof(adjm_t));
    
    for (uint8_t c = 0; c < GRAPH_ORDER; ++c) {
        // valueOfCell method
        uint8_t i = c / N, j = c % N;
        graph->labels[c] = sudoku[i][j];
    }

    return graph;
}

void copyAdjm(adjm_t a1, adjm_t a2) {
    for (uint8_t i = 0; i < GRAPH_ORDER; ++i)
        for (uint8_t j = 0; j < GRAPH_ORDER; ++j)
            a1[i][j] = a2[i][j];
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
        int8_t grado = graph_set->labels[i];
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
            printf("[VERBOSE] GRAPH %d", graph_set->n_solutions);
            printAdjm(graph, " ");
            //system("pause");
        }
        copyAdjm(graph_set->solutions[graph_set->n_solutions++], graph);
        /*
        // Aumentar capacidad conjunto de soluciones
        if (n_solutions == sz_solutions) {
            adjm* new_solutions = new adjm[sz_solutions + 10*N];
            for (int i = 0; i < n_solutions; ++i) {
                copyAdjm(new_solutions[i], solutions[i]);
        }
        delete [] solutions;
        solutions = new_solutions;
        }*/
        return;
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
            generateGraph(graph, start+1, verbose);
            return;
        }

        // Esto se puede calcular una vez y guardar en una matriz
        for (int j = start+1; j < GRAPH_ORDER; ++j)
            if (graph_set->labels[start] != graph_set->labels[j]) ++k_bits;

        if (k_bits != 0) {
            BinarySequences binseqs(k_bits, n_ones);
            for (int j = 0; j < binseqs.getN(); ++j) {
                adjm cGraph;
                copyAdjm(cGraph, graph);
                apply_binary_seq(cGraph, binseqs.getSequences()[j], start);
                // Caso recursivo
                generateGraph(cGraph, start+1, verbose);
            }
        }
    }
}
