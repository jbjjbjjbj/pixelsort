#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <FreeImage.h>

#include "pixelsort.h"

void printhelp(char *executable) {
    printf("usage: %s [options] input\n", executable);
    printf("\n");
    printf("options:\n");
    printf("  -h    this help message.\n");
    printf("  -u    threshold upper limit (default: 204).\n");
    printf("  -l    threshold lower limit (default: 63).\n");
    printf("  -d    order of sorting directions (default \"hv\").\n");
}

char *parseargs(int argc, char **argv, context_t *ctx) {
    int opt;

    while((opt = getopt(argc, argv, "hd:u:l:")) != -1) {
        switch (opt) {
            case 'u':
                ctx->upper = strtol(optarg, NULL, 10);
                break;
            case 'l':
                ctx->lower = strtol(optarg, NULL, 10);
                break;
            case 'd':
                ctx->dirs = optarg;
                break;
            case 'h':
                printhelp(argv[0]);
                exit(0);
                break;
            case '?':
                printhelp(argv[0]);
                exit(1);
                break;
        }
    }

    if (argc - optind < 1) {
        fprintf(stderr, "Input image not supplied");
        exit(1);
    }

    return argv[optind];

}

FIBITMAP *load_image(char *name) {
    FREE_IMAGE_FORMAT imgformat = FreeImage_GetFileType(name, 0);
    if (imgformat == FIF_UNKNOWN) {
        fprintf(stderr, "Could not determine image format\n");
        return NULL;
    }

    FIBITMAP *temp = FreeImage_Load(imgformat, name, 0);
    if (!temp) {
        fprintf(stderr, "Error loading image\n");
        return NULL;
    }

    FIBITMAP *image = FreeImage_ConvertTo24Bits(temp);
    if (!temp) {
        fprintf(stderr, "Error converting image to 24 bits\n");
        return NULL;
    }

    FreeImage_Unload(temp);

    return image;
}

int main(int argc, char **argv) {
    context_t ctx = {
        .upper = 204,
        .lower = 63,
        .dirs = "hv",
        .check_color = checkThreshold
    };
    char *imgname = parseargs(argc, argv, &ctx);

    printf("Starting\n");
    FreeImage_Initialise(1);

    FIBITMAP *img = load_image(imgname);
    if (!img) {
        fprintf(stderr, "Error loading image %s\n", imgname);
        return 1;
    }

    int ret = pixelsort(&ctx, img);
    if (ret) {
        exit(1);
    }

    if (!FreeImage_Save(FIF_PNG, img, "out.png", 0)) {
        fprintf(stderr, "Error saving to out.png\n");
        return 1;
    }

    FreeImage_DeInitialise();
}
