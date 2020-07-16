#include "sort.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define SUM(c) (c[FI_RGBA_RED] + c[FI_RGBA_GREEN] + c[FI_RGBA_BLUE])

unsigned partition(pixelm *arr, unsigned lo, unsigned hi) {
    int pivot = arr[lo].val;
    int i = lo - 1;
    int j = hi + 1;

    while (true) {
        do {
            i++;
        } while (arr[i].val < pivot);

        do {
            j--;
        } while (arr[j].val > pivot);

        if (i >= j) {
            return j;
        }

        // Swap j and i
        pixelm temp;
        memcpy(&temp, arr+i, sizeof(pixelm));
        memcpy(arr+i, arr+j, sizeof(pixelm));
        memcpy(arr+j, &temp, sizeof(pixelm));
    }
}

void quicksort(pixelm *arr, unsigned lo, unsigned hi) {
    if (lo >= hi) {
        return;
    }

    unsigned p = partition(arr, lo, hi);
    quicksort(arr, lo, p);
    quicksort(arr, p+1, hi);

    return;
}

