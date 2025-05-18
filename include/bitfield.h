#ifndef BITFIELD_H
#define BITFIELD_H

#include <commons.h>

#define SET_BIT(BF, N) BF |= ((uint64_t) 0x00000000000000001 << N)
#define CLR_BIT(BF, N) BF &= ~((uint64_t) 0x00000000000000001 << N)
#define IS_BIT_SET(BF, N) ((BF >> N) & 0x1)

typedef uint64_t bitfield_t;

typedef struct {
    uint64_t* buffer_ptr;
} BitField;

#endif