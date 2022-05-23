#ifndef __FILE_H
#define __FILE_H

#include "cimple/all.h"

typedef char* string;

typedef struct FileInfo {
    string path;
    size_t size;
    uint8_t* data;
} FileInfo;

FileInfo* read_file(string path) {
    FileInfo* info = calloc(1, sizeof(FileInfo));
    info->path = path;

    FILE* file = fopen(path, "rb");
    if (file == NULL) {
        free(info);
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    info->size = ftell(file);
    fseek(file, 0, SEEK_SET);
    info->data = malloc(sizeof(uint8_t) * info->size);
    fread(info->data, sizeof(uint8_t), info->size, file);
    fclose(file);

    return info;
}

void write_file(FileInfo* info) {
    FILE* file = fopen(info->path, "wb");
    if (file == NULL) {
        free(info->data), free(info), fclose(file);
        return;
    }
    fwrite(info->data, sizeof(uint8_t), info->size, file);
    fclose(file);
}

void free_file(FileInfo* info) {
    free(info->data), free(info);
}

#endif // __FILE_H
