#include "helper.h"

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
    notes[0x13A] = strdup("父女聊天次數");
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
    for (i32 i = 0; i < 50; i++) {
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
    Console.info(__FMT_YELLOW "Offset: Description" __FMT_NONE);
    for (size_t i = 0; i < 10000; i++) {
        if (notes[i] != NULL) {
            Console.info("%6zu: %s", i, notes[i]);
        }
    }
    Console.info(__FMT_YELLOW "Offset: Description" __FMT_NONE);

    Console.cyan("Please select an offset you want to edit: ");
    i32 offset;
    scanf("%" SCNd32 "%*c", &offset);
    if (notes[offset] == NULL) {
        Console.red("Invalid offset.");
        return EXIT_FAILURE;
    }
    Console.cyan("You selected %d (%s)", offset, notes[offset]);
    Console.cyan("Please enter the new value: ");
    string value = calloc(1024, sizeof(char));
    scanf("%[^\n]%*c", value);
    if (strlen(value) == 0) {
        Console.red("Invalid value. Cannot be empty.");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
