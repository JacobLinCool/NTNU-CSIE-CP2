// #define DEBUG
#include "utils.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#define MY_FILENAME_MAX 1024

// y = a*sin(bx) + c * cos(dx)
typedef struct Eq {
    i64 a, b, c, d;
    bool sin, cos;
} Eq;

StructVector(Eqs, Eq*, NULL);
Eqs* parse_config(string path);

i32 main() {
    string img_path = calloc(MY_FILENAME_MAX, sizeof(char));
    string eq_path = calloc(MY_FILENAME_MAX, sizeof(char));
    i64 width = 0, height = 0, line_width = 0, x_bound = 0, y_bound = 0;

    printf("Please enter the output image name: ");
    THROW_IF(scanf("%[^\n]%*c", img_path) != 1, "Invalid Source Image Filename.");
    printf("Please enter the size (x,y): ");
    THROW_IF(scanf("(%" SCNd64 ",%" SCNd64 ")%*c", &width, &height) != 2, "Invalid size.");
    THROW_IF(width <= 0 || height <= 0, "Invalid size.");
    printf("Please enter the line width (1,2,3): ");
    THROW_IF(scanf("%" SCNd64 "%*c", &line_width) != 1, "Invalid line width.");
    THROW_IF(line_width < 1 || line_width > 3, "Invalid line width.");
    printf("Please enter the equation file name: ");
    THROW_IF(scanf("%[^\n]%*c", eq_path) != 1, "Invalid equation file name.");
    printf("Please enter the x boundary (unit:pi): ");
    THROW_IF(scanf("%" SCNd64, &x_bound) != 1, "Invalid x boundary.");

    BMP* bmp = create_bmp(width, height, PIXEL_WHITE);

    // draw x-axis and y-axis
    Pixel PIXEL_GRAY = blend(PIXEL_BLACK, PIXEL_WHITE, 0.5);
    bmp->line(bmp, 0, height / 2, width, height / 2, line_width / 2, PIXEL_GRAY);
    bmp->line(bmp, 0, 0, 0, height, line_width / 2, PIXEL_GRAY);

    Eqs* eqs = parse_config(eq_path);
    // find y-bound
    for (i32 i = 0; i < eqs->size; i++) {
        Eq* eq = eqs->data[i];
        if (abs(eq->a) + abs(eq->c) > y_bound) {
            y_bound = abs(eq->a) + abs(eq->c);
        }
    }
    // printf("%u equations parsed.\n", eqs->size);
    for (i32 i = 0; i < eqs->size; i++) {
        Eq* eq = eqs->data[i];

        Pixel p = RGB(rand() % 16581375);

        for (i64 x = 0; x < width; x++) {
            i64 y = height / 2;
            if (eq->sin) {
                y += eq->a * sin(-eq->b * (f64)x / width * x_bound * M_PI) * height / (2 * y_bound);
            }
            else if (eq->cos) {
                y += eq->c * cos(-eq->d * (f64)x / width * x_bound * M_PI) * height / (2 * y_bound);
            }

            bmp->circle(bmp, x, y, line_width, p);
        }
    }

    bmp->save(bmp, img_path, 8, 8, 8, 0);
    return EXIT_SUCCESS;
}

Eqs* parse_config(string path) {
    i64 fd = open(path, O_RDONLY);
    THROW_IF(fd < 0, "Failed to open file.");
    u64 file_size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    string file_data = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);

    Eqs* eqs = create_Eqs();
    i64 offset = 0;
    while (true) {
        string line = calloc(1024, sizeof(char));
        i64 o = 0;
        sscanf(file_data + offset, "%[^\n]%*c%n", line, &o);
        offset += o;
        if (strlen(line) == 0) {
            break;
        }

        Eq* eq = calloc(1, sizeof(Eq));
        if (sscanf(line, "y=%ldsin(%ldx)", &eq->a, &eq->b) == 2) {
            eq->sin = true;
        }
        else if (sscanf(line, "y=%ldcos(%ldx)", &eq->c, &eq->d) == 2) {
            eq->cos = true;
        }
        else if (sscanf(line, "y=%ldsin(%ldx)+%ldcos(%ldx)", &eq->a, &eq->b, &eq->c, &eq->d) == 4) {
            eq->sin = true;
            eq->cos = true;
        }
        else if (strcmp(line, "y=sin(x)") == 0) {
            eq->a = 1;
            eq->b = 1;
            eq->sin = true;
        }
        else if (strcmp(line, "y=cos(x)") == 0) {
            eq->c = 1;
            eq->d = 1;
            eq->cos = true;
        }
        else if (strcmp(line, "y=sin(x)+cos(x)") == 0) {
            eq->a = 1;
            eq->b = 1;
            eq->c = 1;
            eq->d = 1;
            eq->sin = true;
            eq->cos = true;
        }
        else if (sscanf(line, "y=%ldsin(x)", &eq->a) == 1) {
            eq->b = 1;
            eq->sin = true;
        }
        else if (sscanf(line, "y=%ldcos(x)", &eq->c) == 1) {
            eq->d = 1;
            eq->cos = true;
        }
        else if (sscanf(line, "y=%ldsin(x)+%ldcos(x)", &eq->a, &eq->c) == 2) {
            eq->sin = true;
            eq->cos = true;
        }
        else if (sscanf(line, "y=sin(%ldx)", &eq->b) == 1) {
            eq->a = 1;
            eq->sin = true;
        }
        else if (sscanf(line, "y=cos(%ldx)", &eq->d) == 1) {
            eq->c = 1;
            eq->cos = true;
        }
        else if (sscanf(line, "y=sin(%ldx)+cos(%ldx)", &eq->b, &eq->d) == 2) {
            eq->sin = true;
            eq->cos = true;
        }
        else {
            THROW("Invalid equation.");
        }

        eqs->push(eqs, eq);
    }

    return eqs;
}