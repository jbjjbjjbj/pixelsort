#ifndef SORT_H
#define SORT_H

#include <FreeImage.h>

typedef struct {
    uint8_t color[3];
    uint16_t val;
} pixelm;

void quicksort(pixelm *arr, unsigned lo, unsigned hi);

#endif
