#include "helper.h"

typedef struct String* TString;

void print_help(i32 argc, char* argv[]) {
    Console.rainbow("Visual Cryptography Image Generator");
    Console.info("Example:");
    Console.info(__FMT_YELLOW "    %s path/to/img.bmp" __FMT_NONE, argv[0]);
    Console.info("Licensing:");
    Console.info("    Copyright (c) 2022 Jacob Lin <hi@jacoblin.cool>");
    Console.info("    Distributed under the MIT License (MIT)");
    Console.info("    Source code is available on GitHub (maybe)");
    Console.info("    Application Built on " __DATE__ " " __TIME__);
}

i32 main(i32 argc, char* argv[]) {
    if (argc != 2) {
        Console.error("Usage: %s <source image path>", argv[0]);
        print_help(argc, argv);
        return EXIT_FAILURE;
    }

    Timing.start("Total");
    TString source_path = String.new(argv[1]);
    TString source_name = String.substring(source_path, 0, source_path->length - 4);
    BMP* source_img;
    if (read_bmp(source_path->data, &source_img)) {
        Console.error("Failed to read source image: %s", source_path->data);
        return EXIT_FAILURE;
    }

    for (i32 y = 0; y < source_img->header->info_header.height; y++) {
        for (i32 x = 0; x < source_img->header->info_header.width; x++) {
            f64 gray = (
                source_img->pixels[y][x]->red * 0.299 +
                source_img->pixels[y][x]->green * 0.587 +
                source_img->pixels[y][x]->blue * 0.114
                );

            if (gray >= 128) {
                source_img->rect(source_img, x, y, 1, 1, PIXEL_WHITE);
            }
            else {
                source_img->rect(source_img, x, y, 1, 1, PIXEL_BLACK);
            }
        }
    }

    u64 width = source_img->header->info_header.width * 2;
    u64 height = source_img->header->info_header.height * 2;

    BMP* layer_1 = create_bmp(width, height, PIXEL_WHITE_TRANSPARENT);
    BMP* layer_2 = create_bmp(width, height, PIXEL_WHITE_TRANSPARENT);
    BMP* overlap = create_bmp(width, height, PIXEL_WHITE);

    srand(time(NULL));

    for (i32 y = 0; y < source_img->header->info_header.height; y++) {
        for (i32 x = 0; x < source_img->header->info_header.width; x++) {
            i32 r = rand() % 2;
            if (memcmp(source_img->pixels[y][x], &PIXEL_WHITE, sizeof(Pixel)) == 0) {
                layer_1->rect(layer_1, x * 2, y * 2 + r, 1, 1, PIXEL_BLACK);
                layer_1->rect(layer_1, x * 2 + 1, y * 2 + !r, 1, 1, PIXEL_BLACK);
                layer_2->rect(layer_2, x * 2, y * 2 + r, 1, 1, PIXEL_BLACK);
                layer_2->rect(layer_2, x * 2 + 1, y * 2 + !r, 1, 1, PIXEL_BLACK);
            }
            else {
                layer_1->rect(r ? layer_1 : layer_2, x * 2, y * 2, 1, 1, PIXEL_BLACK);
                layer_1->rect(r ? layer_1 : layer_2, x * 2 + 1, y * 2 + 1, 1, 1, PIXEL_BLACK);
                layer_2->rect(r ? layer_2 : layer_1, x * 2 + 1, y * 2, 1, 1, PIXEL_BLACK);
                layer_2->rect(r ? layer_2 : layer_1, x * 2, y * 2 + 1, 1, 1, PIXEL_BLACK);
            }
        }
    }

    for (i32 y = 0; y < height; y++) {
        for (i32 x = 0; x < width; x++) {
            if (memcmp(layer_1->pixels[y][x], &PIXEL_BLACK, sizeof(Pixel)) == 0 ||
                memcmp(layer_2->pixels[y][x], &PIXEL_BLACK, sizeof(Pixel)) == 0) {
                overlap->rect(overlap, x, y, 1, 1, PIXEL_BLACK);
            }
        }
    }

    Console.info("Generating Layer 1...");
    TString output_layer_1_path = String.format("%s_layer_1.bmp", source_name->data);
    layer_1->save(layer_1, output_layer_1_path->data, 8, 8, 8, 0);
    Console.success("Saved to %s", output_layer_1_path->data);

    Console.info("Generating Layer 2...");
    TString output_layer_2_path = String.format("%s_layer_2.bmp", source_name->data);
    layer_2->save(layer_2, output_layer_2_path->data, 8, 8, 8, 0);
    Console.success("Saved to %s", output_layer_2_path->data);

    Console.info("Generating Overlap Image...");
    TString output_overlap_path = String.format("%s_overlap.bmp", source_name->data);
    overlap->save(overlap, output_overlap_path->data, 8, 8, 8, 0);
    Console.success("Saved to %s", output_overlap_path->data);

    Console.info("Generating Layer 1 (transparent)...");
    TString output_layer_1_transparent_path = String.format("%s_layer1_transparent.bmp", source_name->data);
    layer_1->save(layer_1, output_layer_1_transparent_path->data, 8, 8, 8, 8);
    Console.success("Saved to %s", output_layer_1_transparent_path->data);

    Console.info("Generating Layer 2 (transparent)...");
    TString output_layer_2_transparent_path = String.format("%s_layer2_transparent.bmp", source_name->data);
    layer_2->save(layer_2, output_layer_2_transparent_path->data, 8, 8, 8, 8);
    Console.success("Saved to %s", output_layer_2_transparent_path->data);

    Console.yellow("All tasks done in %.0Lf ms.", Timing.check("Total"));
    return EXIT_SUCCESS;
}
