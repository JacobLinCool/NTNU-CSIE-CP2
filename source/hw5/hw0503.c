#include "lib/cimple/all.h"

typedef char* string;

typedef enum Mode {
    NONE,
    ENCODE,
    DECODE,
} Mode;

void help() {
    Console.yellow("Usage:");
    Console.yellow("\t./hw0503 [options]");
    Console.cyan("Options:");
    Console.cyan("\t-e, --encode <file>\tEncode a file to a text file. (binary to base64)");
    Console.cyan("\t-d, --decode <file>\tDecode a text file to a file. (base64 to binary)");
    Console.cyan("\t-o, --output <file>\tOutput file name.");
    Console.cyan("\t-h, --help         \tDisplay this help.");
    Console.magenta("Example:");
    Console.magenta("\t./hw0503 -e maldives.bmp -o maldives.txt");
    Console.magenta("\t./hw0503 -d maldives.txt -o maldives.bmp");
    Console.blue("Contact: JacobLinCool <hi@jacoblin.cool> (https://github.com/JacobLinCool)");
}

char* base64_encode(uint8_t input[], size_t input_size, size_t* output_size) {
    string dict = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    size_t size = (input_size + 2) / 3 * 4;
    char* output = calloc(size + 1, sizeof(char));

    for (size_t idx_in = 0, idx_out = 0; idx_in < input_size; idx_in += 3, idx_out += 4) {
        uint32_t block = 0, padding = 0;
        for (size_t i = 0; i < 3; i++) {
            block <<= 8;
            if (idx_in + i < input_size) {
                block |= input[idx_in + i];
            }
            else {
                padding++;
            }
        }
        for (size_t i = 0; i < 4; i++) {
            if (3 - i < padding) {
                output[idx_out + i] = '=';
            }
            else {
                output[idx_out + i] = dict[(block >> (6 * (3 - i))) & 0x3F];
            }
        }
    }

    *output_size = size;
    return output;
}

uint8_t reversed(char c) {
    if (c == 43) {
        return 62;
    }
    if (c == 47) {
        return 63;
    }
    if (c == 61) {
        return 64;
    }
    if ((c > 47) && (c < 58)) {
        return c + 4;
    }
    if ((c > 64) && (c < 91)) {
        return c - 'A';
    }
    if ((c > 96) && (c < 123)) {
        return 26 + (c - 'a');
    }
    return 0;
}

uint8_t* base64_decode(char input[], size_t input_size, size_t* output_size) {
    size_t size = input_size / 4 * 3;
    uint8_t* output = calloc(size + 1, sizeof(uint8_t));

    uint32_t padding = 0;
    for (size_t idx_in = 0, idx_out = 0; idx_in < input_size; idx_in += 4, idx_out += 3) {
        uint32_t block = 0;
        for (size_t i = 0; i < 4; i++) {
            block <<= 6;
            if (input[idx_in + i] != '=') {
                block |= reversed(input[idx_in + i]);
            }
            else {
                padding++;
            }
        }
        for (size_t i = 0; i < 3 - padding; i++) {
            output[idx_out + i] = (block >> (8 * (2 - i))) & 0xFF;
        }
    }

    *output_size = size - padding;
    return output;
}

i32 main(i32 argc, string argv[]) {
    ParsedOptions* options = Options.parse(argc, argv);

    if (Options.has(options, "help") || Options.has(options, "h")) {
        help();
        return EXIT_SUCCESS;
    }

    Mode mode = NONE;
    string input;
    if (Options.get(options, "encode") || Options.get(options, "e")) {
        mode = ENCODE;
        input = Options.get(options, "encode") == NULL ? Options.get(options, "e") : Options.get(options, "encode");
    }
    else if (Options.get(options, "decode") || Options.get(options, "d")) {
        mode = DECODE;
        input = Options.get(options, "decode") == NULL ? Options.get(options, "d") : Options.get(options, "decode");
    }
    else {
        Console.error("You must specify \"encode\" or \"decode\" option!");
        help();
        return EXIT_FAILURE;
    }

    string output = Options.get(options, "output") == NULL ? Options.get(options, "o") : Options.get(options, "output");
    if (output == NULL) {
        Console.error("You must specify \"output\" option!");
        help();
        return EXIT_FAILURE;
    }

    if (mode == ENCODE) {
        uint8_t* input_data = NULL;
        size_t input_size = 0;
        FILE* input_file = fopen(input, "rb");
        if (input_file == NULL) {
            Console.error("Failed to open input file \"%s\"!", input);
            return EXIT_FAILURE;
        }
        fseek(input_file, 0, SEEK_END);
        input_size = ftell(input_file);
        fseek(input_file, 0, SEEK_SET);
        input_data = $(calloc(input_size, sizeof(uint8_t)));
        fread(input_data, sizeof(uint8_t), input_size, input_file);
        fclose(input_file);

        size_t output_size = 0;
        Timing.start("encode");
        char* output_data = $(base64_encode(input_data, input_size, &output_size));
        Console.success("Encoded %d bytes to %d bytes in %.2Lf ms!", input_size, output_size, Timing.check("encode"));
        FILE* output_file = fopen(output, "w");
        if (output_file == NULL) {
            Console.error("Failed to open output file \"%s\"!", output);
            return EXIT_FAILURE;
        }
        fwrite(output_data, sizeof(char), output_size, output_file);
        fclose(output_file);
        $free();
    }
    else if (mode == DECODE) {
        string input_data = NULL;
        size_t input_size = 0;
        FILE* input_file = fopen(input, "r");
        if (input_file == NULL) {
            Console.error("Failed to open input file \"%s\"!", input);
            return EXIT_FAILURE;
        }
        fseek(input_file, 0, SEEK_END);
        input_size = ftell(input_file);
        fseek(input_file, 0, SEEK_SET);
        input_data = $(calloc(input_size, sizeof(char)));
        fread(input_data, sizeof(char), input_size, input_file);
        fclose(input_file);

        size_t output_size = 0;
        Timing.start("decode");
        uint8_t* output_data = $(base64_decode(input_data, input_size, &output_size));
        Console.success("Decoded %d bytes to %d bytes in %.2Lf ms!", input_size, output_size, Timing.check("decode"));
        FILE* output_file = fopen(output, "wb");
        if (output_file == NULL) {
            Console.error("Failed to open output file \"%s\"!", output);
            return EXIT_FAILURE;
        }
        fwrite(output_data, sizeof(uint8_t), output_size, output_file);
        fclose(output_file);
        $free();
    }

    return EXIT_SUCCESS;
}
