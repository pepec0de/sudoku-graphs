#include <graph.h>

GraphSet* initGraph(sudoku_t sudoku) {
    GraphSet* graph_set = malloc(sizeof(GraphSet));

    memcpy(graph_set->sudoku, sudoku, sizeof(sudoku_t));

    graph_set->n_solutions = 0;

    graph_set->solutions_size = 10000;
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

    graph_set->sorted_idx = malloc(GRAPH_ORDER * sizeof(uint8_t));
    uint8_t* arr = malloc(GRAPH_ORDER * sizeof(uint8_t));
    memcpy(arr, graph_set->labels, GRAPH_ORDER * sizeof(uint8_t));

    bubble_sort(arr, GRAPH_ORDER, graph_set->sorted_idx);

    printf("Sorted idx:\n");
    for (uint8_t i = 0; i < GRAPH_ORDER; ++i)
        printf("%d,", graph_set->sorted_idx[i]);
    printf("\n\n");

    free(arr);

    return graph_set;
}

void cleanAdjm(adjm_t a1) {
    for (uint8_t i = 0; i < GRAPH_ORDER; ++i)
        for (uint8_t j = 0; j < GRAPH_ORDER; ++j)
            a1[i][j] = 0;
}

void searchGraphs(adjm_t graph, GraphSet* graph_set, uint8_t start, bool verbose) {
    // Temporal
    if (graph_set->n_solutions == graph_set->solutions_size) return;

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
            if (graph_set->n_solutions % 1000 == 0)
            printf("[VERBOSE] GRAPH %d\n", graph_set->n_solutions);
            //printGraph(graph, graph_set);
        }

        memcpy(graph_set->solutions[graph_set->n_solutions++], graph, sizeof(adjm_t));
        
        // TODO: Aumentar capacidad conjunto de soluciones -> usar realloc
        if (graph_set->n_solutions == graph_set->solutions_size) {
            //exit(9); // temporal
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
    
    /*
    Hacer directamente ++start y reajustar el parametro n_ones
    para no tener que hacer la comprobacion de grafoCompleto y grafo no valido
    // Resultados: paraN = 3 N_GRAPHS = 93
    */
    
    if (start == GRAPH_ORDER-1)
        return;

    n_ones = 0;
    while (n_ones == 0) {
        n_ones = graph_set->labels[start];
        for (uint8_t j = 0; j < start; ++j) {
            if (n_ones == 0)
                break;

            if (graph[start][j])
                --n_ones;
        }
        
        if (n_ones == 0) {
            ++start;
            if (start == GRAPH_ORDER-1)
                return;
        }
    }

    uint8_t k_bits = graph_set->k_bits_vct[start];

    // nCr(k_bits, n_ones) Must happen k_bits >= n_ones
    if (n_ones > k_bits)
        n_ones = k_bits;

    BinSeqs* bin_seqs = getSequences(k_bits, n_ones);
    for (int_seq_t j = 0; j < bin_seqs->n; ++j) {
        adjm_t cGraph;
        memcpy(cGraph, graph, sizeof(adjm_t));
        apply_binary_seq(cGraph, graph_set, bin_seqs->table[j], start);
        // Caso recursivo
        searchGraphs(cGraph, graph_set, start+1, verbose);
    }
    free_BinSeqs(bin_seqs);

}

void apply_binary_seq(adjm_t graph, GraphSet* graph_set, bool* bin_seq, uint8_t idx) {
    uint8_t i_seq = 0;
    for (uint8_t j = idx + 1; j < GRAPH_ORDER; ++j)
        if (graph_set->labels[idx] != graph_set->labels[j])
            set(graph, idx, j, bin_seq[i_seq++]);
}

void set(adjm_t graph, uint8_t i, uint8_t j, bool x) {
    graph[i][j] = x;
    graph[j][i] = x;
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

int_set_t compareSet(GraphSet* graph_set) {
    int_set_t n_uniques = 0;
    bool is_unique;

    for (int_set_t i = 0; i < graph_set->n_solutions; ++i) {
        is_unique = true;

        for (int_set_t j = 0; j < i; ++j) {
            if (areEqualAdjm(graph_set->solutions[i], graph_set->solutions[j])) {
                is_unique = false;
                break;
            }
        }

        if (is_unique)
            ++n_uniques;
    }

    return n_uniques;
}

bool areEqualAdjm(adjm_t a1, adjm_t a2) {
    for (uint8_t i = 0; i < GRAPH_ORDER; ++i)
        for (uint8_t j = 0; j < GRAPH_ORDER; ++j)
            if (a1[i][j] != a2[i][j])
                return false;

    return true;
}

int_set_t compareEqualSet(GraphSet* graph_set) {
    int_set_t n_uniques = 0;
    bool is_unique;

    for (int_set_t i = 0; i < graph_set->n_solutions; ++i) {
        is_unique = true;

        for (int_set_t j = 0; j < i; ++j) {
            
            if (areEqualGraph(graph_set, i, j)) {
                is_unique = false;
                break;
            }
        }
        if (is_unique) {
            ++n_uniques;
            printf("Unique %d\n", n_uniques);
            printGraph(graph_set->solutions[i], graph_set);
        }
    }

    return n_uniques;
}

void take_mat_at(uint8_t l, GraphSet* graph_set, uint8_t matriz[N][N], adjm_t a1) {
    for (uint8_t i = 0; i < N; ++i)
            for (uint8_t j = 0; j < N; ++j)
                matriz[i][j] = -1;

    uint8_t c1 = 0;
    for (uint8_t i = 0; i < GRAPH_ORDER; ++i) {
        if (graph_set->labels[i] == l) {
            // Store the nodes connected to cell(i) in matriz[c1]
            uint8_t c2 = 0;
            for (uint8_t j = 0; j < GRAPH_ORDER; ++j) {
                if (a1[i][j]) {
                    matriz[c1][c2++] = graph_set->labels[j];
                    //printf("c2 = %d, matriz[c1][c2++] = graph_set->labels[j] = %d\n", c2-1, graph_set->labels[j]);
                }
                if (c2 == l)
                    break;
            }
            bubble_sort(matriz[c1], N, NULL);
            ++c1;
        }
    }
}

bool areEqualEdges(uint8_t m1[N][N], uint8_t m2[N][N]) {
    
    bool found;
    for (uint8_t i = 0; i < N; ++i) {
        found = false;
        for (uint8_t j = 0; j < N; ++j) {
            bool equal = true;
            for (uint8_t k = 0; k < N; ++k) {
                if (m1[i][k] != m2[j][k]) {
                    equal = false;
                    break;
                }
            }

            if (equal) {
                found = true;
                break;
            }
        }

        if (!found)
            return false;
    }

    for (uint8_t i = 0; i < N; ++i) {
        found = false;
        for (uint8_t j = 0; j < N; ++j) {
            bool equal = true;
            for (uint8_t k = 0; k < N; ++k) {
                if (m2[i][k] != m1[j][k]) {
                    equal = false;
                    break;
                }
            }

            if (equal) {
                found = true;
                break;
            }
        }

        if (!found)
            return false;
    }

    return true;
}

bool areEqualGraph(GraphSet* graph_set, int_set_t idx1, int_set_t idx2) {

    uint8_t matriz1[N][N], matriz2[N][N];

    bool equal;

    for (uint8_t l = 1; l <= N; ++l) {

        take_mat_at(l, graph_set, matriz1, graph_set->solutions[idx1]);
        take_mat_at(l, graph_set, matriz2, graph_set->solutions[idx2]);

        equal = areEqualEdges(matriz1, matriz2);

        /*
        if (equal)
        printf("Equal mats:\n");
        else 
        printf("NOT Equal mats:\n");
        printf("Matriz1 = \n");
        for (uint8_t i = 0; i < N; ++i) {
            for (uint8_t j = 0; j < N; ++j)
                printf("%d, ", matriz1[i][j]);
            printf("\n");
        }
        printf("Matriz2 = \n");
        for (uint8_t i = 0; i < N; ++i) {
            for (uint8_t j = 0; j < N; ++j)
                printf("%d, ", matriz2[i][j]);
            printf("\n");
        }
        */

        if (!equal)
            return false;
    }

    return true;
}
