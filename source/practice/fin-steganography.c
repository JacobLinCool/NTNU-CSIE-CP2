#include "lib/bmp.h"
#include "lib/catcher.h"
#include "lib/cimple/all.h"

typedef char* string;

void help() {
    Console.yellow("./img_hide [option] [cover_bmp] [secret_data]");
    Console.cyan("\t-w, --write\t: Write the secret data to the cover_bmp.");
    Console.cyan(
        "\t-e, --extract\t: Extract the secret data from the cover_bmp to the secret_data.");
    Console.cyan("\t-b, --bits N\t: Use last N bits. N is from 1 to 8. The default N is 1.");
}

i32 main(i32 argc, string argv[]) {
    setup_catcher();

    bool           fixed = false;
    ParsedOptions* options = Options.parse(argc, argv);
    Options.key_only(options, "help");
    Options.key_only(options, "h");
    fixed |= Options.key_only(options, "write");
    fixed |= Options.key_only(options, "w");
    fixed |= Options.key_only(options, "extract");
    fixed |= Options.key_only(options, "e");

    if (Options.has(options, "help") || Options.has(options, "h")) {
        help();
        return EXIT_SUCCESS;
    }

    if (!((Options.has(options, "write") || Options.has(options, "w")) ^
          (Options.has(options, "extract") || Options.has(options, "e")))) {
        Console.red(
            "You must specify one and only one of the options: -w, --write or -e, --extract.");
        return EXIT_FAILURE;
    }

    uint8_t bits = 1;
    if (Options.get(options, "bits") || Options.get(options, "b")) {
        char* end = NULL;
        bits = (uint8_t)strtol(
            Options.get(options, "bits") ? Options.get(options, "bits") : Options.get(options, "b"),
            &end, 10);
        if (bits < 1 || bits > 8 || *end != '\0') {
            Console.red("The value of the option -b, --bits must be from 1 to 8.");
            return EXIT_FAILURE;
        }
    }

    if (Options.has(options, "write") || Options.has(options, "w")) {
        if (options->wild_count < 2) {
            Console.red("You must specify the cover_bmp and the secret_data.");
            return EXIT_FAILURE;
        }

        string bmp_path = fixed ? options->wilds[1] : options->wilds[0];
        string secret = fixed ? options->wilds[0] : options->wilds[1];

        BMP* img;
        u8   err = read_bmp(bmp_path, &img);
        if (err) {
            Console.red("Failed to read the bmp file. %s.", BMP_ERROR_MESSAGE[err]);
            return EXIT_FAILURE;
        }

        size_t secret_size = strlen(secret) * 8;

        if (secret_size >
            img->header->info_header.height * img->header->info_header.width * 3 * bits) {
            Console.red("The secret data is too large to hide.");
            return EXIT_FAILURE;
        }

        memcpy(&img->header->info_header.file_header.reserve_1, &secret_size, 4);

        size_t offset = 0;
        for (size_t y = 0; y < img->header->info_header.height; y++) {
            for (size_t x = 0; x < img->header->info_header.width; x++) {
                Pixel*   pixel = img->pixels[y][x];
                uint8_t* red = (uint8_t*)&pixel->red;
                uint8_t* green = (uint8_t*)&pixel->green;
                uint8_t* blue = (uint8_t*)&pixel->blue;

                for (size_t i = 0; i < bits; i++) {
                    if (offset >= secret_size) {
                        break;
                    }

                    Buffer.set_bit(red, i, Buffer.get_bit(secret, offset));
                    offset++;
                }

                for (size_t i = 0; i < bits; i++) {
                    if (offset >= secret_size) {
                        break;
                    }

                    Buffer.set_bit(green, i, Buffer.get_bit(secret, offset));
                    offset++;
                }

                for (size_t i = 0; i < bits; i++) {
                    if (offset >= secret_size) {
                        break;
                    }

                    Buffer.set_bit(blue, i, Buffer.get_bit(secret, offset));
                    offset++;
                }
            }
        }

        if (img->save(img, bmp_path, 8, 8, 8, 0)) {
            Console.red("Failed to write the bmp file.");
            return EXIT_FAILURE;
        }
    } else {
        if (options->wild_count < 1) {
            Console.red("You must specify the cover_bmp.");
            return EXIT_FAILURE;
        }

        string bmp_path = options->wilds[0];

        BMP* img;
        u8   err = read_bmp(bmp_path, &img);
        if (err) {
            Console.red("Failed to read the bmp file. %s.", BMP_ERROR_MESSAGE[err]);
            return EXIT_FAILURE;
        }

        size_t secret_size = 0;
        memcpy(&secret_size, &img->header->info_header.file_header.reserve_1, 4);

        if (secret_size == 0) {
            Console.red("There is no secret.");
            return EXIT_FAILURE;
        }

        u8* secret_data = malloc(((secret_size + 7) / 8) + 1);
        secret_data[secret_size / 8] = 0;

        size_t offset = 0;
        for (size_t y = 0; y < img->header->info_header.height; y++) {
            for (size_t x = 0; x < img->header->info_header.width; x++) {
                Pixel*   pixel = img->pixels[y][x];
                uint8_t* red = (uint8_t*)&pixel->red;
                uint8_t* green = (uint8_t*)&pixel->green;
                uint8_t* blue = (uint8_t*)&pixel->blue;

                for (size_t i = 0; i < bits; i++) {
                    if (offset >= secret_size) {
                        break;
                    }

                    Buffer.set_bit(secret_data, offset, Buffer.get_bit(red, i));
                    offset++;
                }

                for (size_t i = 0; i < bits; i++) {
                    if (offset >= secret_size) {
                        break;
                    }

                    Buffer.set_bit(secret_data, offset, Buffer.get_bit(green, i));
                    offset++;
                }

                for (size_t i = 0; i < bits; i++) {
                    if (offset >= secret_size) {
                        break;
                    }

                    Buffer.set_bit(secret_data, offset, Buffer.get_bit(blue, i));
                    offset++;
                }
            }
        }

        Console.green("The secret data is: \"%s\"", secret_data);
    }

    Options.free(options);
    return EXIT_SUCCESS;
}
