// Yoinked from: https://github.com/k64ret/cv64/blob/main/include/game/save.h

#ifndef SAVE_H
#define SAVE_H

#include "include/bit.h"

#define NUM_EVENT_FLAGS  16
#define SIZE_ITEMS_ARRAY 64 // Size of the whole items array
#define NUM_SAVES 4

enum PlayerCharacterID {
    REINHARDT = 0,
    CARRIE    = 1
};

enum SubweaponID {
    SUBWEAPON_NONE       = 0,
    SUBWEAPON_KNIFE      = 1,
    SUBWEAPON_HOLY_WATER = 2,
    SUBWEAPON_CROSS      = 3,
    SUBWEAPON_AXE        = 4
};

enum MapID {
    /** Forest of Silence */
    MORI              = 0,
    /** Castle Wall (Towers) */
    TOU               = 1,
    /** Castle Wall (Main) */
    TOUOKUJI          = 2,
    /** Villa (Yard) */
    NAKANIWA          = 3,
    /** Villa (Foyer) */
    BEKKAN_1F         = 4,
    /** Villa (Hallway) */
    BEKKAN_2F         = 5,
    /** Villa (Maze Garden) */
    MEIRO_TEIEN       = 6,
    /** Tunnel */
    CHIKA_KODO        = 7,
    /** Underground Waterway */
    CHIKA_SUIRO       = 8,
    /** Castle Center (Main) */
    HONMARU_B1F       = 9,
    /** Castle Center (Bottom Elevator) */
    HONMARU_1F        = 10,
    /** Castle Center (Gears) */
    HONMARU_2F        = 11,
    /** Castle Center (Friendly Lizard-man) */
    HONMARU_3F_MINAMI = 12,
    /** Castle Center (Library) */
    HONMARU_4F_MINAMI = 13,
    /** Castle Center (Nitro Room) */
    HONMARU_3F_KITA   = 14,
    /** Castle Center (Top Elevator) */
    HONMARU_5F        = 15,
    /** Tower of Execution */
    SHOKEI_TOU        = 16,
    /** Tower of Sorcery */
    MAHOU_TOU         = 17,
    /** Tower of Science */
    KAGAKU_TOU        = 18,
    /** Duel Tower */
    KETTOU_TOU        = 19,
    /** Castle Keep Stairs */
    TURO_TOKEITOU     = 20,
    /** Castle Keep */
    TENSHU            = 21,
    /** Intro Cutscene Map */
    ENDING_DUMMY      = 22,
    /** Clock Tower */
    TOKEITOU_NAI      = 23,
    /** Dracula Desert */
    DRACULA           = 24,
    /** Rose / Actrice Fan Room */
    ROSE              = 25,
    /** Villa (Vampire Crypt) */
    BEKKAN_BOSS       = 26,
    /** Room of Clocks */
    TOU_TURO          = 27,
    /** Ending Map */
    ENDING            = 28,
    /** Test Grid */
    TEST_GRID         = 29,
    MAP_NONE          = -1
};

enum SaveFlag {
    SAVE_FLAG_GAME_WAS_SAVED_MID_PLAY    = BIT(0),
    SAVE_FLAG_EASY                       = BIT(4),
    SAVE_FLAG_NORMAL                     = BIT(5),
    SAVE_FLAG_HARD                       = BIT(6),
    SAVE_FLAG_HARD_MODE_UNLOCKED         = BIT(8),
    SAVE_FLAG_HAVE_REINHARDT_ALT_COSTUME = BIT(9),
    SAVE_FLAG_HAVE_CARRIE_ALT_COSTUME    = BIT(10),
    SAVE_FLAG_REINDHART_GOOD_ENDING      = BIT(17),
    SAVE_FLAG_CARRIE_GOOD_ENDING         = BIT(18),
    SAVE_FLAG_REINDHART_BAD_ENDING       = BIT(19),
    SAVE_FLAG_CARRIE_BAD_ENDING          = BIT(20),
    SAVE_FLAG_COSTUME_IS_BEING_USED      = BIT(30),
    SAVE_FLAG_CAN_EXPLODE_ON_JUMPING     = BIT(31)
};

struct SaveData {
    /* 0x000 */ unsigned int event_flags[NUM_EVENT_FLAGS];
    /* 0x040 */ unsigned int flags;
    /* 0x044 */ short week;
    /* 0x046 */ short day;
    /* 0x048 */ short hour;
    /* 0x04A */ short minute;
    /* 0x04C */ short seconds;
    /* 0x04E */ unsigned short milliseconds;
    /* 0x050 */ unsigned int gameplay_framecount; // Updates at 60fps
    /* 0x054 */ short button_config;
    /* 0x056 */ short sound_mode;
    /* 0x058 */ short character;
    /* 0x05A */ short life;
    /**
     * Only set to 100, like the life, and never used otherwise.
     * Maybe related to the scrapped S / E meter?
     */
    /* 0x05C */ short field_0x5C;
    /* 0x05E */ short subweapon;
    /* 0x060 */ unsigned int gold;
    /* 0x064 */ unsigned char items[SIZE_ITEMS_ARRAY];
    /* 0xA4 */ unsigned int player_status;
    /* 0xA8 */ short health_depletion_rate_while_poisoned;
    /**
    * If greater than 24 (midnight), the player turns into a vampire
    */
    /* 0xAA */ unsigned short current_hour_VAMP;
    /* 0xAC */ short map;
    /* 0xAE */ short spawn;
    /* 0xB0 */ unsigned char save_crystal_number;
    /* 0xB1 */ unsigned char field50_0xb1;
    /* 0xB2 */ unsigned char field51_0xb2;
    /* 0xB3 */ unsigned char field52_0xb3;
    /* 0xB4 */ unsigned int time_saved_counter;
    /* 0xB8 */ unsigned int death_counter;
    /* 0xBC */ int field55_0xbc;
    /* 0xC0 */ int field59_0xc0;
    /* 0xC4 */ int field63_0xc4;
    /* 0xC8 */ short field67_0xc8;
    /* 0xCA */ short field69_0xca;
    /* 0xCC */ int field71_0xcc;
    /* 0xD0 */ int field75_0xd0;
    /* 0xD2 */ short field77_0xd2;
    /* 0xD4 */ short field79_0xd4;
    /* 0xD8 */ int field83_0xd8;
    /* 0xDC */ unsigned int gold_spent_on_Renon;

    SaveData& operator=(const SaveData& other) {
        if (this != &other) {
            for (unsigned int i = 0; i < NUM_EVENT_FLAGS; i++) {
                event_flags[i] = other.event_flags[i];
            }

            flags = other.flags;
            week = other.week;
            day = other.day;
            hour = other.hour;
            minute = other.minute;
            seconds = other.seconds;
            milliseconds = other.milliseconds;
            gameplay_framecount = other.gameplay_framecount;
            button_config = other.button_config;
            sound_mode = other.sound_mode;
            character = other.character;
            life = other.life;
            field_0x5C = other.field_0x5C;
            subweapon = other.subweapon;
            gold = other.gold;

            for (unsigned int j = 0; j < SIZE_ITEMS_ARRAY; j++) {
                items[j] = other.items[j];
            }

            player_status = other.player_status;
            health_depletion_rate_while_poisoned = other.health_depletion_rate_while_poisoned;
            current_hour_VAMP = other.current_hour_VAMP;
            map = other.map;
            spawn = other.spawn;
            save_crystal_number = other.save_crystal_number;
            field50_0xb1 = other.field50_0xb1;
            field51_0xb2 = other.field51_0xb2;
            field52_0xb3 = other.field52_0xb3;
            time_saved_counter = other.time_saved_counter;
            death_counter = other.death_counter;
            field55_0xbc = other.field55_0xbc;
            field59_0xc0 = other.field59_0xc0;
            field63_0xc4 = other.field63_0xc4;
            field67_0xc8 = other.field67_0xc8;
            field69_0xca = other.field69_0xca;
            field71_0xcc = other.field71_0xcc;
            field75_0xd0 = other.field75_0xd0;
            field77_0xd2 = other.field77_0xd2;
            field79_0xd4 = other.field79_0xd4;
            field83_0xd8 = other.field83_0xd8;
            gold_spent_on_Renon = other.gold_spent_on_Renon;
        }

        return* this;
    }
}; // Size = 0xE0 bytes

#endif
