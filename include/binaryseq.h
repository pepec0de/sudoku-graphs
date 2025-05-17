#ifndef BINARYSEQ_H
#define BINARYSEQ_H

#include <commons.h>

// TODO: aplicar bitfields aqui

typedef struct {
    bool** table;
    int_seq_t n;
} BinSeqs;

int_seq_t nCm(uint8_t* n, uint8_t* k);
void printSeq(bool seq[], uint8_t n);
void generateSequences(uint8_t* n, uint8_t k, uint8_t pos, bool seq[], bool** table, int_seq_t* seq_idx_ptr);
BinSeqs* getSequences(uint8_t n, uint8_t k);
void free_BinSeqs(BinSeqs* bin_seqs);

#endif