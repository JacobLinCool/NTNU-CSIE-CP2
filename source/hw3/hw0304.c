// #define DEBUG
#include "helper.h"
#include "lib/bmp.h"

i32 main() {
    string input = calloc(1024, sizeof(char));
    string output = calloc(1024, sizeof(char));

    printf("Please enter the input image name: ");
    THROW_IF(scanf("%[^\n]%*c", input) != 1, "Invalid input image path.");
    printf("Please enter the output image name: ");
    THROW_IF(scanf("%[^\n]%*c", output) != 1, "Invalid output image path.");

    BMP* bmp;
    u8 error = read_bmp(input, &bmp);
    THROW_IF(error, "Failed to load the input image: \"%s\"\n", BMP_ERROR_MESSAGE[error]);

    THROW_IF(bmp->save(bmp, output, 5, 6, 5, 0), "Failed to write the output image: \"%s\"\n", output);

    printf("Done.\n");
    return EXIT_SUCCESS;
}
