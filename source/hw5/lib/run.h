#ifndef __RUN_H
#define __RUN_H

typedef char* string;

string run(string command) {
    string cmd = String.format("%s 2>&1", command);
    FILE* fp = popen(cmd, "r");
    if (fp == NULL) {
        Console.error("Failed to run command: %s", command);
    }

    size_t len = 0, max = 1024;
    string result = calloc(max, sizeof(char));
    while (!feof(fp)) {
        char c = fgetc(fp);

        result[len++] = c;

        if (len == max) {
            max <<= 1;
            result = realloc(result, max);
        }
    }

    pclose(fp);
    free(cmd);
    return result;
}

#endif // __RUN_H
