#include <binaryseq.h>

int_seq_t nCm(uint8_t* n, uint8_t* k) {
    int_seq_t res = 1;
    uint8_t k_ = *k;

    // Since C(n, k) = C(n, n-k)
    if (*k > *n - *k)
        k_ = *n - *k;

    // Calculate value of
    // [n * (n-1) *---* (n-k+1)] / [k * (k-1) *----* 1]
    for (uint8_t i = 0; i < k_; ++i) {
        res *= (*n - i);
        res /= (i + 1);
    }

    return res;
}

BinSeqs* getSequences(uint8_t n, uint8_t k) {
    BinSeqs* sequences_ptr = malloc(sizeof(BinSeqs));
    sequences_ptr->n = nCm(&n, &k);
    sequences_ptr->table = malloc(sequences_ptr->n * sizeof(bool*));
    int_seq_t* seq_idx_ptr = malloc(sizeof(int_seq_t));
    *seq_idx_ptr = 0;

    bool seq[n];

    generateSequences(&n, k, 0, seq, sequences_ptr->table, seq_idx_ptr);

    free(seq_idx_ptr);

    return sequences_ptr;
}

void generateSequences(uint8_t* n, uint8_t k, uint8_t pos, bool seq[], bool** table, int_seq_t* seq_idx_ptr) {
    if (pos == *n) {
        table[*seq_idx_ptr] = malloc(sizeof(bool) * (*n));
        memcpy(table[*seq_idx_ptr], seq, sizeof(bool) * (*n));
        ++(*seq_idx_ptr);
        return;
    }

    if (k > 0) {
        seq[pos] = 1;
        generateSequences(n, k - 1, pos + 1, seq, table, seq_idx_ptr);
    }

    if (*n - pos > k) {
        seq[pos] = 0;
        generateSequences(n, k, pos + 1, seq, table, seq_idx_ptr);
    }
}

void free_BinSeqs(BinSeqs* bin_seqs) {
    if (bin_seqs == NULL)
        return;

    for (int_seq_t idx = 0; idx < bin_seqs->n; ++idx)
        free(bin_seqs->table[idx]);

    free(bin_seqs->table);
    bin_seqs->table = NULL;

    free(bin_seqs);
}