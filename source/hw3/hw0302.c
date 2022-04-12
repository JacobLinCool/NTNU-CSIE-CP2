// #define DEBUG
#include "helper.h"
#include "lib/uds/vector.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <ctype.h>

typedef struct Subtitle {
    u64 id;
    u64 start_time;
    u64 end_time;
    string content;
} Subtitle;

StructVector(Subtitles, Subtitle*, NULL);

static inline u64 parse_time(string str) {
    u64 h = 0, m = 0, s = 0, ms = 0;
    sscanf(str, "%" SCNu64 ":%" SCNu64 ":%" SCNu64 ",%" SCNu64, &h, &m, &s, &ms);
    return h * 3600000 + m * 60000 + s * 1000 + ms;
}

struct timespec* get_duration(u64 time, f64 speed) {
    struct timespec* duration = calloc(1, sizeof(struct timespec));
    time = (u64)((f128)time / (f128)speed);
    duration->tv_sec = time / 1000;
    duration->tv_nsec = (time % 1000) * 1000000;
    return duration;
}

Subtitle* parse(string* raw) {
    if (*raw == NULL) {
        return NULL;
    }

    Subtitle* subtitle = calloc(1, sizeof(Subtitle));
    subtitle->content = calloc(1024 * 32, sizeof(char));

    string start_time = calloc(64, sizeof(char));
    string end_time = calloc(64, sizeof(char));
    i32 offset = 0;

    i32 state = sscanf(
        *raw,
        "%" SCNu64 " %s --> %s%n",
        &subtitle->id, start_time, end_time,
        &offset
    );
    if (state != 3) {
        return NULL;
    }
    DBG_PRINT("[%" PRId32 "] %" PRIu64 " [%s] [%s]\n", offset, subtitle->id, start_time, end_time);
    *raw += offset + 1;

    string line = calloc(1024 * 32, sizeof(char));
    while (sscanf(*raw, "%[^\n]%*c%n", line, &offset) == 1) {
        DBG_PRINT("[%s]\n", line);
        *raw += offset;
        if (subtitle->content[0] != '\0') {
            strcat(subtitle->content, "\n");
        }
        strcat(subtitle->content, line);
        memset(line, 0, strlen(line));
    }

    subtitle->start_time = parse_time(start_time);
    subtitle->end_time = parse_time(end_time);

    free(start_time), free(end_time);
    subtitle->content = realloc(subtitle->content, strlen(subtitle->content) + 1);
    return subtitle;
}

Subtitles* open_srt(string filepath) {
    string file = File.read(filepath);
    THROW_IF(file == NULL, "No File");
    string head = file;

    Subtitles* subtitles = create_Subtitles();
    while (1) {
        Subtitle* subtitle = parse(&head);
        if (subtitle == NULL) {
            break;
        }
        subtitles->push(subtitles, subtitle);
    }

    return subtitles;
}

void play_subtitles(Subtitles* subtitles, f64 speed) {
    u64 current_time = 0;
    system("clear");

    for (size_t i = 0; i < subtitles->size; i++) {
        Subtitle* subtitle = subtitles->get(subtitles, i);
        DBG_PRINT("start = %" PRIu64 ", end = %" PRIu64 ", now = %" PRIu64 "\n", subtitle->start_time, subtitle->end_time, current_time);

        if (subtitle->start_time > current_time) {
            nanosleep(get_duration(subtitle->start_time - current_time, speed), NULL);
        }

        current_time = subtitle->start_time;
        printf("%s\n", subtitle->content);
        nanosleep(get_duration(subtitle->end_time - current_time, speed), NULL);
        system("clear");
        current_time = subtitle->end_time;
    }

    printf("Done. All %zu subtitles have been played.\n", subtitles->size);
}

i32 main() {
    string filepath = calloc(1024, sizeof(char));
    f64 speed = 1.0;

    printf("Open a srt file: ");
    scanf("%[^\n]%*c", filepath);
    printf("Speed (0.5-2): ");
    THROW_IF(scanf("%lf", &speed) != 1, "Invalid Speed");

    THROW_IF(speed < 0.5 || speed > 2, "Speed must be in range of 0.5 ~ 2. Received %lg", speed);

    Subtitles* subtitles = open_srt(filepath);
    play_subtitles(subtitles, speed);
    subtitles->free(subtitles);

    return EXIT_SUCCESS;
}
