#include "helper.h"
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#define SAVE_FILE_SIZE 8000

enum Type {
    INT,
    STR_EN,
    STR_CH
};

enum Type types[SAVE_FILE_SIZE] = { INT };
string notes[SAVE_FILE_SIZE] = { NULL };
i32 low_boundaries[SAVE_FILE_SIZE] = { 0 };
i32 high_boundaries[SAVE_FILE_SIZE] = { 0 };
u8 field_sizes[SAVE_FILE_SIZE] = { 0 };

void setup() {
    notes[0x32] = strdup("年份");
    low_boundaries[0x32] = 1;
    high_boundaries[0x32] = 9999;
    field_sizes[0x32] = 2;
    notes[0x34] = strdup("月份");
    low_boundaries[0x34] = 1;
    high_boundaries[0x34] = 12;
    field_sizes[0x34] = 2;
    notes[0x36] = strdup("星期");
    low_boundaries[0x36] = 1;
    high_boundaries[0x36] = 7;
    field_sizes[0x36] = 2;
    notes[0x38] = strdup("日期");
    low_boundaries[0x38] = 1;
    high_boundaries[0x38] = 31;
    field_sizes[0x38] = 2;
    notes[0x3E] = strdup("體力");
    low_boundaries[0x3E] = 0;
    high_boundaries[0x3E] = 999;
    field_sizes[0x3E] = 2;
    notes[0x40] = strdup("腕力");
    low_boundaries[0x40] = 0;
    high_boundaries[0x40] = 999;
    field_sizes[0x40] = 2;
    notes[0x42] = strdup("智力");
    low_boundaries[0x42] = 0;
    high_boundaries[0x42] = 999;
    field_sizes[0x42] = 2;
    notes[0x44] = strdup("氣質");
    low_boundaries[0x44] = 0;
    high_boundaries[0x44] = 999;
    field_sizes[0x44] = 2;
    notes[0x46] = strdup("魅力");
    low_boundaries[0x46] = 0;
    high_boundaries[0x46] = 999;
    field_sizes[0x46] = 2;
    notes[0x48] = strdup("道德");
    low_boundaries[0x48] = 0;
    high_boundaries[0x48] = 999;
    field_sizes[0x48] = 2;
    notes[0x4A] = strdup("信仰");
    low_boundaries[0x4A] = 0;
    high_boundaries[0x4A] = 999;
    field_sizes[0x4A] = 2;
    notes[0x4C] = strdup("罪孽");
    low_boundaries[0x4C] = 0;
    high_boundaries[0x4C] = 999;
    field_sizes[0x4C] = 2;
    notes[0x4E] = strdup("感受");
    low_boundaries[0x4E] = 0;
    high_boundaries[0x4E] = 999;
    field_sizes[0x4E] = 2;
    notes[0x50] = strdup("疲勞");
    low_boundaries[0x50] = 0;
    high_boundaries[0x50] = 999;
    field_sizes[0x50] = 2;
    notes[0x52] = strdup("Unknown");
    low_boundaries[0x52] = 0;
    high_boundaries[0x52] = 0;
    notes[0x54] = strdup("戰士評價");
    low_boundaries[0x54] = 0;
    high_boundaries[0x54] = 999;
    field_sizes[0x54] = 2;
    notes[0x56] = strdup("魔法評價");
    low_boundaries[0x56] = 0;
    high_boundaries[0x56] = 999;
    field_sizes[0x56] = 2;
    notes[0x58] = strdup("社交評價");
    low_boundaries[0x58] = 0;
    high_boundaries[0x58] = 999;
    field_sizes[0x58] = 2;
    notes[0x5A] = strdup("家事評價");
    low_boundaries[0x5A] = 0;
    high_boundaries[0x5A] = 999;
    field_sizes[0x5A] = 2;
    notes[0x5C] = strdup("戰鬥技術");
    low_boundaries[0x5C] = 0;
    high_boundaries[0x5C] = 999;
    field_sizes[0x5C] = 2;
    notes[0x5E] = strdup("攻擊能力");
    low_boundaries[0x5E] = 0;
    high_boundaries[0x5E] = 999;
    field_sizes[0x5E] = 2;
    notes[0x60] = strdup("防禦能力");
    low_boundaries[0x60] = 0;
    high_boundaries[0x60] = 999;
    field_sizes[0x60] = 2;
    notes[0x62] = strdup("魔法技術");
    low_boundaries[0x62] = 0;
    high_boundaries[0x62] = 999;
    field_sizes[0x62] = 2;
    notes[0x64] = strdup("魔法能力");
    low_boundaries[0x64] = 0;
    high_boundaries[0x64] = 999;
    field_sizes[0x64] = 2;
    notes[0x66] = strdup("抗魔能力");
    low_boundaries[0x66] = 0;
    high_boundaries[0x66] = 999;
    field_sizes[0x66] = 2;
    notes[0x68] = strdup("禮儀表現");
    low_boundaries[0x68] = 0;
    high_boundaries[0x68] = 999;
    field_sizes[0x68] = 2;
    notes[0x6A] = strdup("藝術修養");
    low_boundaries[0x6A] = 0;
    high_boundaries[0x6A] = 999;
    field_sizes[0x6A] = 2;
    notes[0x6C] = strdup("談話技巧");
    low_boundaries[0x6C] = 0;
    high_boundaries[0x6C] = 999;
    field_sizes[0x6C] = 2;
    notes[0x6E] = strdup("烹飪技術");
    low_boundaries[0x6E] = 0;
    high_boundaries[0x6E] = 999;
    field_sizes[0x6E] = 2;
    notes[0x70] = strdup("掃除洗衣");
    low_boundaries[0x70] = 0;
    high_boundaries[0x70] = 999;
    field_sizes[0x70] = 2;
    notes[0x72] = strdup("待人接物");
    low_boundaries[0x72] = 0;
    high_boundaries[0x72] = 999;
    field_sizes[0x72] = 2;
    notes[0xB0] = strdup("金錢");
    low_boundaries[0xB0] = 0;
    high_boundaries[0xB0] = 9999999;
    field_sizes[0xB0] = 4;
    notes[0xD2] = strdup("身高 (單位: 0.01 公分)");
    low_boundaries[0xD2] = 0;
    high_boundaries[0xD2] = 65535;
    field_sizes[0xD2] = 2;
    notes[0xD4] = strdup("體重 (單位: 0.01 公斤)");
    low_boundaries[0xD4] = 0;
    high_boundaries[0xD4] = 65535;
    field_sizes[0xD4] = 2;
    notes[0xD6] = strdup("胸圍 (單位: 0.01 公分)");
    low_boundaries[0xD6] = 0;
    high_boundaries[0xD6] = 9999;
    field_sizes[0xD6] = 2;
    notes[0xD8] = strdup("腰圍 (單位: 0.01 公分)");
    low_boundaries[0xD8] = 0;
    high_boundaries[0xD8] = 9999;
    field_sizes[0xD8] = 2;
    notes[0xDA] = strdup("臀圍 (單位: 0.01 公分)");
    low_boundaries[0xDA] = 0;
    high_boundaries[0xDA] = 9999;
    field_sizes[0xDA] = 2;
    notes[0xEC] = strdup("變壞指數");
    low_boundaries[0xEC] = 0;
    high_boundaries[0xEC] = 100;
    field_sizes[0xEC] = 2;
    notes[0xF0] = strdup("人際關係");
    low_boundaries[0xF0] = 0;
    high_boundaries[0xF0] = 999;
    field_sizes[0xF0] = 2;
    notes[0x10C] = strdup("飲食計畫");
    low_boundaries[0x10C] = 0;
    high_boundaries[0x10C] = 3;
    field_sizes[0x10C] = 1;
    notes[0x118] = strdup("與人交談次數");
    low_boundaries[0x118] = 0;
    high_boundaries[0x118] = 12 * 8;
    field_sizes[0x118] = 1;
    notes[0x138] = strdup("父女對話次數");
    low_boundaries[0x138] = 0;
    high_boundaries[0x138] = 12 * 8;
    field_sizes[0x138] = 1;
    notes[0x13A] = strdup("父女聊天次數");
    low_boundaries[0x13A] = 0;
    high_boundaries[0x13A] = 12 * 8;
    field_sizes[0x13A] = 1;
    notes[0x13E] = strdup("父女說教次數");
    low_boundaries[0x13E] = 0;
    high_boundaries[0x13E] = 12 * 8;
    field_sizes[0x13E] = 1;
    notes[0x1C8] = strdup("冒險次數");
    low_boundaries[0x1C8] = 0;
    high_boundaries[0x1C8] = 12 * 8 * 3;
    field_sizes[0x1C8] = 2;
    notes[0x1CA] = strdup("東部冒險次數");
    low_boundaries[0x1CA] = 0;
    high_boundaries[0x1CA] = 12 * 8 * 3;
    field_sizes[0x1CA] = 2;
    notes[0x1CC] = strdup("西部冒險次數");
    low_boundaries[0x1CC] = 0;
    high_boundaries[0x1CC] = 12 * 8 * 3;
    field_sizes[0x1CC] = 2;
    notes[0x1CE] = strdup("南部冒險次數");
    low_boundaries[0x1CE] = 0;
    high_boundaries[0x1CE] = 12 * 8 * 3;
    field_sizes[0x1CE] = 2;
    notes[0x1D0] = strdup("北部冒險次數");
    low_boundaries[0x1D0] = 0;
    high_boundaries[0x1D0] = 12 * 8 * 3;
    field_sizes[0x1D0] = 2;
    // notes[0x1E2] = strdup("冒險被幹掉次數 （不是喔～）");

    notes[0x262] = strdup("父女好感度");
    low_boundaries[0x262] = 0;
    high_boundaries[0x262] = 100;
    field_sizes[0x262] = 1;

    notes[0x26E] = strdup("是否佩帶武器 ?");
    low_boundaries[0x26E] = 0;
    high_boundaries[0x26E] = 1;
    field_sizes[0x26E] = 1;

    notes[0x39C] = strdup("已完成天數");
    low_boundaries[0x39C] = 0;
    high_boundaries[0x39C] = 12 * 8 * 31;
    field_sizes[0x39C] = 2;

    notes[0x4A8] = strdup("裝備 物品 ID");
    high_boundaries[0x4A8] = 512;
    field_sizes[0x4A8] = 2;
    notes[0x4AA] = strdup("裝備 物品價格");
    high_boundaries[0x4AA] = 65535;
    field_sizes[0x4AA] = 2;
    notes[0x4AE] = strdup("裝備 物品名稱（中文）");
    high_boundaries[0x4AE] = 16;
    field_sizes[0x4AE] = 16;
    types[0x4AE] = STR_CH;
    notes[0x4C0] = strdup("裝備 物品敘述（英文）");
    high_boundaries[0x4C0] = 64;
    field_sizes[0x4C0] = 64;
    types[0x4C0] = STR_EN;
    notes[0x502] = strdup("裝備 物品敘述（中文）");
    high_boundaries[0x502] = 16;
    field_sizes[0x502] = 16;
    types[0x502] = STR_CH;

    notes[0x514] = strdup("武器 物品 ID");
    high_boundaries[0x514] = 512;
    field_sizes[0x514] = 2;
    notes[0x516] = strdup("武器 物品價格");
    high_boundaries[0x516] = 65535;
    field_sizes[0x516] = 2;
    notes[0x51A] = strdup("武器 物品名稱（中文）");
    high_boundaries[0x51A] = 16;
    field_sizes[0x51A] = 16;
    types[0x51A] = STR_CH;
    notes[0x52C] = strdup("武器 物品敘述（英文）");
    high_boundaries[0x52C] = 64;
    field_sizes[0x52C] = 64;
    types[0x52C] = STR_EN;
    notes[0x56E] = strdup("武器 物品敘述（中文）");
    high_boundaries[0x56E] = 16;
    field_sizes[0x56E] = 16;
    types[0x56E] = STR_CH;

    notes[0x580] = strdup("倉庫大小");
    low_boundaries[0x580] = 0;
    high_boundaries[0x580] = 50;
    field_sizes[0x580] = 1;
    for (i32 i = 0; i < 50; i++) {
        i32 offset = 0x584 + i * 110;
        notes[offset + 0] = calloc(64, sizeof(char));
        sprintf(notes[offset + 0], "Item No.%d 物品 ID", i + 1);
        high_boundaries[offset + 0] = 512;
        field_sizes[offset + 0] = 2;
        notes[offset + 2] = calloc(64, sizeof(char));
        sprintf(notes[offset + 2], "Item No.%d 物品價格", i + 1);
        high_boundaries[offset + 2] = 65535;
        field_sizes[offset + 2] = 2;
        notes[offset + 8] = calloc(64, sizeof(char));
        sprintf(notes[offset + 8], "Item No.%d 物品名稱（中文）", i + 1);
        high_boundaries[offset + 8] = 16;
        field_sizes[offset + 8] = 16;
        types[offset + 8] = STR_CH;
        notes[offset + 26] = calloc(64, sizeof(char));
        sprintf(notes[offset + 26], "Item No.%d 物品敘述（英文）", i + 1);
        high_boundaries[offset + 26] = 64;
        field_sizes[offset + 26] = 64;
        types[offset + 26] = STR_EN;
        notes[offset + 92] = calloc(64, sizeof(char));
        sprintf(notes[offset + 92], "Item No.%d 物品敘述（中文）", i + 1);
        high_boundaries[offset + 92] = 16;
        field_sizes[offset + 92] = 16;
        types[offset + 92] = STR_CH;
    }
}

i32 main(i32 argc, string argv[]) {
    setup();

    string options[] = { "p|bypass?" };
    string parsed[1] = { NULL };
    if (parse_options(argc, argv, options, 1, parsed)) {
        Console.error("Invalid arguments.");
        return EXIT_FAILURE;
    }

    bool bypass = false;
    if (parsed[0] != NULL) {
        bypass = true;
    }

    Console.cyan("Which file do you want to edit?");
    string filename = calloc(1024, sizeof(char));
    scanf("%s", filename);

    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        Console.error("Could not open file.");
        return EXIT_FAILURE;
    }
    fclose(file);

    Console.info(__FMT_YELLOW "Offset : Acceptable Range : Description" __FMT_NONE);
    for (size_t i = 0; i < SAVE_FILE_SIZE; i++) {
        if (notes[i] != NULL) {
            Console.info("%6zu : %6d ~ %7d : %s", i, low_boundaries[i], high_boundaries[i], notes[i]);
        }
    }
    Console.info(__FMT_YELLOW "Offset : Acceptable Range : Description" __FMT_NONE);

    while (true) {
        Console.cyan("Please select an offset you want to edit: ");
        i32 offset;
        scanf("%" SCNd32 "%*c", &offset);
        if (!bypass && notes[offset] == NULL) {
            Console.error("Invalid offset. Assume you want to exit.");
            break;
        }
        Console.cyan("You selected %d (%s)", offset, notes[offset]);
        Console.cyan("Please enter the new value: (%d ~ %d)", low_boundaries[offset], high_boundaries[offset]);
        string value = calloc(1024, sizeof(char));
        scanf("%[^\n]%*c", value);
        if (strlen(value) == 0) {
            Console.error("Invalid value. Cannot be empty.");
            continue;
        }

        i64 size;
        FILE* file = fopen(filename, "rb");
        if (file == NULL) {
            Console.error("Could not open file.");
            return EXIT_FAILURE;
        }
        fseek(file, 0, SEEK_END);
        size = ftell(file);
        fseek(file, 0, SEEK_SET);
        u8* dump = calloc(size, sizeof(u8));
        fread(dump, size, 1, file);
        fclose(file);

        if (types[offset] == INT) {
            i64 new_value = atoi(value);
            if (!bypass && (new_value < low_boundaries[offset] || new_value > high_boundaries[offset])) {
                Console.error("Invalid value. Out of range.");
                continue;
            }
            memcpy(dump + offset, &new_value, field_sizes[offset]);
        }
        else if (types[offset] == STR_EN) {
            if (!bypass && (strlen(value) < low_boundaries[offset] || strlen(value) > high_boundaries[offset])) {
                Console.error("Invalid value. Too short or too long.");
                continue;
            }
            memcpy(dump + offset, value, field_sizes[offset]);
        }
        else {
            Console.error("Invalid type. Big5 Encoding is not supported.");
            continue;
        }

        FILE* fp = fopen(filename, "w");
        fwrite(dump, sizeof(u8), size, fp);
        fclose(fp);
        Console.success("Saved.");
        if (offset >= dump[0x580] * 110 + 0x584) {
            Console.warn("You may need to edit the storage size (current: %d). (at %d)", dump[0x580], 0x580);
        }
        free(dump);
        free(value);
    }

    return EXIT_SUCCESS;
}
