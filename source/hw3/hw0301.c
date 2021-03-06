// #define DEBUG
#include "helper.h"
#include "lib/uds/vector.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <ctype.h>

typedef struct Sentence {
    u64 chapter;
    u64 verse;
    string text;
    string translation_id;
    string book_id;
    string book_name;
} Sentence;

StructVector(Sentences, Sentence*, NULL);

Sentence* parse(string* raw) {
    if (*raw == NULL) {
        return NULL;
    }
    char* start = strchr(*raw, '{');
    char* end = strchr(*raw, '}');

    if (start == NULL || end == NULL) {
        return NULL;
    }

    Sentence* sentence = calloc(1, sizeof(Sentence));
    sentence->text = calloc(end - start, sizeof(char));
    sentence->translation_id = calloc(end - start, sizeof(char));
    sentence->book_id = calloc(end - start, sizeof(char));
    sentence->book_name = calloc(end - start, sizeof(char));

    sscanf(
        start,
        "{\"chapter\":%" PRIu64 ",\"verse\":%" PRIu64 ",\"text\":\"%[^\"]\",\"translation_id\":\"%[^\"]\",\"book_id\":\"%[^\"]\",\"book_name\":\"%[^\"]\"}",
        &sentence->chapter,
        &sentence->verse,
        sentence->text,
        sentence->translation_id,
        sentence->book_id,
        sentence->book_name
    );

    sentence->text = realloc(sentence->text, sizeof(char) * (strlen(sentence->text) + 1));
    sentence->translation_id = realloc(sentence->translation_id, sizeof(char) * (strlen(sentence->translation_id) + 1));
    sentence->book_id = realloc(sentence->book_id, sizeof(char) * (strlen(sentence->book_id) + 1));
    sentence->book_name = realloc(sentence->book_name, sizeof(char) * (strlen(sentence->book_name) + 1));

    *raw = end + 1;
    return sentence;
}

i32 main() {
    system("clear");
    printf("Parsing bible.txt ... It may take a few seconds.\n");

    string file = File.read("bible.txt");
    THROW_IF(file == NULL, "No File");
    string head = file;
    u64 size = strlen(file);

    timing_start("Parse bible.txt");
    Sentences* sentences = create_Sentences();
    while (head < file + size) {
        if (sentences->size % 2000 == 0) {
            system("clear");
            printf("Parsing bible.txt ... It may take a few seconds. Parsed %zu sentences.\n", sentences->size);
        }
        Sentence* sentence = parse(&head);
        if (sentence != NULL) {
            sentences->push(sentences, sentence);
        }
        else {
            break;
        }
    }
    f128 elapsed = timing_check("Parse bible.txt");
    system("clear");
    DBG_PRINT("Parsed %zu sentences in %Lf ms.\n", sentences->size, elapsed);

    string target = calloc(size, sizeof(char));
    printf("Please enter the search target: ");
    scanf("%[^\n]%*c", target);
    target = String.lower(target);

    Sentences* found = create_Sentences();
    for (u64 i = 0; i < sentences->size; i++) {
        Sentence* sentence = sentences->get(sentences, i);
        string lower = String.lower(sentence->text);
        if (strstr(lower, target) != NULL) {
            found->push(found, sentence);
        }
        free(lower);
    }

    printf("Found %zu time(s)\n", found->size);
    for (u64 i = 0; i < found->size; i++) {
        Sentence* sentence = found->get(found, i);
        printf("%" PRIu64 ". %s %" PRIu64 ":%" PRIu64 " %s\n", i + 1, sentence->book_id, sentence->chapter, sentence->verse, sentence->text);
    }

    return EXIT_SUCCESS;
}
