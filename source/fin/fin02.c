#include "date.h"
#include "lib/catcher.h"
#include "lib/cimple/all.h"
#include "lib/run.h"
#define URL_BASE                                                                                   \
    "https://raw.githubusercontent.com/CSSEGISandData/COVID-19/master/csse_covid_19_data/"         \
    "csse_covid_19_daily_reports/"

typedef char* string;

void help() {
    Console.yellow("fin02:");
    Console.cyan("    -c, --country COUNTRY  : the country name. default: Taiwan");
    Console.cyan("    -s, --start MM-DD-YYYY : the start date. default: 01-23-2020.");
    Console.cyan("    -e, --end MM-DD-YYYY   : the end date. default: 12-31-2021.");
    Console.cyan("    -o, --output FILENAME  : output file name. default: output.csv");
    Console.cyan("    -h, --help             : This description");
}

i64 get_number(string date, string country);

i32 main(i32 argc, string argv[]) {
    setup_catcher();

    ParsedOptions* options = Options.parse(argc, argv);
    Options.key_only(options, "help");
    Options.key_only(options, "h");

    if (Options.has(options, "help") || Options.has(options, "h")) {
        help();
        return EXIT_SUCCESS;
    }

    string country = calloc(1024, sizeof(char));
    strcpy(country, "Taiwan");
    string start_date = calloc(1024, sizeof(char)), end_date = calloc(1024, sizeof(char));
    strcpy(start_date, "01-23-2020");
    strcpy(end_date, "12-31-2021");
    string output = calloc(1024, sizeof(char));
    strcpy(output, "output.csv");

    if (Options.has(options, "country") || Options.has(options, "c")) {
        if (!Options.get(options, "country") && !Options.get(options, "c")) {
            Console.red("Value of country is required.");
            return EXIT_FAILURE;
        }

        strcpy(country, Options.get(options, "country") ? Options.get(options, "country")
                                                        : Options.get(options, "c"));
    }

    if (Options.has(options, "start") || Options.has(options, "s")) {
        if (!Options.get(options, "start") && !Options.get(options, "s")) {
            Console.red("Value of start is required.");
            return EXIT_FAILURE;
        }

        strcpy(start_date, Options.get(options, "start") ? Options.get(options, "start")
                                                         : Options.get(options, "s"));
    }

    if (Options.has(options, "end") || Options.has(options, "e")) {
        if (!Options.get(options, "end") && !Options.get(options, "e")) {
            Console.red("Value of end is required.");
            return EXIT_FAILURE;
        }

        strcpy(end_date, Options.get(options, "end") ? Options.get(options, "end")
                                                     : Options.get(options, "e"));
    }

    if (Options.has(options, "output") || Options.has(options, "o")) {
        if (!Options.get(options, "output") && !Options.get(options, "o")) {
            Console.red("Value of output is required.");
            return EXIT_FAILURE;
        }

        strcpy(output, Options.get(options, "output") ? Options.get(options, "output")
                                                      : Options.get(options, "o"));
    }

    i32 start_idx = -1, end_idx = -1;
    for (i32 i = 0; i < dates_size; i++) {
        if (strcmp(dates[i], start_date) == 0) {
            start_idx = i;
        }
        if (strcmp(dates[i], end_date) == 0) {
            end_idx = i;
        }
    }

    if (start_idx == -1 || end_idx == -1) {
        Console.red("Invalid start or end date.");
        return EXIT_FAILURE;
    }

    if (start_idx > end_idx) {
        Console.red("Start date must be earlier than end date.");
        return EXIT_FAILURE;
    }

    if (start_idx == 0) {
        Console.red(
            "Start date must be later than 01-22-2020 because we need the previous day's data.");
        return EXIT_FAILURE;
    }

    i64    total = 0, prev = get_number(dates[start_idx - 1], country);
    string out = calloc(16 * 1024 * 1024, sizeof(char));
    sprintf(out, "Date, New infected\n");
    printf("Date, New infected\n");
    i64 offset = 19;
    for (i32 i = start_idx; i <= end_idx; i++) {
        i64 curr = get_number(dates[i], country);

        i64 new = curr - prev;
        total += new;
        prev = curr;

        string log = calloc(1024, sizeof(char));
        sprintf(log, "%s,%" PRId64 "\n", dates[i], new);
        sprintf(out + offset, "%s", log);
        printf("%s", log);
        offset += strlen(log);
        free(log);
    }
    i64    avg = total / (end_idx - start_idx + 1);
    string log = calloc(1024, sizeof(char));
    sprintf(log, "Average,%" PRId64 "\n", avg);
    sprintf(out + offset, "%s", log);
    printf("%s", log);
    offset += strlen(log);
    free(log);

    File.write(output, out, offset);

    Options.free(options);
    return EXIT_SUCCESS;
}

i64 get_number(string date, string country) {
    string command = $(calloc(1024, sizeof(char)));
    i32    year, month, day;
    sscanf(date, "%d-%d-%d", &month, &day, &year);
    if (year == 2020 && (month <= 2 || (month == 3 && day <= 21))) {
        sprintf(
            command,
            "curl -s %s%s.csv | grep \"%s\\*\\?,\" | awk -F ',' '{ print $4}' | awk '{s+=$1} END "
            "{print s}'",
            URL_BASE, date, country);
    } else {
        sprintf(
            command,
            "curl -s %s%s.csv | grep \"%s\\*\\?,\" | awk -F ',' '{ print $8}' | awk '{s+=$1} END "
            "{print s}'",
            URL_BASE, date, country);
    }
    string result = $(String.trim((string)$(run(command, NULL)), " \n\r"));
    i64    number = strtol(result, NULL, 10);
    $free();
    return number;
}
