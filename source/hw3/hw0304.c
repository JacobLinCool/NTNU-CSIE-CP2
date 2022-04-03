// #define DEBUG
#include "helper.h"
#include "bmp.h"

i32 main() {
    string input = calloc(1024, sizeof(char));
    string output = calloc(1024, sizeof(char));

    printf("Please enter the input image name: ");
    THROW_IF(scanf("%[^\n]%*c", input) != 1, "Invalid input image path.");
    printf("Please enter the output image name: ");
    THROW_IF(scanf("%[^\n]%*c", output) != 1, "Invalid output image path.");

    BMP* bmp = read_bmp(input);
    THROW_IF(bmp == NULL, "Failed to load the input image: \"%s\"\n", input);
    THROW_IF(strncmp(bmp->header->magic, "BM", 2) != 0, "\"%s\" is not a valid BMP file. Magic: %s\n", input, bmp->header->magic);

    THROW_IF(write_bmp(bmp, output, 16) != 0, "Failed to write the output image: \"%s\"\n", output);
    return EXIT_SUCCESS;
}
