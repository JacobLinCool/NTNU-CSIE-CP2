/**
 * @file bmp.h
 * @author JacobLinCool (hi@jacoblin.cool)
 * @brief BMP image library.
 * @version 0.1
 * @date 2022-04-01
 *
 * @copyright Copyright (c) 2022 JacobLinCool
 *
 */
#include "helper.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <ctype.h>

typedef struct Mask {
    u32 blue;
    u32 green;
    u32 red;
    u32 reserved;
} __attribute__((packed)) Mask;

static Mask Mask_565 = { 0b1111100000000000, 0b0000011111100000, 0b0000000000011111, 0b0000000000000000 };

typedef struct Pixel {
    u8 blue;
    u8 green;
    u8 red;
} Pixel;

typedef struct BMP_HEADER {
    char magic[2];
    u32	size;
    u16	reserve_1;
    u16	reserve_2;
    u32	offset;
    u32	header_size;
    i32	width;
    i32	height;
    u16	planes;
    u16	bpp;
    u32	compression;
    u32	bitmap_size;
    i32	res_height;
    i32	res_width;
    u32	palette;
    u32	important;
} __attribute__((packed)) BMP_HEADER;

typedef struct BMP {
    BMP_HEADER* header;
    Pixel*** pixels;
} BMP;

BMP* create_bmp(u64 width, u64 height) {
    BMP* bmp = calloc(1, sizeof(BMP));
    bmp->header = calloc(1, sizeof(BMP_HEADER));
    bmp->header->magic[0] = 'B';
    bmp->header->magic[1] = 'M';
    bmp->header->size = sizeof(BMP_HEADER) + 3 * width * height;
    bmp->header->offset = sizeof(BMP_HEADER);
    bmp->header->header_size = 40;
    bmp->header->width = width;
    bmp->header->height = height;
    bmp->header->planes = 1;
    bmp->header->bpp = 24;
    bmp->header->bitmap_size = 3 * width * height;
    bmp->header->res_height = 9449;
    bmp->header->res_width = 9449;

    bmp->pixels = calloc(height, sizeof(Pixel**));
    for (u64 i = 0; i < height; i++) {
        bmp->pixels[i] = calloc(width, sizeof(Pixel*));
        for (u64 j = 0; j < width; j++) {
            bmp->pixels[i][j] = calloc(1, sizeof(Pixel));
            bmp->pixels[i][j]->blue = 0xFF;
            bmp->pixels[i][j]->green = 0xFF;
            bmp->pixels[i][j]->red = 0xFF;
        }
    }

    return bmp;
}

BMP* read_bmp(string path) {
    i32 fd = open(path, O_RDONLY);
    if (fd < 0) {
        return NULL;
    }
    i64 size = lseek(fd, 0, SEEK_END);

    u8* file = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (file == MAP_FAILED) {
        return NULL;
    }

    BMP* bmp = calloc(1, sizeof(BMP));
    bmp->header = calloc(1, sizeof(BMP_HEADER));

    if (strncmp((string)file, "BM", 2) != 0) {
        bmp->header->magic[0] = file[0];
        bmp->header->magic[1] = file[1];
        munmap(file, size);
        close(fd);
        return bmp;
    }

    memcpy(bmp->header, file, sizeof(BMP_HEADER));
    bmp->pixels = calloc(bmp->header->height, sizeof(Pixel*));
    for (i32 i = 0; i < bmp->header->height; i++) {
        bmp->pixels[i] = calloc(bmp->header->width, sizeof(Pixel*));
    }
    u8* data = file + bmp->header->offset;
    u64 offset = 0;
    u64 row_size = ((bmp->header->width * bmp->header->bpp + 31) / 32) * 4;
    for (i32 i = 0; i < bmp->header->height; i++) {
        for (i32 j = 0; j < bmp->header->width; j++) {
            Pixel* pixel = calloc(1, sizeof(Pixel));

            if (bmp->header->bpp == 24) {
                pixel->blue = data[offset++];
                pixel->green = data[offset++];
                pixel->red = data[offset++];
            }
            else if (bmp->header->bpp == 16) {
                u16 color = *(u16*)&data[offset];
                pixel->blue = (color & 0b1111100000000000) >> 11;
                pixel->green = (color & 0b0000011111100000) >> 5;
                pixel->red = (color & 0b0000000000011111);
                offset += 2;
            }

            bmp->pixels[bmp->header->height - 1 - i][j] = pixel;
        }

        if (offset % row_size != 0) {
            offset += row_size - (offset % row_size);
        }
    }

    munmap(file, size);
    close(fd);
    return bmp;
}

u8 write_bmp(BMP* bmp, string path, u8 bpp) {
    if (bpp != 24 && bpp != 16) {
        return 1;
    }

    FILE* file = fopen(path, "w");
    if (file == NULL) {
        return 1;
    }

    BMP_HEADER* header = calloc(1, sizeof(BMP_HEADER));
    memcpy(header, bmp->header, sizeof(BMP_HEADER));
    header->bpp = bpp;
    header->bitmap_size = bmp->header->width * bmp->header->height * (bpp / 8);
    header->size = sizeof(BMP_HEADER) + header->bitmap_size;

    if (bpp == 16) {
        header->compression = 3;
        header->size += sizeof(Mask) + 68;
        header->offset += sizeof(Mask) + 68;
        header->header_size += sizeof(Mask) + 68;
        fwrite(header, sizeof(BMP_HEADER), 1, file);
        fwrite(&Mask_565, sizeof(Mask), 1, file);
        fwrite("BGRs", 4, 1, file);
        u64 zeros[8] = { 0 };
        fwrite(zeros, 64, 1, file);
    }
    else {
        fwrite(header, sizeof(BMP_HEADER), 1, file);
    }

    free(header);

    u64 row_size = ((bmp->header->width * bmp->header->bpp + 31) / 32) * 4;
    u64 padding = row_size - (bmp->header->width * bmp->header->bpp / 8);
    u8* pad = calloc(padding, sizeof(u8));
    for (i32 i = bmp->header->height - 1; i >= 0; i--) {
        for (i32 j = 0; j < bmp->header->width; j++) {
            if (bpp == 24) {
                fwrite(&bmp->pixels[i][j]->blue, sizeof(u8), 1, file);
                fwrite(&bmp->pixels[i][j]->green, sizeof(u8), 1, file);
                fwrite(&bmp->pixels[i][j]->red, sizeof(u8), 1, file);
            }
            else {
                u8 blue = (f128)bmp->pixels[i][j]->blue / 0xff * 0b11111;
                u8 green = (f128)bmp->pixels[i][j]->green / 0xff * 0b111111;
                u8 red = (f128)bmp->pixels[i][j]->red / 0xff * 0b11111;
                u16 color = (red << 11) | (green << 5) | blue;
                fwrite(&color, sizeof(u16), 1, file);
            }
        }

        if (padding != 0) {
            fwrite(pad, padding, 1, file);
        }
    }

    fclose(file);
    return 0;
}

/**
 * @brief Fill a part of the image with a pixel
 *
 * @param bmp the image to fill
 * @param from_x the x coordinate of the top left corner of the rectangle
 * @param from_y the y coordinate of the top left corner of the rectangle
 * @param width the width of the rectangle
 * @param height the height of the rectangle
 * @param pixel the pixel to fill the rectangle with
 * @return the count of pixels that were filled
 */
u64 bmp_fill(BMP* bmp, i64 from_x, i64 from_y, i64 width, i64 height, Pixel pixel) {
    u64 count = 0;

    for (i64 y = from_y; y < height; y++) {
        for (i64 x = from_x; x < width; x++) {
            if (y >= 0 && y < bmp->header->height && x >= 0 && x < bmp->header->width) {
                bmp->pixels[y][x]->red = pixel.red;
                bmp->pixels[y][x]->green = pixel.green;
                bmp->pixels[y][x]->blue = pixel.blue;
                count++;
            }
        }
    }

    return count;
}

/**
 * @brief Copy a part of the source image to the destination image.
 *
 * @param bmp the destination image
 * @param from_x the x coordinate of the top left corner of the rectangle
 * @param from_y the y coordinate of the top left corner of the rectangle
 * @param width the width of the rectangle
 * @param height the height of the rectangle
 * @param source the source image
 * @param source_x the x coordinate of the source rectangle
 * @param source_y the y coordinate of the source rectangle
 * @return the count of pixels copied.
 */
u64 bmp_copy(BMP* bmp, i64 from_x, i64 from_y, i64 width, i64 height, BMP* source, i64 source_x, i64 source_y) {
    u64 count = 0;

    for (i64 y = 0; y < height; y++) {
        for (i64 x = 0; x < width; x++) {
            if (from_y + y >= 0 && from_y + y < bmp->header->height && from_x + x >= 0 && from_x + x < bmp->header->width) {
                if (source_y + y >= 0 && source_y + y < source->header->height && source_x + x >= 0 && source_x + x < source->header->width) {
                    bmp->pixels[from_y + y][from_x + x]->red = source->pixels[source_y + y][source_x + x]->red;
                    bmp->pixels[from_y + y][from_x + x]->green = source->pixels[source_y + y][source_x + x]->green;
                    bmp->pixels[from_y + y][from_x + x]->blue = source->pixels[source_y + y][source_x + x]->blue;
                    count++;
                }
            }
        }
    }

    return count;
}

/**
 * @brief Draw a line on the image
 *
 * @param bmp the image to draw on
 * @param from_x the x coordinate of the start of the line
 * @param from_y the y coordinate of the start of the line
 * @param to_x the x coordinate of the end of the line
 * @param to_y the y coordinate of the end of the line
 * @param width the width of the line
 * @param pixel the color of the line
 * @return the count of pixels that were drawn
 */
u64 bmp_line(BMP* bmp, i64 from_x, i64 from_y, i64 to_x, i64 to_y, u64 width, Pixel pixel) {
    u64 count = 0;

    i64 center_x = from_x, center_y = from_y;

    i8 vecs[][2] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 }, { 1, 1 }, { -1, 1 }, { 1, -1 }, { -1, -1 } };
    u8 mask = 0b11111111;

    while (1) {
        count += bmp_fill(bmp, center_x - width / 2, center_y - width / 2, width, width, pixel);

        i64 candidate_x = center_x, candidate_y = center_y, candidate_dist = INT64_MAX;
        for (i8 i = 0; i < 8; i++) {
            if (mask & (1 << i)) {
                i64 x = center_x + vecs[i][0], y = center_y + vecs[i][1];

                i64 dist = (x - to_x) * (x - to_x) + (y - to_y) * (y - to_y);
                if (dist < candidate_dist) {
                    candidate_x = x;
                    candidate_y = y;
                    candidate_dist = dist;
                }
                else {
                    mask &= ~(1 << i);
                }
            }
        }

        if (center_x == to_x && center_y == to_y) {
            break;
        }
    }

    return count;
}
