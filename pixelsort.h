#ifndef PXLSORT_H
#define PXLSORT_H

#include <FreeImage.h>
#include <stdbool.h>

#define BPP 3
#define ANGLE_HORI 0
#define ANGLE_VERT 90

typedef struct context {
    // Image variables
    unsigned width;
    unsigned height;

    unsigned angle;

    // Threshold variables
    int upper;
    int lower;
    char *dirs;

    // Functions
    bool (*check_color)(struct context *ctx, BYTE *line);
    BYTE *(*index_color)(struct context *ctx, BYTE *line, unsigned index);
} context_t;

// Interval functions
bool checkThreshold(struct context *ctx, BYTE *color);

// Nextcolor functions
BYTE *indexcolor_horizontal(struct context *ctx, BYTE *line, unsigned index);
BYTE *indexcolor_vertical(struct context *ctx, BYTE *line, unsigned index);

int pixelsort(context_t *ctx, FIBITMAP *img);

#endif
