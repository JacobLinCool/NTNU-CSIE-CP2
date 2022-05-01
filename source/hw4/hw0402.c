#include "helper.h"
#include "lib/uds/vector.h"
#include "lib/string.h"
#define MAX_STRUCT_SIZE (2 * 1024 * 1024)

string template_h = \
"#pragma once\n\n" \
"#include <stdint.h>\n\n" \
"%s\n" \
"%s;\n" \
"%s;\n" \
"%s;\n" \
"%s;\n";

string template_init_body = \
"%s {\n" \
"    %s* %s = calloc(1, sizeof(%s));\n" \
"    return %s;\n" \
"}\n";

string template_free_body = \
"%s {\n" \
"    free(%s);\n" \
"    return;\n" \
"}\n";

string template_encode_body = \
"%s {\n" \
"    if (buffer == NULL || data == NULL) {\n" \
"        return -1;\n" \
"    }\n" \
"\n" \
"    uint8_t* buffer_ptr = buffer;\n" \
"    size_t from_bit = 0;\n" \
"    size_t data_offset = 0;\n" \
"    for (size_t i = 0; i < field_count; i++) {\n" \
"        size_t size = field_sizes[i];\n" \
"        put_buffer(buffer_ptr, from_bit, size, (uint8_t*)data + data_offset);\n" \
"        from_bit += size;\n" \
"        data_offset += (size + 7) / 8;\n" \
"    }\n" \
"\n" \
"    return 0;\n" \
"}\n";

string template_decode_body = \
"%s {\n" \
"    if (buffer == NULL || data == NULL) {\n" \
"        return -1;\n" \
"    }\n" \
"\n" \
"    const uint8_t* buffer_ptr = buffer;\n" \
"    size_t from_bit = 0;\n" \
"    size_t data_offset = 0;\n" \
"    for (size_t i = 0; i < field_count; i++) {\n" \
"        size_t size = field_sizes[i];\n" \
"        uint8_t* bin = take_buffer(buffer_ptr, from_bit, size);\n" \
"        memcpy((uint8_t*)data + data_offset, bin, (size + 7) / 8);\n" \
"        free(bin);\n" \
"        from_bit += size;\n" \
"        data_offset += (size + 7) / 8;\n" \
"    }\n" \
"\n" \
"    return 0;\n" \
"}\n";

string template_c = \
"#include \"%s.h\"\n\n" \
"#include <stdint.h>\n#include <stdlib.h>\n#include <string.h>\n\n" \
"%s\n" \
"%s\n" \
"%s\n" \
"%s\n" \
"%s\n" \
"%s\n";

string helper_functions = \
"static uint8_t* take_buffer(const uint8_t* buffer, size_t from_bit, size_t size) {\n" \
"    size_t offset = from_bit % 8;\n" \
"    size_t block_count = (size + offset + 7) / 8;\n" \
"    uint8_t* data = calloc(block_count, sizeof(uint8_t));\n" \
"    memcpy(data, buffer + from_bit / 8, block_count);\n" \
"\n" \
"    if (offset != 0) {\n" \
"        data[0] <<= offset;\n" \
"        for (size_t i = 1; i < block_count; i++) {\n" \
"            data[i - 1] |= data[i] >> (8 - offset);\n" \
"            data[i] <<= offset;\n" \
"        }\n" \
"    }\n" \
"\n" \
"    size_t last_block_size = size % 8;\n" \
"    if (last_block_size != 0) {\n" \
"        data[block_count - 1] &= ((1 << last_block_size) - 1) << (8 - last_block_size);\n" \
"    }\n" \
"    return data;\n" \
"}\n" \
"\n" \
"static void put_buffer(uint8_t* buffer, size_t from_bit, size_t size, const uint8_t* data) {\n" \
"    size_t block_count = (size + 7) / 8;\n" \
"    size_t offset = from_bit % 8;\n" \
"    size_t start_byte = from_bit / 8;\n" \
"    for (size_t i = 0; i < block_count; i++) {\n" \
"        buffer[start_byte + i] |= data[i] >> offset;\n" \
"        buffer[start_byte + i + 1] |= data[i] << (8 - offset);\n" \
"    }\n" \
"    return;\n" \
"}\n";

void print_bits(const uint8_t* buffer, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%d", (buffer)[i / 8] >> (7 - i % 8) & 1);
        if (i % 8 == 7) {
            printf(" ");
        }
    }
    printf("\n");
    return;
}

void remove_spaces(string s) {
    char* d = s;
    do {
        while (*d == ' ') {
            ++d;
        }
    } while ((*s++ = *d++));
}

string to_lower(string str) {
    string result = calloc(strlen(str) + 1, sizeof(char));
    for (i32 i = 0; i < strlen(str); i++) {
        result[i] = tolower(str[i]);
    }
    return result;
}

string to_upper(string str) {
    string result = calloc(strlen(str) + 1, sizeof(char));
    for (i32 i = 0; i < strlen(str); i++) {
        result[i] = toupper(str[i]);
    }
    return result;
}

typedef struct Field {
    string name;
    size_t size;
} Field;

StructVector(Fields, Field*, NULL);

string format(const restrict string format, ...) {
    THROW_IF(format == NULL, "Cannot format a null string.");

    va_list args;
    va_start(args, format);
    u64    len = vsnprintf(NULL, 0, format, args);
    va_end(args);
    string str = calloc(len + 1, sizeof(char));
    va_start(args, format);
    vsprintf(str, format, args);
    va_end(args);

    return str;
}

Fields* parse_fields(string input) {
    input = strdup(input);
    Fields* fields = create_Fields();

    string line = strtok(input, "\n");
    while (line != NULL) {
        u64 len = strlen(line);

        if (line[0] == '|') {
            string name = strtok(line + 1, "|");
            while (name != NULL) {
                string trimed = TRIM(name);
                u64 size = (strlen(name) + 1) / 2;

                Field* field = calloc(1, sizeof(Field));
                field->name = trimed;
                field->size = size;
                fields->push(fields, field);

                name = strtok(NULL, "|");
            }
        }

        line = strtok(line + len + 1, "\n");
    }

    free(input);
    return fields;
}

string generate_struct(string name, Fields* fields) {
    string struct_body = calloc(MAX_STRUCT_SIZE, sizeof(char));
    size_t offset = 0;

    string head = calloc(1024, sizeof(char));
    sprintf(head, "typedef struct _s%s {\n", name);
    string tail = calloc(1024, sizeof(char));
    sprintf(tail, "}__attribute__((packed)) %s;\n", name);

    strcpy(struct_body, head);
    offset += strlen(head);
    free(head);

    for (i32 i = 0; i < fields->size; i++) {
        Field* field = fields->get(fields, i);

        if (field->size <= 8 || field->size == 16 || field->size == 32 || field->size == 64) {
            string field_body = calloc(1024, sizeof(char));
            string name = strdup(field->name);
            remove_spaces(name);
            string lower = to_lower(name);
            sprintf(field_body, "    uint%zu_t %s; // %zu bit(s)\n", (field->size + 7) / 8 * 8, lower, field->size);
            strcpy(struct_body + offset, field_body);
            offset += strlen(field_body);
            free(field_body), free(name), free(lower);
        }
        else {
            string field_body = calloc(1024, sizeof(char));
            string name = strdup(field->name);
            remove_spaces(name);
            string lower = to_lower(name);
            sprintf(field_body, "    uint8_t %s[%zu]; // %zu bit(s)\n", lower, (field->size + 7) / 8, field->size);
            strcpy(struct_body + offset, field_body);
            offset += strlen(field_body);
            free(field_body), free(name), free(lower);
        }
    }

    strcpy(struct_body + offset, tail);
    offset += strlen(tail);
    free(tail);

    return struct_body;
}

string generate_sizes(Fields* fields) {
    string sizes_body = calloc(MAX_STRUCT_SIZE, sizeof(char));
    size_t offset = 0;

    strcpy(sizes_body, "static const size_t field_sizes[] = { ");
    offset += strlen("static const size_t field_sizes[] = { ");

    for (i32 i = 0; i < fields->size; i++) {
        Field* field = fields->get(fields, i);

        string size_body = calloc(1024, sizeof(char));
        sprintf(size_body, "%zu", field->size);
        strcpy(sizes_body + offset, size_body);
        offset += strlen(size_body);
        free(size_body);
        if (i != fields->size - 1) {
            strcpy(sizes_body + offset, ", ");
            offset += strlen(", ");
        }
    }

    strcpy(sizes_body + offset, " };\n");
    offset += strlen(" };\n");

    string field_count = calloc(64, sizeof(char));
    sprintf(field_count, "static const size_t field_count = %zu;\n", fields->size);
    strcpy(sizes_body + offset, field_count);
    offset += strlen(field_count);

    return sizes_body;
}

i32 main(i32 argc, string argv[]) {
    string options[] = { "i:", "n?:", "p?:" };
    string parsed[3] = { NULL };
    if (parse_options(argc, argv, options, 3, parsed)) {
        Console.error("Invalid arguments.");
        return EXIT_FAILURE;
    }

    if (parsed[0] == NULL) {
        Console.error("Option -i is required");
        return EXIT_FAILURE;
    }

    if (parsed[1] == NULL || strcmp(parsed[1], "") == 0) {
        parsed[1] = strdup("Test");
    }

    if (parsed[2] == NULL || strcmp(parsed[2], "") == 0) {
        parsed[2] = strdup("test");
    }

    Timing.start("read file");
    string input = File.read(parsed[0]);
    if (input == NULL || strlen(input) == 0) {
        Console.error("Failed to read file: %s", parsed[0]);
        return EXIT_FAILURE;
    }
    Console.info("Read \"%s\" in %.2LF ms", parsed[0], Timing.check("read file"));

    Timing.start("parse file");
    Fields* fields = parse_fields(input);
    Console.info("Parse \"%s\" in %.2LF ms", parsed[0], Timing.check("parse file"));

    Timing.start("create header");
    string lower = to_lower(parsed[1]), upper = to_upper(parsed[1]);
    string struct_body = generate_struct(upper, fields);
    string init_func_head = format("%s* %s_init(void)", upper, lower);
    string free_func_head = format("void %s_free(%s* %s)", lower, upper, lower);
    string encode_func_head = format("int %s_encode(void* buffer, const %s* data)", lower, upper);
    string decode_func_head = format("int %s_decode(const void* buffer, %s* data)", lower, upper);
    Console.info("Create header in %.2LF ms", Timing.check("create header"));

    Timing.start("write header");
    string header = format(
        template_h,
        struct_body, init_func_head, free_func_head, encode_func_head, decode_func_head
    );
    File.write(format("%s.h", parsed[2]), header);
    Console.info("Write header in %.2LF ms", Timing.check("write header"));

    Timing.start("create body");
    string sizes = generate_sizes(fields);
    string init_func_body = format(template_init_body, init_func_head, upper, lower, upper, lower);
    string free_func_body = format(template_free_body, free_func_head, lower);
    string encode_func_body = format(template_encode_body, encode_func_head);
    string decode_func_body = format(template_decode_body, decode_func_head);
    Console.info("Create body in %.2LF ms", Timing.check("create body"));

    Timing.start("write body");
    string body = format(
        template_c,
        parsed[2], sizes, helper_functions,
        init_func_body, free_func_body, encode_func_body, decode_func_body
    );
    File.write(format("%s.c", parsed[2]), body);
    Console.info("Write body in %.2LF ms", Timing.check("write body"));

    return EXIT_SUCCESS;
}
