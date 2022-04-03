// #define DEBUG
#include "helper.h"
#include "bmp.h"

i32 main() {
    string input = calloc(1024, sizeof(char));
    string output = calloc(1024, sizeof(char));
    i32 center_x = -1, center_y = -1, radius = -1;

    printf("Please enter the input image name: ");
    THROW_IF(scanf("%[^\n]%*c", input) != 1, "Invalid input image path.");
    printf("Please enter the output image name: ");
    THROW_IF(scanf("%[^\n]%*c", output) != 1, "Invalid output image path.");
    printf("Please enter the center: ");
    THROW_IF(scanf("(%" SCNd32 ",%" SCNd32 ")", &center_x, &center_y) != 2, "Invalid center coordinates.");
    printf("Please enter the radius: ");
    THROW_IF(scanf("%" SCNd32, &radius) != 1, "Invalid radius.");

    BMP* bmp = read_bmp(input);
    THROW_IF(bmp == NULL, "Failed to load the input image: \"%s\"\n", input);
    THROW_IF(strncmp(bmp->header->magic, "BM", 2) != 0, "\"%s\" is not a valid BMP file. Magic: %s\n", input, bmp->header->magic);

    f64 r_square = pow(radius, 2);

    for (i32 y = 0; y < bmp->header->height; y++) {
        for (i32 x = 0; x < bmp->header->width; x++) {
            if (pow(x - center_x, 2) + pow(y - center_y, 2) > r_square) {
                bmp->pixels[y][x]->red = 0xFF;
                bmp->pixels[y][x]->green = 0xFF;
                bmp->pixels[y][x]->blue = 0xFF;
            }
        }
    }

    THROW_IF(write_bmp(bmp, output, 24) != 0, "Failed to write the output image: \"%s\"\n", output);
    printf("Done.\n");

    return EXIT_SUCCESS;
}
