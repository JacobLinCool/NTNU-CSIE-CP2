#include "lib/cimple/all.h"
#include "lib/run.h"
#include "lib/file.h"
#define SIZE (3 * 1024 * 1024UL)

typedef char* string;

typedef struct WAV {
    struct header {
        char riff[4];
        uint32_t size;
        char wave[4];
        char fmt[4];
        uint32_t fmt_size;
        uint16_t format;
        uint16_t channels;
        uint32_t sample_rate;
        uint32_t byte_rate;
        uint16_t block_align;
        uint16_t bits_per_sample;
        char data[4];
        uint32_t data_size;
    } header;
    uint8_t data[SIZE];
} WAV;

void download_wav(string content) {
    string escaped = $(String.replace(content, " ", "+"));
    string url = $(String.format("https://hapsing.ithuan.tw/bangtsam?taibun=%s", escaped));
    string command = $(String.format("curl -o '%swav' %s", escaped, url));

    Timing.start(escaped);
    $(run(command, NULL));
    Console.success("Downloaded \"%swav\" in %.1Lf ms.", escaped, Timing.check(escaped));
    $free();
}

void transform(string raw) {
    size_t len = strlen(raw);

    if (len == 0) {
        return;
    }

    if (raw[len - 1] == '1') {
        raw[len - 1] = '7';
    }
    else if (raw[len - 1] == '2') {
        raw[len - 1] = '1';
    }
    else if (raw[len - 1] == '3') {
        raw[len - 1] = '2';
    }
    else if (raw[len - 1] == '4' && len > 1) {
        if (strchr("ptk", raw[len - 2]) != NULL) {
            raw[len - 1] = '8';
        }
        else if (raw[len - 2] == 'h') {
            raw[len - 1] = '2';
        }
    }
    else if (raw[len - 1] == '5') {
        raw[len - 1] = '7';
    }
    else if (raw[len - 1] == '7') {
        raw[len - 1] = '3';
    }
    else if (raw[len - 1] == '8' && len > 1) {
        if (strchr("ptk", raw[len - 2]) != NULL) {
            raw[len - 1] = '4';
        }
        else if (raw[len - 2] == 'h') {
            raw[len - 1] = '3';
        }
    }
}

string* parse(string content, size_t* count) {
    string* slices = String.split($(String.replace(content, "-", " ")), " ", count);

    for (size_t i = 0; i < *count; i++) {
        transform(slices[i]);
        size_t len = strlen(slices[i]);
        if (slices[i][len - 1] != '.') {
            slices[i] = realloc(slices[i], len + 2);
            strcat(slices[i], ".");
        }
    }

    $free();
    return slices;
}

i32 main(i32 argc, string argv[]) {
    printf(__FMT_GRAY "input: " __FMT_NONE);
    string content = calloc(64, sizeof(char));
    scanf("%[^\n]%*c", content);
    if (content == NULL) {
        Console.error("Failed to get input!");
        return EXIT_FAILURE;
    }

    size_t count;
    string* slices = parse(content, &count);

    printf(__FMT_GRAY "output: " __FMT_NONE);
    for (size_t i = 0; i < count; i++) {
        printf("%s ", slices[i]);
    }
    printf("%s\n", content);

    download_wav(content);
    for (size_t i = 0; i < count; i++) {
        download_wav(slices[i]);
    }

    Timing.start("concat");

    WAV* wav = calloc(1, sizeof(WAV));
    memcpy(&wav->header, "RIFF", 4);
    wav->header.size = 36;
    memcpy(&wav->header.wave, "WAVE", 4);
    memcpy(&wav->header.fmt, "fmt ", 4);
    wav->header.fmt_size = 16;
    wav->header.format = 1;
    wav->header.channels = 1;
    wav->header.sample_rate = 16000U;
    wav->header.byte_rate = 48000U;
    wav->header.block_align = 3;
    wav->header.bits_per_sample = 24;
    memcpy(&wav->header.data, "data", 4);
    wav->header.data_size = 0;

    for (size_t i = 0; i < count; i++) {
        string filename = $(String.format("%swav", slices[i]));
        FileInfo* file = read_file(filename);

        WAV* sub = ((WAV*)file->data);
        memcpy(wav->data + wav->header.data_size, sub->data, sub->header.data_size);
        wav->header.data_size += sub->header.data_size;
        wav->header.size += sub->header.data_size;

        Console.success("Concatenated \"%s\".", filename);
    }

    string filename = $(String.format("%swav", (String.replace(content, " ", "+"))));
    FileInfo* file = read_file(filename);

    WAV* sub = ((WAV*)file->data);

    memcpy(wav->data + wav->header.data_size, sub->data, sub->header.data_size);
    wav->header.data_size += sub->header.data_size;
    wav->header.size += sub->header.data_size;

    Console.success("Concatenated \"%s\".", filename);

    FileInfo* out = calloc(1, sizeof(FileInfo));
    out->data = (uint8_t*)wav;
    out->size = wav->header.size;
    out->path = "out.wav";
    write_file(out);

    Console.success("Concatenated all sounds and wrote \"out.wav\" (%d bytes) in %.1Lf ms.", out->size, Timing.check("concat"));

    $free();
    return EXIT_SUCCESS;
}
