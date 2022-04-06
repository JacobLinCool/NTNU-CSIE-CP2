// #define DEBUG
#include "helper.h"
#include "lib/bmp.h"

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

    BMP* bmp;
    u8 error = read_bmp(input, &bmp);
    THROW_IF(error, "Failed to load the input image: \"%s\"\n", BMP_ERROR_MESSAGE[error]);

    f64 r_square = radius * radius;

    for (i32 y = 0; y < bmp->header->info_header.height; y++) {
        for (i32 x = 0; x < bmp->header->info_header.width; x++) {
            if (pow(x - center_x, 2) + pow(y - center_y, 2) > r_square) {
                bmp->rect(bmp, x, y, 1, 1, PIXEL_WHITE);
            }
        }
    }

    THROW_IF(bmp->save(bmp, output, 8, 8, 8, 0), "Failed to write the output image: \"%s\"\n", output);

    printf("Done.\n");
    return EXIT_SUCCESS;
}
