#include "lib/bmp.h"
#include "lib/catcher.h"
#include "lib/cimple/all.h"

typedef char* string;

Pixel* get_pixel(i64 r, i64 g, i64 b) {
    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255) {
        return NULL;
    }

    Pixel* pixel = (Pixel*)malloc(sizeof(Pixel));
    pixel->red = (uint8_t)r;
    pixel->green = (uint8_t)g;
    pixel->blue = (uint8_t)b;
    return pixel;
}

void help() {
    Console.yellow("Usage:");
    Console.yellow("    ./fin01 [options]");
    Console.cyan("Options:");
    Console.cyan("    -w, --width  : the width of the output figure. (Default: 1024)");
    Console.cyan("    -h, --height : the height of the output figure. (Default: 768)");
    Console.cyan("    -o, --output : the output file name. (Default: output.bmp)");
    Console.cyan("    -H, --help   : this message");
}

i32 main(i32 argc, string argv[]) {
    setup_catcher();

    ParsedOptions* options = Options.parse(argc, argv);
    Options.key_only(options, "help");
    Options.key_only(options, "H");

    if (Options.has(options, "help") || Options.has(options, "H")) {
        help();
        return EXIT_SUCCESS;
    }

    i64    W = 1024, H = 768;
    string output = calloc(1024, sizeof(char));
    strcpy(output, "output.bmp");

    if (Options.has(options, "width") || Options.has(options, "w")) {
        if (!Options.get(options, "width") && !Options.get(options, "w")) {
            Console.red("Value of width is required.");
            return EXIT_FAILURE;
        }

        char* end = NULL;
        W = (i64)strtol(Options.get(options, "width") ? Options.get(options, "width")
                                                      : Options.get(options, "w"),
                        &end, 10);

        if (*end != '\0' || W < 1 || W > 2000) {
            Console.red(
                "The value of the option -w, --width must be an integer between 1 and 2000.");
            return EXIT_FAILURE;
        }
    }

    if (Options.has(options, "height") || Options.has(options, "h")) {
        if (!Options.get(options, "height") && !Options.get(options, "h")) {
            Console.red("Value of height is required.");
            return EXIT_FAILURE;
        }

        char* end = NULL;
        H = (i64)strtol(Options.get(options, "height") ? Options.get(options, "height")
                                                       : Options.get(options, "h"),
                        &end, 10);

        if (*end != '\0' || H < 1 || H > 2000) {
            Console.red(
                "The value of the option -h, --height must be an integer between 1 and 2000.");
            return EXIT_FAILURE;
        }
    }

    if (Options.has(options, "output") || Options.has(options, "o")) {
        if (!Options.get(options, "output") && !Options.get(options, "o")) {
            Console.red("Value of output is required.");
            return EXIT_FAILURE;
        }

        strcpy(output, Options.get(options, "output") ? Options.get(options, "output")
                                                      : Options.get(options, "o"));

        if (strlen(output) == 0) {
            Console.red("The value of the option -o, --output must be a non-empty string.");
            return EXIT_FAILURE;
        }
    }

    Pixel *top_left = NULL, *top_right = NULL, *bottom_left = NULL, *bottom_right = NULL;

    i64 r = 0, g = 0, b = 0;
    printf(__FMT_CYAN "Please enter the top left pixel: " __FMT_NONE);
    if (scanf("%" PRId64 ",%" PRId64 ",%" PRId64, &r, &g, &b) != 3) {
        Console.red("Invalid input.");
        return EXIT_FAILURE;
    }
    top_left = get_pixel(r, g, b);
    if (top_left == NULL) {
        Console.red("The value of the pixel is invalid.");
        return EXIT_FAILURE;
    }
    printf(__FMT_CYAN "Please enter the top right pixel: " __FMT_NONE);
    if (scanf("%" PRId64 ",%" PRId64 ",%" PRId64, &r, &g, &b) != 3) {
        Console.red("Invalid input.");
        return EXIT_FAILURE;
    }
    top_right = get_pixel(r, g, b);
    if (top_right == NULL) {
        Console.red("The value of the pixel is invalid.");
        return EXIT_FAILURE;
    }
    printf(__FMT_CYAN "Please enter the bottom left pixel: " __FMT_NONE);
    if (scanf("%" PRId64 ",%" PRId64 ",%" PRId64, &r, &g, &b) != 3) {
        Console.red("Invalid input.");
        return EXIT_FAILURE;
    }
    bottom_left = get_pixel(r, g, b);
    if (bottom_left == NULL) {
        Console.red("The value of the pixel is invalid.");
        return EXIT_FAILURE;
    }
    printf(__FMT_CYAN "Please enter the bottom right pixel: " __FMT_NONE);
    if (scanf("%" PRId64 ",%" PRId64 ",%" PRId64, &r, &g, &b) != 3) {
        Console.red("Invalid input.");
        return EXIT_FAILURE;
    }
    bottom_right = get_pixel(r, g, b);
    if (bottom_right == NULL) {
        Console.red("The value of the pixel is invalid.");
        return EXIT_FAILURE;
    }

    BMP* img = create_bmp(W, H, PIXEL_BLACK_TRANSPARENT);
    if (img == NULL) {
        Console.red("Failed to create the image.");
        return EXIT_FAILURE;
    }

    Pixel* base_a[W + 1];
    Pixel* base_b[W + 1];

    for (size_t i = 0; i < W; i++) {
        base_a[i] = get_pixel(
            (i64)(top_left->red + ((f128)(top_right->red - top_left->red) * i / (W - 1))),
            (i64)(top_left->green + ((f128)(top_right->green - top_left->green) * i / (W - 1))),
            (i64)(top_left->blue + ((f128)(top_right->blue - top_left->blue) * i / (W - 1))));
        base_b[i] = get_pixel(
            (i64)(bottom_left->red + ((f128)(bottom_right->red - bottom_left->red) * i / (W - 1))),
            (i64)(bottom_left->green +
                  ((f128)(bottom_right->green - bottom_left->green) * i / (W - 1))),
            (i64)(bottom_left->blue +
                  ((f128)(bottom_right->blue - bottom_left->blue) * i / (W - 1))));
    }

    for (size_t x = 0; x < W; x++) {
        for (size_t y = 0; y < H; y++) {
            Pixel* pixel = get_pixel(
                (i64)(base_a[x]->red + ((f128)(base_b[x]->red - base_a[x]->red) * y / (H - 1))),
                (i64)(base_a[x]->green +
                      ((f128)(base_b[x]->green - base_a[x]->green) * y / (H - 1))),
                (i64)(base_a[x]->blue + ((f128)(base_b[x]->blue - base_a[x]->blue) * y / (H - 1))));
            img->pixels[y][x] = pixel;
        }
    }

    if (img->save(img, output, 8, 8, 8, 0)) {
        Console.red("Failed to save the image.");
        return EXIT_FAILURE;
    }

    Console.green("Done");

    return EXIT_SUCCESS;
}
