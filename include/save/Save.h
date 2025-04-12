/**
 * @file Save.h
 * @brief Raw save data structures
 *
 * @note source: https://github.com/k64ret/cv64/blob/main/include/game/save.h
 */

#ifndef SAVE_H
#define SAVE_H

#include "include/bit.h"

#define NUM_EVENT_FLAGS  16
#define SIZE_ITEMS_ARRAY 64
#define NUM_SAVES 4

/**
 * @class SaveData
 * @brief The main save data structure
 *
 * This structure contains the gameplay variables from one single save inside the game.
 */
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

    // PAL-only
    /* 0x058 */ short language;
    /* 0x05A */ short padding5A_PAL;

    /* 0x058 */ short character;
    /* 0x05A */ short life;
    /**
     * Only set to 100, like the life, and never used otherwise.
     */
    /* 0x05C */ short field_0x5C;
    /* 0x05E */ short subweapon;
    /* 0x060 */ unsigned int gold;
    /* 0x064 */ unsigned char items[SIZE_ITEMS_ARRAY];
    /* 0x0A4 */ unsigned int player_status;
    /* 0x0A8 */ short health_depletion_rate_while_poisoned;
    /**
    * If greater than 24 (midnight), the player turns into a vampire
    */
    /* 0x0AA */ unsigned short current_hour_VAMP;
    /* 0x0AC */ short map;
    /* 0x0AE */ short spawn;
    /* 0x0B0 */ unsigned short save_crystal_number;
    /* 0x0B2 */ unsigned char field51_0xb2;
    /* 0x0B3 */ unsigned char field52_0xb3;
    /* 0x0B4 */ unsigned int time_saved_counter;
    /* 0x0B8 */ unsigned int death_counter;
    /* 0x0BC */ int field55_0xbc;
    /* 0x0C0 */ int field59_0xc0;
    /* 0x0C4 */ int field63_0xc4;
    /* 0x0C8 */ short field67_0xc8;
    /* 0x0CA */ short field69_0xca;
    /* 0x0CC */ int field71_0xcc;
    /* 0x0D0 */ int field75_0xd0;
    /* 0x0D2 */ short field77_0xd2;
    /* 0x0D4 */ short field79_0xd4;
    /* 0x0D8 */ int field83_0xd8;
    /* 0x0DC */ unsigned int gold_spent_on_Renon;

    enum eItemId {
        ITEM_ID_NOTHING           = 0,
        ITEM_ID_WHITE_JEWEL       = 1,
        ITEM_ID_RED_JEWEL         = 2,  /// @note We only used item ID 2 to reference the red jewels, and not 3.
        ITEM_ID_SPECIAL1          = 4,
        ITEM_ID_SPECIAL2          = 5,
        ITEM_ID_SPECIAL3          = 6,  // JPN and PAL-exclusive

        /// @note These items IDs are the same ID, but added +1 in the JPN and PAL versions.
        /// Shown below are their IDs in the USA version, used as a base
        ITEM_ID_ROAST_CHICKEN     = 6,
        ITEM_ID_ROAST_BEEF        = 7,
        ITEM_ID_HEALING_KIT       = 8,
        ITEM_ID_PURIFYING         = 9,
        ITEM_ID_CURE_AMPOULE      = 10,

        ITEM_ID_POUT_POURRI       = 11, // USA-exclusive
        ITEM_ID_POWERUP           = 12,

        // We don't take into account the subweapons here, as we won't treat them as items.

        ITEM_ID_THE_CONTRACT      = 19,
        ITEM_ID_ENGAGEMENT_RING   = 20,
        ITEM_ID_MAGICAL_NITRO     = 21,
        ITEM_ID_MANDRAGORA        = 22,
        ITEM_ID_SUN_CARD          = 23,
        ITEM_ID_MOON_CARD         = 24,
        ITEM_ID_INCANDESCENT_GAZE = 25,
        ITEM_ID_ARCHIVES_KEY      = 26,
        ITEM_ID_LEFT_TOWER_KEY    = 27,
        ITEM_ID_STOREROOM_KEY     = 28,
        ITEM_ID_GARDEN_KEY        = 29,
        ITEM_ID_COPPER_KEY        = 30,
        ITEM_ID_CHAMBER_KEY       = 31,
        ITEM_ID_EXECUTION_KEY     = 32,
        ITEM_ID_SCIENCE_KEY1      = 33,
        ITEM_ID_SCIENCE_KEY2      = 34,
        ITEM_ID_SCIENCE_KEY3      = 35,
        ITEM_ID_CLOCKTOWER_KEY1   = 36,
        ITEM_ID_CLOCKTOWER_KEY2   = 37,
        ITEM_ID_CLOCKTOWER_KEY3   = 38,
        ITEM_ID_GOLD_500          = 39,
        ITEM_ID_GOLD_300          = 40,
        ITEM_ID_GOLD_100          = 41
    };

    enum PlayerCharacterID {
        REINHARDT = 0,
        CARRIE    = 1
    };

    enum SubweaponID {
        SUBWEAPON_NONE         = 0,
        SUBWEAPON_KNIFE        = 1,
        SUBWEAPON_HOLY_WATER   = 2,
        SUBWEAPON_CROSS        = 3,
        SUBWEAPON_AXE          = 4,
        SUBWEAPON_WOODEN_STAKE = 5,
        SUBWEAPON_ROSE         = 6
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

    enum PlayerStatus {
        PLAYER_FLAG_STO                      = BIT(26),
        PLAYER_FLAG_VAMP                     = BIT(27),
        PLAYER_FLAG_POISON                   = BIT(28)
    };

    enum SaveFlag {
        SAVE_FLAG_ACTIVE                     = BIT(0),
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

    enum eRegion {
        USA,    // Americas
        JPN,    // Japan
        PAL     // Europe
    };

    /// @note Only has an effect in PAL saves
    enum eLanguage {
        ENGLISH = 0,
        JAPANESE = 0,
        GERMAN,
        FRENCH
    };

    inline unsigned int getPlayerStatus(unsigned int bitFlagMask) {
        return BITS_HAS(player_status, bitFlagMask);
    }

    inline unsigned int getFlag(unsigned int bitFlagMask) {
        return BITS_HAS(flags, bitFlagMask);
    }

    inline unsigned int getItem(int itemId) {
        return items[itemId - 1];
    }

    inline unsigned int getEventFlags(int setIndex) {
        return event_flags[setIndex];
    }
}; /// @note Size = 0x0E0 bytes

/**
 * @class SaveSlot
 * @brief Save slot container struct
 *
 * This structure contains all the save data associated to one single slot / file.
 *
 * It consists of the following variables:
 *     - mainSave: This is the save data that contains the variables mostly used throughout the game.
 *
 *     - beginningOfStage: The game only saves to this save when entering a stage / map for the first time.
 *                         It is used as a sort of "backup" in case the player gets stuck in the mainSave.
 *
 *     - checksum1: First checksum value for the save. Used for checking if the file was tampered with or corrupted.
 *
 *     - checksum2: Second checksum value for the save. Used for checking if the file was tampered with or corrupted.
 */
struct SaveSlot {
    SaveData mainSave;
    SaveData beginningOfStage;
    unsigned int checksum1;
    unsigned int checksum2;

    void clear() {
        mainSave = {};
        beginningOfStage = {};
        checksum1 = 0;
        checksum2 = 0;
    }

    void assignDefaultValues() {
        // Since most default values are 0 already, we call clear first,
        // then only write to the values whose default values are non-zero
        clear();

        mainSave.life = 100;
        beginningOfStage.life = 100;
        mainSave.field_0x5C = 100;
        beginningOfStage.field_0x5C = 100;
    }

    SaveSlot() { clear(); }
};  /// Size = 0x200 bytes. @note Its effective size is 0x1C8 bytes, but is padded to 0x200.

#endif
