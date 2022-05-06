#include "helper.h"
#include "lib/uds/vector.h"
#include "lib/string.h"
#define MAX_STRUCT_SIZE (2 * 1024 * 1024)

// #region buffer_h
string buffer_h = \
"/**\n"
" * @file buffer.h\n"
" * @author JacobLinCool <hi@jacoblin.cool> (github.com/JacobLinCool)\n"
" * @brief The buffer header file of Cimple Lib.\n"
" * @version 3.0.0\n"
" * @date 2022-05-03\n"
" *\n"
" * @copyright Copyright (c) 2022 JacobLinCool (MIT License)\n"
" * @see https://github.com/JacobLinCool/Cimple-Lib\n"
" */\n"
"#ifndef __CIMPLE_UTILS_BUFFER_H\n"
"#define __CIMPLE_UTILS_BUFFER_H\n"
"\n"
"#include <stdbool.h>\n"
"#include <stdint.h>\n"
"#include <stdio.h>\n"
"#include <stdlib.h>\n"
"\n"
"bool __buffer_get_bit(const void* buffer, size_t bit) {\n"
"    if (buffer == NULL) {\n"
"        return false;\n"
"    }\n"
"\n"
"    const uint8_t* buffer_ptr = buffer;\n"
"    size_t         byte = bit / 8;\n"
"    size_t         offset = 7 - bit % 8;\n"
"    return (buffer_ptr[byte] >> offset) & 1;\n"
"}\n"
"\n"
"void __buffer_set_bit(void* buffer, size_t bit, bool value) {\n"
"    if (buffer == NULL) {\n"
"        return;\n"
"    }\n"
"\n"
"    uint8_t* buffer_ptr = buffer;\n"
"    size_t   byte = bit / 8;\n"
"    size_t   offset = 7 - bit % 8;\n"
"    if (value) {\n"
"        buffer_ptr[byte] |= 1 << offset;\n"
"    } else {\n"
"        buffer_ptr[byte] &= ~(1 << offset);\n"
"    }\n"
"}\n"
"\n"
"char* __buffer_stringify(const void* buffer, size_t size, size_t col_size, size_t row_size) {\n"
"    if (buffer == NULL) {\n"
"        return NULL;\n"
"    }\n"
"\n"
"    size_t cols = (size + col_size - 1) / col_size;\n"
"    size_t rows = (cols + row_size - 1) / row_size;\n"
"    char*  str = malloc(size + cols + rows + 1);\n"
"\n"
"    size_t idx = 0;\n"
"    for (size_t i = 0; i < size; i++) {\n"
"        str[idx++] = __buffer_get_bit(buffer, i) ? '1' : '0';\n"
"        if (i % col_size == col_size - 1) {\n"
"            str[idx++] = ' ';\n"
"        }\n"
"        if (i % (col_size * row_size) == (col_size * row_size) - 1 && i != size - 1) {\n"
"            str[idx++] = '\\n';\n"
"        }\n"
"    }\n"
"    str[idx] = '\\0';\n"
"\n"
"    return str;\n"
"}\n"
"\n"
"void __buffer_parse(void* buffer, const char* str, size_t size) {\n"
"    if (buffer == NULL || str == NULL) {\n"
"        return;\n"
"    }\n"
"\n"
"    size_t idx = 0;\n"
"    for (size_t i = 0; i < size; i++) {\n"
"        if (str[idx] != '0' && str[idx] != '1') {\n"
"            idx++, i--;\n"
"            continue;\n"
"        }\n"
"        __buffer_set_bit(buffer, i, str[idx++] == '1');\n"
"    }\n"
"}\n"
"\n"
"bool __buffer_endian() { return (*(uint16_t*)\"\\x01\\x02\") == 0x0201; }\n"
"\n"
"void* __buffer_reverse(const void* buffer, size_t size) {\n"
"    if (buffer == NULL) {\n"
"        return NULL;\n"
"    }\n"
"\n"
"    uint8_t* reversed = malloc(size);\n"
"    for (size_t i = 0; i < size; i++) {\n"
"        reversed[i] = ((uint8_t*)buffer)[size - i - 1];\n"
"    }\n"
"    return reversed;\n"
"}\n"
"\n"
"void* __buffer_to_big(const void* buffer, size_t size) {\n"
"    if (buffer == NULL) {\n"
"        return NULL;\n"
"    }\n"
"\n"
"    size_t   blocks = (size + 7) / 8;\n"
"    size_t   padding = blocks * 8 - size;\n"
"    uint8_t* big = calloc(blocks, 1);\n"
"    for (size_t b = 0; b < blocks; b++) {\n"
"        size_t len = b == blocks - 1 ? 8 - padding : 8;\n"
"        for (size_t i = 0; i < len; i++) {\n"
"            size_t pos = size - 1 - b * 8 - (7 - i) + (b == blocks - 1 ? padding : 0);\n"
"            if (pos < size && pos >= 0) {\n"
"                __buffer_set_bit(big, pos, __buffer_get_bit(buffer, b * 8 + i));\n"
"            }\n"
"        }\n"
"    }\n"
"\n"
"    return big;\n"
"}\n"
"\n"
"void* __buffer_to_little(const void* buffer, size_t size) {\n"
"    if (buffer == NULL) {\n"
"        return NULL;\n"
"    }\n"
"\n"
"    size_t   blocks = (size + 7) / 8;\n"
"    size_t   padding = blocks * 8 - size;\n"
"    uint8_t* little = calloc(blocks, 1);\n"
"    for (size_t b = 0; b < blocks; b++) {\n"
"        size_t len = b == blocks - 1 ? 8 - padding : 8;\n"
"        for (size_t i = 0; i < len; i++) {\n"
"            size_t pos = b * 8 + i;\n"
"            if (pos < size && pos >= 0) {\n"
"                __buffer_set_bit(little, pos,\n"
"                                 __buffer_get_bit(buffer, size - 1 - b * 8 - (7 - i) +\n"
"                                                              (b == blocks - 1 ? padding : 0)));\n"
"            }\n"
"        }\n"
"    }\n"
"\n"
"    return little;\n"
"}\n"
"\n"
"/**\n"
" * @brief Buffer utility functions.\n"
" */\n"
"struct {\n"
"    bool (*get_bit)(const void* buffer, size_t bit);\n"
"    void (*set_bit)(void* buffer, size_t bit, bool value);\n"
"    char* (*stringify)(const void* buffer, size_t size, size_t col_size, size_t row_size);\n"
"    void (*parse)(void* buffer, const char* str, size_t size);\n"
"    bool (*endian)();\n"
"    void* (*reverse)(const void* buffer, size_t size);\n"
"    void* (*to_big)(const void* buffer, size_t size);\n"
"    void* (*to_little)(const void* buffer, size_t size);\n"
"} Buffer = {\n"
"    .get_bit = __buffer_get_bit,\n"
"    .set_bit = __buffer_set_bit,\n"
"    .stringify = __buffer_stringify,\n"
"    .parse = __buffer_parse,\n"
"    .endian = __buffer_endian,\n"
"    .reverse = __buffer_reverse,\n"
"    .to_big = __buffer_to_big,\n"
"    .to_little = __buffer_to_little,\n"
"};\n"
"\n"
"#endif  // __CIMPLE_UTILS_BUFFER_H\n";
// #endregion

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
"    size_t from_bit = 0;\n" \
"    size_t data_offset = 0;\n" \
"    for (size_t i = 0; i < field_count; i++) {\n" \
"        size_t size = field_sizes[i];\n" \
"        put_buffer(buffer, from_bit, size, (uint8_t*)data + data_offset);\n" \
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
"    size_t from_bit = 0;\n" \
"    size_t data_offset = 0;\n" \
"    for (size_t i = 0; i < field_count; i++) {\n" \
"        size_t size = field_sizes[i];\n" \
"        uint8_t* bin = take_buffer(buffer, from_bit, size);\n" \
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
"#include <stdint.h>\n#include <stdbool.h>\n#include <stdlib.h>\n#include <string.h>\n\n" \
"%s\n" \
"%s\n" \
"%s\n" \
"%s\n" \
"%s\n" \
"%s\n" \
"%s\n";

string helper_functions = \
"static uint8_t* take_buffer(const uint8_t* buffer, size_t from_bit, size_t size) {\n" \
"    size_t pad = (size < 8) ? 8 - size : 0;\n" \
"    void* data = calloc((size + 7) / 8, 1);\n" \
"    for (size_t i = 0; i < size; i++) {\n" \
"        Buffer.set_bit(data, pad + i, Buffer.get_bit(buffer, from_bit + i));\n" \
"    }\n" \
"    if (size > 8) {\n" \
"        void* little = Buffer.to_little(data, size);\n" \
"        free(data);\n" \
"        return little;\n" \
"    }\n" \
"    else {\n" \
"        return data;\n" \
"    }\n" \
"}\n" \
"\n" \
"static void put_buffer(uint8_t* buffer, size_t from_bit, size_t size, const uint8_t* data) {\n" \
"    if (size <= 8) {\n" \
"        for (size_t i = 0; i < size; i++) {\n" \
"            size_t pad = (size < 8) ? 8 - size : 0;\n" \
"            Buffer.set_bit(buffer, from_bit + i, Buffer.get_bit(data, pad + i));\n" \
"        }\n" \
"    }\n" \
"    else {\n" \
"        void* big = Buffer.to_big(data, size);\n" \
"        for (size_t i = 0; i < size; i++) {\n" \
"            size_t pad = (size < 8) ? 8 - size : 0;\n" \
"            Buffer.set_bit(buffer, from_bit + i, Buffer.get_bit(big, pad + i));\n" \
"        }\n" \
"        free(big);\n" \
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
        parsed[2], buffer_h, sizes, helper_functions,
        init_func_body, free_func_body, encode_func_body, decode_func_body
    );
    File.write(format("%s.c", parsed[2]), body);
    Console.info("Write body in %.2LF ms", Timing.check("write body"));

    return EXIT_SUCCESS;
}
