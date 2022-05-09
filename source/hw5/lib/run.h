#ifndef __RUN_H
#define __RUN_H

typedef char* string;

uint8_t* run(string command, size_t* buffer_size) {
    string cmd = String.format("%s 2>&1", command);
    FILE* fp = popen(cmd, "r");
    if (fp == NULL) {
        pclose(fp), free(cmd);
        return NULL;
    }

    size_t idx = 0, max = 1024;
    uint8_t* result = calloc(max, sizeof(uint8_t));
    while (fread(&result[idx], sizeof(uint8_t), 1, fp) > 0) {
        idx++;
        if (idx == max) {
            max <<= 1;
            result = realloc(result, max);
        }
    }

    if (buffer_size != NULL) {
        *buffer_size = idx;
    }

    pclose(fp), free(cmd);
    return result;
}

#endif // __RUN_H
