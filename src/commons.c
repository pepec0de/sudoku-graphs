#include <commons.h>

void copySudoku(sudoku_t g1, sudoku_t g2) {
    for (uint8_t i = 0; i < N; ++i) {
        for (uint8_t j = 0; j < N; ++j) {
            g1[i][j] = g2[i][j];
        }
    }
}

void clean(sudoku_t g) {
    for (uint8_t i = 0; i < N; ++i) {
        for (uint8_t j = 0; j < N; ++j) {
            g[i][j] = 0;
        }
    }
}

void printSudoku(sudoku_t g) {
    uint8_t K = N/NS;
    for (uint8_t i = 0; i < N; ++i) {
        for (uint8_t j = 0; j < N; ++j) {
            printf("%d ", g[i][j]);

            if ((j + 1) % NS == 0 && j != N - 1) {
                printf("| ");
            }
        }
        printf("\n");

        if ((i + 1) % NS == 0 && i != N - 1) {
            for (uint8_t k = 0; k < K; ++k) {
                for (uint8_t m = 0; m < NS; ++m) {
                    printf("--");
                }

                if (k != K - 1) {
                    printf("+");
                    if (NS % 2 != 0 && k != K - 1)
                        printf("-");
                }
            }
            printf("\n");
        }
    }
    printf("\n");
}

void bubble_sort(uint8_t* arr, uint8_t n, uint8_t* idxs) {
    bool swapped;

    if (idxs)
        for (uint8_t i = 0; i < n; ++i)
            idxs[i] = i;
            
    for (uint8_t i = 0; i < n - 1; i++) {
        swapped = false;
        for (uint8_t j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                // Intercambiar
                uint8_t temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swapped = true;

                if (idxs) {
                    temp = idxs[j];
                    idxs[j] = idxs[j + 1];
                    idxs[j + 1] = temp;
                }
            }
        }
        if (!swapped)
            break;
    }
}