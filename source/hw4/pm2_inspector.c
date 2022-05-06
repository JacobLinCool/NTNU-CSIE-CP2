#include "lib/cimple-lib/all.h"
#include "lib/uds/vector.h"
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef u8 byte;
typedef char* string;

StructVector(Positions, i64, -1);

u8* map(string filepath, i64* size) {
    i32 fd = open(filepath, O_RDONLY);
    struct stat sb;
    fstat(fd, &sb);
    *size = sb.st_size;

    u8* file = mmap(NULL, *size, PROT_READ, MAP_PRIVATE, fd, 0);
    u8* dump = calloc(*size, sizeof(u8));
    memcpy(dump, file, *size);

    munmap(file, *size);
    close(fd);

    return dump;
}

Positions* diff(u8* prev, u8* curr, i64 size) {
    Positions* positions = create_Positions();

    for (i64 i = 0; i < size; i++) {
        if (curr[i] != prev[i]) {
            positions->push(positions, i);
        }
    }

    return positions;
}

Positions* calc_dod(Positions* prev, Positions* curr) {
    if (prev == NULL || prev->size != curr->size) {
        Positions* clone = create_Positions();

        for (i32 i = 0; i < curr->size; i++) {
            clone->push(clone, curr->data[i]);
        }

        return clone;
    }

    Positions* diff = create_Positions();

    for (i32 i = 0; i < curr->size; i++) {
        if (curr->data[i] != prev->data[i]) {
            diff->push(diff, i);
        }
    }

    return diff;
}

string notes[10000] = { NULL };

void set_notes() {
    notes[0x34] = strdup("月份");
    notes[0x36] = strdup("星期");
    notes[0x38] = strdup("日期");
    notes[0x3E] = strdup("體力");
    notes[0x40] = strdup("腕力");
    notes[0x42] = strdup("智力");
    notes[0x44] = strdup("氣質");
    notes[0x46] = strdup("魅力");
    notes[0x48] = strdup("道德");
    notes[0x4A] = strdup("信仰");
    notes[0x4C] = strdup("罪孽");
    notes[0x4E] = strdup("感受");
    notes[0x50] = strdup("疲勞");
    notes[0x52] = strdup("Unknown");
    notes[0x54] = strdup("戰士評價");
    notes[0x56] = strdup("魔法評價");
    notes[0x58] = strdup("社交評價");
    notes[0x5A] = strdup("家事評價");
    notes[0x5C] = strdup("戰鬥技術");
    notes[0x5E] = strdup("攻擊能力");
    notes[0x60] = strdup("防禦能力");
    notes[0x62] = strdup("魔法技術");
    notes[0x64] = strdup("魔法能力");
    notes[0x66] = strdup("抗魔能力");
    notes[0x68] = strdup("禮儀表現");
    notes[0x6A] = strdup("藝術修養");
    notes[0x6C] = strdup("談話技巧");
    notes[0x6E] = strdup("烹飪技術");
    notes[0x70] = strdup("掃除洗衣");
    notes[0x72] = strdup("待人接物");
    notes[0xB0] = strdup("金錢");
    notes[0xF0] = strdup("人際關係");
    notes[0x10C] = strdup("飲食計畫");
    notes[0x138] = strdup("父女對話次數");
    notes[0x13A] = strdup("父女聊天次數");
    notes[0x13E] = strdup("父女說教次數");
    notes[0x1C8] = strdup("冒險次數");
    notes[0x1CA] = strdup("東部冒險次數");
    notes[0x1CC] = strdup("西部冒險次數");
    notes[0x1CE] = strdup("南部冒險次數");
    notes[0x1D0] = strdup("北部冒險次數");
    notes[0x1E2] = strdup("冒險被幹掉次數 （不是喔～）");

    notes[0x262] = strdup("父女好感度");

    notes[0x26E] = strdup("是否佩帶武器 ?");

    notes[0x39C] = strdup("已完成天數");

    notes[0x4A8] = strdup("裝備 物品 ID");
    notes[0x4AA] = strdup("裝備 物品價格");
    notes[0x4AE] = strdup("裝備 物品名稱（中文）");
    notes[0x4C0] = strdup("裝備 物品敘述（英文）");
    notes[0x502] = strdup("裝備 物品敘述（中文）");

    notes[0x514] = strdup("武器 物品 ID");
    notes[0x516] = strdup("武器 物品價格");
    notes[0x51A] = strdup("武器 物品名稱（中文）");
    notes[0x52C] = strdup("武器 物品敘述（英文）");
    notes[0x56E] = strdup("武器 物品敘述（中文）");

    notes[0x580] = strdup("倉庫大小");
    for (i32 i = 0; i < 30; i++) {
        i32 offset = 0x584 + i * 110;
        notes[offset + 0] = calloc(64, sizeof(char));
        sprintf(notes[offset + 0], "Item No.%d 物品 ID", i + 1);
        notes[offset + 2] = calloc(64, sizeof(char));
        sprintf(notes[offset + 2], "Item No.%d 物品價格", i + 1);
        notes[offset + 8] = calloc(64, sizeof(char));
        sprintf(notes[offset + 8], "Item No.%d 物品名稱（中文）", i + 1);
        notes[offset + 92] = calloc(64, sizeof(char));
        sprintf(notes[offset + 92], "Item No.%d 物品敘述（中文）", i + 1);
    }
}

i32 main() {
    set_notes();
    string filepath = "./PM2/F101.GNX";

    Console.log("Start inspecting \"%s\", make any changes to see the difference.", filepath);

    i64 size;
    u8* dump = map(filepath, &size);
    u8* prev;

    Positions* prev_positions = NULL;
    Positions* positions = NULL;

    while (true) {
        sleep(1);
        u8* prev = dump;
        dump = map(filepath, &size);
        prev_positions = positions;
        positions = diff(prev, dump, size);

        Positions* dod = calc_dod(prev_positions, positions);

        if (dod->size) {
            Console.blue("---");
            Console.cyan("Offset     %4s %4s %6s %6s %12s %12s", "i8", "u8", "i16", "u16", "i32", "u32");
            i32 last = -100;
            for (i32 i = 0; i < dod->size; i++) {
                i64 offset = dod->data[i];

                if (offset - last > 1) {
                    Console.gray("-");
                }

                u8* ptr = dump + offset;
                u8* prev_ptr = prev + offset;
                Console.yellow(
                    "%6X [%c] %4" PRId8 " %4" PRIu8 " %6" PRId16 " %6" PRIu16 " %12" PRId32 " %12" PRIu32 " %s",
                    offset, (*ptr >= 32 && *ptr <= 127) ? *(char*)ptr : ' ', *ptr, *ptr, *(i16*)ptr, *(u16*)ptr, *(i32*)ptr, *(u32*)ptr,
                    notes[offset] != NULL ? notes[offset] : ""
                );
                if (prev != NULL) {
                    Console.blue(
                        "       [%c] %4" PRId8 " %4" PRIu8 " %6" PRId16 " %6" PRIu16 " %12" PRId32 " %12" PRIu32,
                        (*prev_ptr >= 32 && *prev_ptr <= 127) ? *(char*)prev_ptr : ' ', *prev_ptr, *prev_ptr, *(i16*)prev_ptr, *(u16*)prev_ptr, *(i32*)prev_ptr, *(u32*)prev_ptr
                    );
                }
                last = offset;
            }
        }

        dod->free(dod);

        if (prev != NULL) {
            free(prev);
        }

        if (prev_positions != NULL) {
            prev_positions->free(prev_positions);
        }
    }

    return EXIT_SUCCESS;
}
