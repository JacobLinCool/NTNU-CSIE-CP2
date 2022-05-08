#include "lib/cimple/all.h"

typedef char* string;

string Genres[] = { "Blues", "Classic rock", "Country", "Dance", "Disco", "Funk", "Grunge", "Hip-Hop", "Jazz", "Metal", "New Age", "Oldies", "Other", "Pop", "Rhythm and Blues", "Rap", "Reggae", "Rock", "Techno", "Industrial", "Alternative", "Ska", "Death metal", "Pranks", "Soundtrack", "Euro-Techno", "Ambient", "Trip-Hop", "Vocal", "Jazz & Funk", "Fusion", "Trance", "Classical", "Instrumental", "Acid", "House", "Game", "Sound clip", "Gospel", "Noise", "Alternative Rock", "Bass", "Soul", "Punk", "Space", "Meditative", "Instrumental Pop", "Instrumental Rock", "Ethnic", "Gothic", "Darkwave", "Techno-Industrial", "Electronic", "Pop-Folk", "Eurodance", "Dream", "Southern Rock", "Comedy", "Cult", "Gangsta", "Top 40", "Christian Rap", "Pop/Funk", "Jungle", "Native US", "Cabaret", "New Wave", "Psychedelic", "Rave", "Show tunes", "Trailer", "Lo-Fi", "Tribal", "Acid Punk", "Acid Jazz", "Polka", "Retro", "Musical", "Rock 'n' Roll", "Hard rock" };

typedef struct ID3 {
    char header[3];
    char title[30];
    char artist[30];
    char album[30];
    char year[4];
    char comment[28];
    char zero[1];
    char track[1];
    char genre[1];
} ID3;

typedef struct FileInfo {
    string path;
    size_t size;
    uint8_t* data;
} FileInfo;

FileInfo* read_file(string path) {
    FileInfo* info = calloc(1, sizeof(FileInfo));
    info->path = path;

    FILE* file = fopen(path, "rb");
    if (file == NULL) {
        free(info), fclose(file);
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    info->size = ftell(file);
    fseek(file, 0, SEEK_SET);
    info->data = malloc(sizeof(uint8_t) * info->size);
    fread(info->data, sizeof(uint8_t), info->size, file);
    fclose(file);

    return info;
}

void write_file(FileInfo* info) {
    FILE* file = fopen(info->path, "wb");
    if (file == NULL) {
        free(info->data), free(info), fclose(file);
        return;
    }
    fwrite(info->data, sizeof(uint8_t), info->size, file);
    fclose(file);
}

void free_file(FileInfo* info) {
    free(info->data), free(info);
}

void help() {
    Console.yellow("usage:");
    Console.yellow("\thw0502 -[tTaAycg] \"value\" file1 [file2...]");
    Console.yellow("\thw0502 -d file1 [file2...]");
    Console.yellow("\thw0502 -l file1 [file2...]");
    Console.yellow("\thw0502 -h");
    Console.cyan("options:");
    Console.cyan("\t-t\tModifies a Title tag");
    Console.cyan("\t-T\tModifies a Track tag");
    Console.cyan("\t-a\tModifies an Artist tag");
    Console.cyan("\t-A\tModifies an Album tag");
    Console.cyan("\t-y\tModifies a Year tag");
    Console.cyan("\t-c\tModifies a Comment tag");
    Console.cyan("\t-g\tModifies a Genre tag");
    Console.cyan("\t-l\tLists an ID3 tag");
    Console.cyan("\t-d\tDeletes an ID3 tag");
    Console.cyan("\t-h\tDisplays this help info");
    Console.magenta("supported genres:");
    printf("\t");
    for (int i = 0; i < sizeof(Genres) / sizeof(string); i++) {
        printf(__FMT_MAGENTA "%s" __FMT_NONE, Genres[i]);
        if (i != sizeof(Genres) / sizeof(string) - 1) {
            printf(", ");
        }
        if (i % 5 == 4) {
            printf("\n\t");
        }
    }
    printf("\n");
}

i32 main(i32 argc, string argv[]) {
    ParsedOptions* options = Options.parse(argc, argv);
    Options.key_only(options, "l");
    Options.key_only(options, "d");

    if (options->option_count == 0) {
        Console.error("You should read the help info first!");
        help();
        return EXIT_FAILURE;
    }

    if (Options.has(options, "h")) {
        help();
        return EXIT_SUCCESS;
    }

    if (options->wild_count == 0) {
        Console.error("No files specified!");
        help();
        return EXIT_FAILURE;
    }

    FileInfo** files = malloc(sizeof(FileInfo*) * options->wild_count);
    ID3** tags = malloc(sizeof(ID3*) * options->wild_count);
    for (i32 i = 0; i < options->wild_count; i++) {
        files[i] = read_file(options->wilds[i]);
        if (files[i] == NULL) {
            Console.error("Failed to read file: %s", options->wilds[i]);
            return EXIT_FAILURE;
        }
        tags[i] = (ID3*)(files[i]->data + (files[i]->size - sizeof(ID3)));
        if (memcmp(tags[i]->header, "TAG", 3) != 0) {
            files[i]->size += sizeof(ID3);
            files[i]->data = realloc(files[i]->data, files[i]->size);
            tags[i] = (ID3*)(files[i]->data + (files[i]->size - sizeof(ID3)));
            memset(tags[i], 0, sizeof(ID3));
            strncpy(tags[i]->header, "TAG", 3);
            Console.yellow("ID3 tag will be added to \"%s\"", options->wilds[i]);
        }
    }

    if (Options.get(options, "t") != NULL) {
        string title = Options.get(options, "t");
        if (title == NULL || strlen(title) > 30) {
            Console.error("Invalid title! It should be less than 30 characters!");
            return EXIT_FAILURE;
        }
        for (i32 i = 0; i < options->wild_count; i++) {
            memset(tags[i]->title, 0, 30);
            memcpy(tags[i]->title, title, strlen(title));
        }
    }
    if (Options.get(options, "T") != NULL) {
        string track = Options.get(options, "T");
        char* end;
        i32 track_num = strtol(track, &end, 10);
        if (track == NULL || track_num < 0 || track_num > 255 || *end != '\0') {
            Console.error("Invalid track! It should be between 0 and 255!");
            return EXIT_FAILURE;
        }
        for (i32 i = 0; i < options->wild_count; i++) {
            memset(tags[i]->zero, 0, 1);
            memset(tags[i]->track, 0, 1);
            memcpy(tags[i]->track, &track_num, 1);
        }
    }
    if (Options.get(options, "a") != NULL) {
        string artist = Options.get(options, "a");
        if (artist == NULL || strlen(artist) > 30) {
            Console.error("Invalid artist! It should be less than 30 characters!");
            return EXIT_FAILURE;
        }
        for (i32 i = 0; i < options->wild_count; i++) {
            memset(tags[i]->artist, 0, 30);
            memcpy(tags[i]->artist, artist, strlen(artist));
        }
    }
    if (Options.get(options, "A") != NULL) {
        string album = Options.get(options, "A");
        if (album == NULL || strlen(album) > 30) {
            Console.error("Invalid album! It should be less than 30 characters!");
            return EXIT_FAILURE;
        }
        for (i32 i = 0; i < options->wild_count; i++) {
            memset(tags[i]->album, 0, 30);
            memcpy(tags[i]->album, album, strlen(album));
        }
    }
    if (Options.get(options, "y") != NULL) {
        string year = Options.get(options, "y");
        char* end;
        i32 year_num = strtol(year, &end, 10);
        if (year == NULL || year_num < 0 || year_num > 9999 || *end != '\0') {
            Console.error("Invalid year! It should be between 0 and 9999!");
            return EXIT_FAILURE;
        }
        for (i32 i = 0; i < options->wild_count; i++) {
            memset(tags[i]->year, 0, 4);
            memcpy(tags[i]->year, year, strlen(year));
        }
    }
    if (Options.get(options, "c") != NULL) {
        string comment = Options.get(options, "c");
        if (comment == NULL || strlen(comment) > 28) {
            Console.error("Invalid comment! It should be less than 28 characters!");
            return EXIT_FAILURE;
        }
        for (i32 i = 0; i < options->wild_count; i++) {
            memset(tags[i]->comment, 0, 28);
            memcpy(tags[i]->comment, comment, strlen(comment));
        }
    }
    if (Options.get(options, "g") != NULL) {
        string genre = Options.get(options, "g");
        i32 genre_num = -1;
        if (genre != NULL) {
            for (i32 i = 0; i < sizeof(Genres) / sizeof(string); i++) {
                if (strcmp(genre, Genres[i]) == 0) {
                    genre_num = i;
                    break;
                }
            }
        }
        if (genre == NULL || genre_num == -1) {
            Console.error("Invalid genre! Use \"-h\" to see the list of genres!");
            return EXIT_FAILURE;
        }
        for (i32 i = 0; i < options->wild_count; i++) {
            memcpy(tags[i]->genre, &genre_num, 1);
        }
    }
    if (Options.has(options, "d")) {
        for (i32 i = 0; i < options->wild_count; i++) {
            memset(tags[i], 0, sizeof(ID3));
            strncpy(tags[i]->header, "TAG", 3);
        }
    }
    if (Options.has(options, "l")) {
        for (i32 i = 0; i < options->wild_count; i++) {
            Console.magenta("Listing tags for file: %s", options->wilds[i]);
            Console.cyan("\tTitle:  \t%s", $(strndup(tags[i]->title, sizeof(tags[i]->title))));
            Console.cyan("\tArtist: \t%s", $(strndup(tags[i]->artist, sizeof(tags[i]->artist))));
            Console.cyan("\tAlbum:  \t%s", $(strndup(tags[i]->album, sizeof(tags[i]->album))));
            Console.cyan("\tYear:   \t%s", $(strndup(tags[i]->year, sizeof(tags[i]->year))));
            Console.cyan("\tGenre:  \t%s (%" PRIu8 ")", *(u8*)tags[i]->genre >= 80 ? "" : Genres[*(u8*)tags[i]->genre], *(u8*)tags[i]->genre);
            Console.cyan("\tComment:\t%s", $(strndup(tags[i]->comment, sizeof(tags[i]->comment))));
            Console.cyan("\tTrack:  \t%" PRIu8, *(u8*)tags[i]->track);
        }
        $free();
    }

    for (i32 i = 0; i < options->wild_count; i++) {
        write_file(files[i]);
        free_file(files[i]);
    }

    return EXIT_SUCCESS;
}
