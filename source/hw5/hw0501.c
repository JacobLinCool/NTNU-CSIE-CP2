#include "lib/cimple/all.h"
#include "lib/run.h"

typedef char* string;

void help() {
    Console.yellow("Usage:");
    Console.yellow("\thw0501 [options]");
    Console.cyan("Options:");
    Console.cyan("\t-b,\t--bytes\t\tShow output in bytes");
    Console.cyan("\t\t--kilo\t\tShow output in kilobytes %s(default)", Console.C.MAGENTA);
    Console.cyan("\t\t--mega\t\tShow output in megabytes");
    Console.cyan("\t-s N,\t--seconds N\tRepeat printing every N seconds");
    Console.cyan("\t-c N,\t--count N\tRepeat printing N times , then exit");
    Console.cyan("\t-h,\t--help\t\tDisplay this help and exit");
}

i32 main(i32 argc, string argv[]) {
#ifndef __linux__
    Console.error("This program is only for linux system.");
    return EXIT_FAILURE;
#endif

    ParsedOptions* options = Options.parse(argc, argv);

    if (Options.has(options, "help") || Options.has(options, "h")) {
        help();
        return EXIT_SUCCESS;
    }

    i64 divider = -1;
    string unit;
    if (Options.has(options, "bytes") || Options.has(options, "b")) {
        divider = 1;
        unit = "B";
    }
    if (Options.has(options, "kilo")) {
        if (divider != -1) {
            Console.error("You can only specify one unit.");
            return EXIT_FAILURE;
        }
        divider = 1000;
        unit = "KB";
    }
    if (Options.has(options, "mega")) {
        if (divider != -1) {
            Console.error("You can only specify one unit.");
            return EXIT_FAILURE;
        }
        divider = 1000000;
        unit = "MB";
    }
    if (divider == -1) {
        divider = 1000;
        unit = "KB";
    }

    i64 interval = 1000; // milliseconds
    if (Options.has(options, "seconds") || Options.has(options, "s")) {
        if (Options.get(options, "seconds") == NULL && Options.get(options, "s") == NULL) {
            Console.error("\"seconds\" option detected without value!");
            help();
            return EXIT_FAILURE;
        }

        char* end;
        interval = strtol(Options.get(options, "seconds") == NULL ? Options.get(options, "s") : Options.get(options, "seconds"), &end, 10) * 1000;
        if (*end != '\0' || interval < 0) {
            Console.error("Invalid value of \"seconds\"!");
            help();
            return EXIT_FAILURE;
        }
    }

    i64 repeat = 1;
    if (Options.has(options, "count") || Options.has(options, "c")) {
        if (Options.get(options, "count") == NULL && Options.get(options, "c") == NULL) {
            Console.error("\"count\" option detected without value!");
            help();
            return EXIT_FAILURE;
        }

        char* end;
        repeat = strtol(Options.get(options, "count") == NULL ? Options.get(options, "c") : Options.get(options, "count"), &end, 10);
        if (*end != '\0' || repeat < 0) {
            Console.error("Invalid value of \"count\"!");
            help();
            return EXIT_FAILURE;
        }
    }

    i64 total = 0, available = 0;

    for (i64 i = 0; i < repeat; i++) {
        size_t size = 0;
        string* meminfo = $(String.split($(run("grep -m 3 -o \"[0-9]*\" /proc/meminfo", NULL)), "\n", &size));
        for (size_t j = 0; j < size; j++) { $(meminfo[j]); }
        total = strtol(meminfo[0], NULL, 10) * 1024 / divider;
        available = strtol(meminfo[2], NULL, 10) * 1024 / divider;
        $free();

        Console.gray("[%*" PRId64 "]" __FMT_CYAN " Available: %" PRId64 " %s (%.2Lf%%)", (int)log10(repeat) + 1, i + 1, available, unit, available * 100.0L / total);

        if (i < repeat - 1) {
            Timing.sleep(interval);
        }
    }

    Console.yellow("Tip: Available = Free + Buffers + Cached");
    return EXIT_SUCCESS;
}
