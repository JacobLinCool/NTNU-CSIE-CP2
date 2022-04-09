// #define DEBUG
#include "utils.h"
#define DATA_FILENAME_MAX 1024

StructVector(Years, i64, INT64_MIN);
StructVector(Populations, i64, INT64_MIN);

typedef struct Country {
    string name;
    string code;
    string indicator_name;
    string indicator_code;
    Populations* populations;
} Country;

StructVector(Countries, Country*, NULL);

typedef struct Data {
    string source;
    string date;
    Years* years;
    Countries* countries;
} Data;

Data* read_csv(string path);
void print_menu();
i64 year_to_index(Data* data, i64 year);
string get_largest_population_growth(Data* data, i64 from, i64 to);
string get_largest_population_growth_rate(Data* data, i64 from, i64 to);
string get_smallest_population_growth(Data* data, i64 from, i64 to);
string get_smallest_population_growth_rate(Data* data, i64 from, i64 to);
string get_largest_population_standard_deviation(Data* data, i64 from, i64 to);
string get_smallest_population_standard_deviation(Data* data, i64 from, i64 to);
i64 predict_population(Data* data, string name, i64 from, i64 to);

i32 main() {
    string filename = calloc(DATA_FILENAME_MAX, sizeof(char));
    printf("Please enter the data name: ");
    THROW_IF(scanf("%[^\n]%*c", filename) != 1, "Invalid Filename");

    Data* data = read_csv(filename);
    DBG(
        VectorInspect(data->years, "%" PRId64);
    );

    i64 start_year = 0, end_year = 0, start_index = -1, end_index = -1;
    printf("Please enter the analysis start time: ");
    THROW_IF(scanf("%" SCNd64, &start_year) != 1, "Invalid Start Year");
    THROW_IF((start_index = year_to_index(data, start_year)) == -1, "Invalid Start Year");
    printf("Please enter the analysis end time  : ");
    THROW_IF(scanf("%" SCNd64, &end_year) != 1, "Invalid End Year");
    THROW_IF((end_index = year_to_index(data, end_year)) == -1, "Invalid End Year");
    THROW_IF(start_index >= end_index, "Start Year must be less than End Year");

    i64 choice = 0;
    while (true) {
        print_menu();
        i64 status = scanf("%" SCNd64, &choice);
        if (status != 1 || choice < 1 || choice > 8) {
            fprintf(stderr, "Invalid Choice. Please try again in the range 1 ~ 8\n");
            scanf("%*[^\n]%*c");
            continue;
        }

        switch (choice) {
        case 1:
            printf("%s\n", get_largest_population_growth(data, start_index, end_index));
            break;
        case 2:
            printf("%s\n", get_largest_population_growth_rate(data, start_index, end_index));
            break;
        case 3:
            printf("%s\n", get_smallest_population_growth(data, start_index, end_index));
            break;
        case 4:
            printf("%s\n", get_smallest_population_growth_rate(data, start_index, end_index));
            break;
        case 5:
            printf("%s\n", get_largest_population_standard_deviation(data, start_index, end_index));
            break;
        case 6:
            printf("%s\n", get_smallest_population_standard_deviation(data, start_index, end_index));
            break;
        case 7:
            printf("Please enter the country name: ");
            string country_name = calloc(1024, sizeof(char));
            THROW_IF(scanf("%s", country_name) != 1, "Invalid Country Name");
            printf("%" PRId64 "\n", predict_population(data, country_name, start_index, end_index));
            free(country_name);
            break;
        case 8:
            printf("Bye!\n");
            return EXIT_SUCCESS;
        default:
            continue;
        }
    }

    return EXIT_SUCCESS;
}

// API_SP.POP.TOTL_DS2_en_csv_v2_3731322.csv
Data* read_csv(string path) {
    FILE* fp = fopen(path, "r");
    THROW_IF(fp == NULL, "Failed to open data source");

    i64 size = 0;
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    if (!(size > 3 && fgetc(fp) == 0xEF && fgetc(fp) == 0xBB && fgetc(fp) == 0xBF)) {
        fseek(fp, 0, SEEK_SET);
    }

    Data* data = calloc(1, sizeof(Data));
    data->source = calloc(1024, sizeof(char));
    data->date = calloc(1024, sizeof(char));
    data->years = create_Years();
    data->countries = create_Countries();

    fscanf(fp, "\"Data Source\",\"%[^\"]\",%*[\n\r]", data->source);
    fscanf(fp, "\"Last Updated Date\",\"%[^\"]\",%*[\n\r]", data->date);

    data->source = realloc(data->source, strlen(data->source) + 1);
    data->date = realloc(data->date, strlen(data->date) + 1);

    fscanf(fp, "\"Country Name\",\"Country Code\",\"Indicator Name\",\"Indicator Code\",");
    i64 year = INT64_MIN;
    while (fscanf(fp, "\"%" SCNd64 "\",", &year) == 1) {
        if (year != INT64_MIN) {
            data->years->push(data->years, year);
        }
    }
    fscanf(fp, "%*[\n\r]");

    DBG_PRINT("Size: %" PRId64 "\n", size);
    DBG_PRINT("Source: %s\n", data->source);
    DBG_PRINT("Date: %s\n", data->date);

    while (true) {
        Country* country = calloc(1, sizeof(Country));
        country->name = calloc(1024, sizeof(char));
        country->code = calloc(1024, sizeof(char));
        country->indicator_name = calloc(1024, sizeof(char));
        country->indicator_code = calloc(1024, sizeof(char));
        country->populations = create_Populations();

        i64 status = fscanf(fp, "\"%[^\"]\",\"%[^\"]\",\"%[^\"]\",\"%[^\"]\",", country->name, country->code, country->indicator_name, country->indicator_code);
        if (status != 4) {
            free(country->name), free(country->code), free(country->indicator_name), free(country->indicator_code);
            free(country);
            break;
        }
        DBG_PRINT("%s, %s, %s, %s\n", country->name, country->code, country->indicator_name, country->indicator_code);
        country->name = realloc(country->name, strlen(country->name) + 1);
        country->code = realloc(country->code, strlen(country->code) + 1);
        country->indicator_name = realloc(country->indicator_name, strlen(country->indicator_name) + 1);
        country->indicator_code = realloc(country->indicator_code, strlen(country->indicator_code) + 1);

        i64 population = INT64_MIN;
        while (true) {
            string x = calloc(64, sizeof(char));
            if (fscanf(fp, "\"%" SCNd64 "\",", &population) == 1) {
                country->populations->push(country->populations, population);
            }
            else if (fscanf(fp, "%[^,\n\r],", x) == 1) {
                country->populations->push(country->populations, 0);
            }
            else {
                free(x);
                break;
            }
            free(x);
        }
        THROW_IF(
            country->populations->size != data->years->size,
            "Invalid data of country \"%s\". Please check if the data is correct or contact the developer.",
            country->name
        );

        data->countries->push(data->countries, country);
        fscanf(fp, "%*[\n\r]");
    }

    fclose(fp);
    return data;
}

void print_menu() {
    printf("--- Menu ---\n");
    printf("1) Which country has the largest population growth?\n");
    printf("2) Which country has the largest population growth rate?\n");
    printf("3) Which country has the smallest population growth, including negative growth?\n");
    printf("4) Which country has the smallest population growth rate, including negative growth?\n");
    printf("5) Which country has the largest population standard deviation?\n");
    printf("6) Which country has the smallest population standard deviation?\n");
    printf("7) Given a country name (case-insensitive), please use linear least squares method to predict the country's population in 2021.\n");
    printf("8) Exit\n");
    printf("Your choice: ");
}

i64 year_to_index(Data* data, i64 year) {
    for (i64 i = 0; i < data->years->size; i++) {
        if (data->years->get(data->years, i) == year) {
            return i;
        }
    }
    return -1;
}

string get_largest_population_growth(Data* data, i64 from, i64 to) {
    i64 max_index = 0;
    i64 max_growth = INT64_MIN;
    for (i64 i = 0; i < data->countries->size; i++) {
        Country* country = data->countries->get(data->countries, i);
        i64 growth = country->populations->get(country->populations, to) - country->populations->get(country->populations, from);
        if (growth > max_growth) {
            max_index = i;
            max_growth = growth;
        }
    }
    return data->countries->get(data->countries, max_index)->name;
}

string get_largest_population_growth_rate(Data* data, i64 from, i64 to) {
    i64 max_index = 0;
    f64 max_growth_rate = INT64_MIN;
    f64 span = data->years->get(data->years, to) - data->years->get(data->years, from);
    for (i64 i = 0; i < data->countries->size; i++) {
        Country* country = data->countries->get(data->countries, i);
        f64 growth_rate = (country->populations->get(country->populations, to) - country->populations->get(country->populations, from)) / span;
        if (growth_rate > max_growth_rate) {
            max_index = i;
            max_growth_rate = growth_rate;
        }
    }
    return data->countries->get(data->countries, max_index)->name;
}

string get_smallest_population_growth(Data* data, i64 from, i64 to) {
    i64 min_index = 0;
    i64 min_growth = INT64_MAX;
    for (i64 i = 0; i < data->countries->size; i++) {
        Country* country = data->countries->get(data->countries, i);
        i64 growth = country->populations->get(country->populations, to) - country->populations->get(country->populations, from);
        if (growth < min_growth) {
            min_index = i;
            min_growth = growth;
        }
    }
    return data->countries->get(data->countries, min_index)->name;
}

string get_smallest_population_growth_rate(Data* data, i64 from, i64 to) {
    i64 min_index = 0;
    f64 min_growth_rate = INT64_MAX;
    f64 span = data->years->get(data->years, to) - data->years->get(data->years, from);
    for (i64 i = 0; i < data->countries->size; i++) {
        Country* country = data->countries->get(data->countries, i);
        f64 growth_rate = (country->populations->get(country->populations, to) - country->populations->get(country->populations, from)) / span;
        if (growth_rate < min_growth_rate) {
            min_index = i;
            min_growth_rate = growth_rate;
        }
    }
    return data->countries->get(data->countries, min_index)->name;
}

f64 get_population_standard_deviation(Country* country, i64 from, i64 to) {
    f64 mean = 0;
    for (i64 i = from; i <= to; i++) {
        mean += country->populations->get(country->populations, i);
    }
    mean /= (to - from + 1);

    f64 sum_of_squares = 0;
    for (i64 i = from; i <= to; i++) {
        sum_of_squares += pow(country->populations->get(country->populations, i) - mean, 2);
    }

    return sqrt(sum_of_squares / (to - from + 1));
}

string get_largest_population_standard_deviation(Data* data, i64 from, i64 to) {
    i64 max_index = 0;
    f64 max_standard_deviation = INT64_MIN;
    for (i64 i = 0; i < data->countries->size; i++) {
        Country* country = data->countries->get(data->countries, i);
        f64 standard_deviation = get_population_standard_deviation(country, from, to);
        if (standard_deviation > max_standard_deviation) {
            max_index = i;
            max_standard_deviation = standard_deviation;
        }
    }
    return data->countries->get(data->countries, max_index)->name;
}

string get_smallest_population_standard_deviation(Data* data, i64 from, i64 to) {
    i64 min_index = 0;
    f64 min_standard_deviation = INT64_MAX;
    for (i64 i = 0; i < data->countries->size; i++) {
        Country* country = data->countries->get(data->countries, i);
        f64 standard_deviation = get_population_standard_deviation(country, from, to);
        if (standard_deviation < min_standard_deviation) {
            min_index = i;
            min_standard_deviation = standard_deviation;
        }
    }
    return data->countries->get(data->countries, min_index)->name;
}

static inline string string_to_lower(const string raw) {
    string result = calloc(strlen(raw) + 1, sizeof(char));
    for (i64 i = 0; raw[i] != '\0'; i++) {
        result[i] = tolower(raw[i]);
    }
    return result;
}

static inline Country* get_country(Data* data, const string name) {
    string target = string_to_lower(name);
    for (i64 i = 0; i < data->countries->size; i++) {
        Country* country = data->countries->get(data->countries, i);
        string country_name = string_to_lower(country->name);
        if (strcmp(country_name, target) == 0) {
            free(country_name), free(target);
            return country;
        }
        free(country_name);
    }

    free(target);
    return NULL;
}

i64 predict_population(Data* data, string name, i64 from, i64 to) {
    Country* country = get_country(data, name);
    if (country == NULL) {
        return -1;
    }

    i64 from_year = data->years->get(data->years, from), to_year = data->years->get(data->years, to);
    f64 span = to_year - from_year;
    f64 sum_of_x = 0, sum_of_y = 0, sum_of_xy = 0, sum_of_x_squares = 0;
    for (i64 i = from; i <= to; i++) {
        sum_of_x += data->years->get(data->years, i);
        sum_of_y += country->populations->get(country->populations, i);
        sum_of_xy += data->years->get(data->years, i) * country->populations->get(country->populations, i);
        sum_of_x_squares += pow(data->years->get(data->years, i), 2);
    }

    f64 avg_of_x = sum_of_x / (to - from + 1);
    f64 avg_of_y = sum_of_y / (to - from + 1);
    f64 avg_of_xy = sum_of_xy / (to - from + 1);
    f64 avg_of_x_squares = sum_of_x_squares / (to - from + 1);

    f64 a = (avg_of_xy - avg_of_x * avg_of_y) / (avg_of_x_squares - avg_of_x * avg_of_x);
    f64 b = avg_of_y - a * avg_of_x;

    return (i64)(a * 2021 + b);
}
