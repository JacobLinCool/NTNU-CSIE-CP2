// #define DEBUG
#include "utils.h"
#include "myvector.h"
#define IMG_FILENAME_MAX 1024

i32 main() {
    string filename = calloc(IMG_FILENAME_MAX, sizeof(char));
    printf("Please enter the file name: ");
    THROW_IF(scanf("%[^\n]%*c", filename) != 1, "Invalid Source Image Filename.");
    BMP* img;
    u8 err;
    THROW_IF((err = read_bmp(filename, &img)) != 0, "Failed to read image. %s", BMP_ERROR_MESSAGE[err]);

    i64 angle = INT64_MIN;
    printf("Rotation angle (int, 0-360): ");
    THROW_IF(scanf("%" SCNd64, &angle) != 1, "Invalid rotation angle.");
    THROW_IF(angle < 0 || angle > 360, "Invalid rotation angle.");
    f64 radian = -angle * (M_PI / 180);

    DBG_PRINT("radian = %lf\n", radian);
    DBG_PRINT("sin = %lf\n", sin(radian));
    DBG_PRINT("cos = %lf\n", cos(radian));

    i64 new_height = abs(img->header->info_header.height * cos(radian)) + abs(img->header->info_header.width * sin(radian));
    i64 new_width = abs(img->header->info_header.width * cos(radian)) + abs(img->header->info_header.height * sin(radian));

    DBG_PRINT("new_height = %ld\n", new_height);
    DBG_PRINT("new_width = %ld\n", new_width);

    BMP* rotated = create_bmp(new_width, new_height, PIXEL_WHITE);

    for (i64 y = 0; y < img->header->info_header.height; y++) {
        for (i64 x = 0; x < img->header->info_header.width; x++) {
            sVector* vec = myvector_init();
            myvector_set(vec, Polar, sqrt(x * x + y * y), atan2(y, x) + radian);
            sVector* cartesian = vec_to_cartesian(vec);
            i64 new_y = (i64)(cartesian->data.c.y + img->header->info_header.width * fabs(sin(radian)));
            i64 new_x = (i64)cartesian->data.c.x;
            if (bmp_safe(rotated, new_x, new_y)) {
                rotated->pixels[new_y][new_x] = img->pixels[y][x];
            }
            else {
                DBG_PRINT("Failed to set pixel at (%ld, %ld)\n", new_x, new_y);
            }
        }
    }

    rotated->save(rotated, "output.bmp", 8, 8, 8, 0);

    return EXIT_SUCCESS;
}
