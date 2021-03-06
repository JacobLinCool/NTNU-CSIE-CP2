// #define DEBUG
#include "helper.h"
#include "lib/bmp.h"
#define LINE_WIDTH 3

i32 main() {
    string image_path = calloc(1024, sizeof(char));
    string config_path = calloc(1024, sizeof(char));

    printf("Please enter the image: ");
    THROW_IF(scanf("%[^\n]%*c", image_path) != 1, "Invalid image path.");
    printf("Please enter the puzzle: ");
    THROW_IF(scanf("%[^\n]%*c", config_path) != 1, "Invalid config path.");

    BMP* source_img;
    u8 error = read_bmp(image_path, &source_img);
    THROW_IF(error, "Failed to load the image file: \"%s\"\n", BMP_ERROR_MESSAGE[error]);

    FILE* config = fopen(config_path, "r");
    THROW_IF(config == NULL, "Failed to open the config file: \"%s\"\n", config_path);

    i64 m, n, k;
    THROW_IF(fscanf(config, "%" PRId64 " %" PRId64 " %" PRId64, &m, &n, &k) != 3, "Invalid Config.\n");
    THROW_IF(m < 1 || n < 1 || k < 1, "Invalid Config. m, n, k must be positive.\n");
    DBG_PRINT("m: %" PRId64 ", n: %" PRId64 ", k: %" PRId64 "\n", m, n, k);

    i32 raw_width = source_img->header->info_header.width, raw_height = source_img->header->info_header.height;
    u64 each_width = (raw_width - LINE_WIDTH * (m + 1)) / m;
    u64 each_height = (raw_height - LINE_WIDTH * (n + 1)) / n;
    BMP** pieces = calloc(m * n + 1, sizeof(BMP*));
    pieces[0] = create_bmp(each_width, each_height, PIXEL_WHITE_TRANSPARENT);
    for (i64 i = 0; i < m * n; i++) {
        pieces[i + 1] = create_bmp(each_width, each_height, PIXEL_YELLOW);
        u64 offset_y = (i / m) * each_height + LINE_WIDTH * ((i / m) + 1);
        u64 offset_x = (i % m) * each_width + LINE_WIDTH * ((i % m) + 1);
        bmp_copy(pieces[i + 1], 0, 0, each_width, each_height, source_img, offset_x, offset_y);
    }

    i64* state = calloc(m * n, sizeof(i64));
    i64 empty = 0, initialized = 0;
    for (u64 i = 0; i < k; i++) {
        i64 new_state[m * n], new_empty;
        for (u64 j = 0; j < m * n; j++) {
            THROW_IF(fscanf(config, "%" PRId64, &new_state[j]) != 1, "Invalid Config.\n");
            THROW_IF(state[j] < 0 || state[j] > m * n, "Invalid Config. Out of range.\n");
            THROW_IF(
                state[j] != 0 && new_state[j] != 0 && state[j] != new_state[j],
                "Invalid Config. Invalid state <%" PRIu64 "> (%" PRId64 " %" PRIu64 "): %" PRId64 "\n",
                i + 1, j / m + 1, j % m + 1, new_state[j]
            );
            if (new_state[j] == 0) {
                new_empty = j;
            }
        }

        THROW_IF(
            initialized &&
            ((new_state[empty] != state[new_empty]) || (llabs(empty / m - new_empty / m) + llabs(empty % m - new_empty % m) != 1)),
            "Invalid Config. Invalid state <%" PRIu64 ">\n",
            i + 1
        );
        initialized = 1;

        for (u64 j = 0; j < m * n; j++) {
            state[j] = new_state[j];
        }
        empty = new_empty;

        BMP* step_img = create_bmp(raw_width / 3 * 3, raw_height / 3 * 3, PIXEL_BLACK);
        for (u64 row = 0; row < n; row++) {
            for (u64 col = 0; col < m; col++) {
                u64 piece_id = state[row * m + col];
                DBG_PRINT("piece_id: %" PRIu64 "\n", piece_id);
                u64 offset_y = row * each_height + LINE_WIDTH * (row + 1);
                u64 offset_x = col * each_width + LINE_WIDTH * (col + 1);
                bmp_copy(
                    step_img, offset_x, offset_y, offset_x + each_width, offset_y + each_height,
                    pieces[piece_id], 0, 0
                );
            }
        }

        string path = calloc(1024, sizeof(char));
        sprintf(path, "%" PRIu64 ".bmp", i);
        THROW_IF(step_img->save(step_img, path, 8, 8, 8, 0), "Failed to write the image file: \"%s\"\n", path);
        free(path);
        step_img->free(step_img);
    }

    free(state);
    for (u64 i = 0; i <= m * n; i++) {
        pieces[i]->free(pieces[i]);
    }
    free(pieces);
    source_img->free(source_img);

    printf("Done.\n");

    return EXIT_SUCCESS;
}
