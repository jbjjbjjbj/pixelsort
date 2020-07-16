#include "pixelsort.h"

#include <stdio.h>
#include <FreeImage.h>
#include <stdlib.h>
#include <string.h>

#include "sort.h"

#define SUM(c) (c[FI_RGBA_RED] + c[FI_RGBA_GREEN] + c[FI_RGBA_BLUE])
#define SCALEUP(c) (255 * c)

int maxcomp(BYTE *c) {
    int first = (c[FI_RGBA_RED] < c[FI_RGBA_BLUE]) ? 
        c[FI_RGBA_BLUE] : c[FI_RGBA_RED];
    return (first < c[FI_RGBA_GREEN]) ? c[FI_RGBA_GREEN] : first;
}

bool checkThreshold(struct context *ctx, BYTE *color) {
    int light = maxcomp(color);
    return light >= ctx->lower && light <= ctx->upper;
}

BYTE *indexcolor_horizontal(struct context *ctx, BYTE *line, unsigned index) {
    return line + BPP * index;
}

BYTE *indexcolor_vertical(struct context *ctx, BYTE *line, unsigned index) {
    return line + BPP * index * ctx->width;
}

void sortpart(context_t *ctx, BYTE *part, pixelm *values, unsigned length) {
    quicksort(values, 0, length-1);

    BYTE *color;;
    for (unsigned i = 0; i < length; i++) {
        color = ctx->index_color(ctx, part, i);
        memcpy(color, values[i].color, 3);
    }
}


int sortline(context_t *ctx, BYTE *line, unsigned length) {
    int start = -1;
    pixelm *pixelvalues = (pixelm *) malloc(length * sizeof(pixelm));
    if (!pixelvalues) {
        fprintf(stderr, "Could not allocate array\n");
        return 1;
    }
    unsigned pxindex = 0;

    BYTE *color;;
    for (unsigned i = 0; i < length; i++) {
        color = ctx->index_color(ctx, line, i);

        int light = maxcomp(color);
        if (ctx->check_color(ctx, color)) {
            if (start < 0) {
                start = i;
                pxindex = 0;
            }
            // Reserve sorting to make bleedlines go "down"
            pixelvalues[pxindex].val = ctx->angle == ANGLE_VERT ? -light : light;
            memcpy(pixelvalues[pxindex].color, color, 3);
            pxindex++;
        } else if (start >= 0) {
            sortpart(ctx, ctx->index_color(ctx, line, start), pixelvalues, pxindex);
            //qsort((void *)&line[start * 3], i - start - 1, 3, compare);

            start = -1;
        }
    }

    if (start >= 0) {
        //qsort((void *)&line[start * 3], length - start - 1, 3, compare);
        sortpart(ctx, ctx->index_color(ctx, line, start), pixelvalues, pxindex);
    }

    free(pixelvalues);

    return 0;
}

void pixelsort_single(context_t *ctx, FIBITMAP *img) {
    unsigned length = ctx->angle == ANGLE_VERT ? ctx->height : ctx->width;
    unsigned looptimes = ctx->angle == ANGLE_VERT ? ctx->width : ctx->height;

    BYTE *bits = FreeImage_GetBits(img);

    for (unsigned i = 0; i < looptimes; i++) {
        BYTE *line = ctx->angle == ANGLE_HORI ? 
            indexcolor_vertical(ctx, bits, i) :
            indexcolor_horizontal(ctx, bits, i);

        sortline(ctx, line, length);
    }
}

int pixelsort(context_t *ctx, FIBITMAP *img) {
    ctx->width = FreeImage_GetWidth(img);
    ctx->height = FreeImage_GetHeight(img);

    char *dirs = ctx->dirs;

    char c;
    while ((c = *dirs++) != 0) {
        switch (c) {
            case 'v':
                ctx->angle = ANGLE_VERT;
                ctx->index_color = indexcolor_vertical;
                break;
            case 'h':
                ctx->angle = ANGLE_HORI;
                ctx->index_color = indexcolor_horizontal;
                break;
            default:
                fprintf(stderr, "Invalid direction %c\n", c);
                return 1;
        }

        pixelsort_single(ctx, img);
    }

    return 0;
}
