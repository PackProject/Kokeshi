#ifndef RP_AUDIO_RP_COW_SOUND_H
#define RP_AUDIO_RP_COW_SOUND_H

//! @addtogroup rp_audio
//! @{

//! @file
//! @brief Sound IDs for the RPCowScene sound archive

//! @}

namespace rp_Cow_sound {

enum ESoundID {
    /* 0x00 */ RP_COW_SE_CON_01_L_CTSP,
    /* 0x01 */ RP_COW_SE_CON_01_R_CTSP,
    /* 0x02 */ RP_COW_SE_CON_02_1P_TALK_CTSP,
    /* 0x03 */ RP_COW_SE_CON_02_2P_TALK_CTSP,
    /* 0x04 */ RP_COW_SE_CON_03_1P_WORD_CTSP,
    /* 0x05 */ RP_COW_SE_CON_03_2P_WORD_CTSP,
    /* 0x06 */ RP_COW_SE_CON_04_1P_WORD_CTSP,
    /* 0x07 */ RP_COW_SE_CON_04_2P_WORD_CTSP,
    /* 0x08 */ RP_COW_SE_CON_05_1P_WORD_CTSP,
    /* 0x09 */ RP_COW_SE_CON_05_2P_WORD_CTSP,
    /* 0x0A */ RP_COW_SE_CON_06_1P_WORD_CTSP,
    /* 0x0B */ RP_COW_SE_CON_06_2P_WORD_CTSP,
    /* 0x0C */ RP_COW_SE_CON_07_1P_WORD_CTSP,
    /* 0x0D */ RP_COW_SE_CON_07_2P_WORD_CTSP,
    /* 0x0E */ RP_COW_AMB_01,
    /* 0x0F */ RP_COW_AMB_02,
    /* 0x10 */ RP_COW_SE_FOOTSTEPS_STEP_01,
    /* 0x11 */ RP_COW_SE_FOOTSTEPS_A_00,
    /* 0x12 */ RP_COW_SE_FOOTSTEPS_A_01,
    /* 0x13 */ RP_COW_SE_FOOTSTEPS_A_02,
    /* 0x14 */ RP_COW_SE_FOOTSTEPS_A_03,
    /* 0x15 */ RP_COW_SE_FOOTSTEPS_A_04,
    /* 0x16 */ RP_COW_SE_FOOTSTEPS_A_05,
    /* 0x17 */ RP_COW_SE_FOOTSTEPS_1P_00,
    /* 0x18 */ RP_COW_SE_FOOTSTEPS_1P_01,
    /* 0x19 */ RP_COW_SE_FOOTSTEPS_1P_02,
    /* 0x1A */ RP_COW_SE_FOOTSTEPS_1P_03,
    /* 0x1B */ RP_COW_SE_FOOTSTEPS_1P_04,
    /* 0x1C */ RP_COW_SE_FOOTSTEPS_1P_05,
    /* 0x1D */ RP_COW_SE_FOOTSTEPS_2P_00,
    /* 0x1E */ RP_COW_SE_FOOTSTEPS_2P_01,
    /* 0x1F */ RP_COW_SE_FOOTSTEPS_2P_02,
    /* 0x20 */ RP_COW_SE_FOOTSTEPS_2P_03,
    /* 0x21 */ RP_COW_SE_FOOTSTEPS_2P_04,
    /* 0x22 */ RP_COW_SE_FOOTSTEPS_2P_05,
    /* 0x23 */ RP_COW_SE_FOOTSTEPS_A_00_GRASS,
    /* 0x24 */ RP_COW_SE_FOOTSTEPS_A_01_GRASS,
    /* 0x25 */ RP_COW_SE_FOOTSTEPS_A_02_GRASS,
    /* 0x26 */ RP_COW_SE_FOOTSTEPS_A_03_GRASS,
    /* 0x27 */ RP_COW_SE_FOOTSTEPS_A_04_GRASS,
    /* 0x28 */ RP_COW_SE_FOOTSTEPS_A_05_GRASS,
    /* 0x29 */ RP_COW_SE_FOOTSTEPS_1P_00_GRASS,
    /* 0x2A */ RP_COW_SE_FOOTSTEPS_1P_01_GRASS,
    /* 0x2B */ RP_COW_SE_FOOTSTEPS_1P_02_GRASS,
    /* 0x2C */ RP_COW_SE_FOOTSTEPS_1P_03_GRASS,
    /* 0x2D */ RP_COW_SE_FOOTSTEPS_1P_04_GRASS,
    /* 0x2E */ RP_COW_SE_FOOTSTEPS_1P_05_GRASS,
    /* 0x2F */ RP_COW_SE_FOOTSTEPS_2P_00_GRASS,
    /* 0x30 */ RP_COW_SE_FOOTSTEPS_2P_01_GRASS,
    /* 0x31 */ RP_COW_SE_FOOTSTEPS_2P_02_GRASS,
    /* 0x32 */ RP_COW_SE_FOOTSTEPS_2P_03_GRASS,
    /* 0x33 */ RP_COW_SE_FOOTSTEPS_2P_04_GRASS,
    /* 0x34 */ RP_COW_SE_FOOTSTEPS_2P_05_GRASS,
    /* 0x35 */ RP_COW_SE_FOOTSTEPS_BStream_1P_01,
    /* 0x36 */ RP_COW_SE_FOOTSTEPS_BStream_2P_01,
    /* 0x37 */ RP_COW_SE_START_DASH_1P_01,
    /* 0x38 */ RP_COW_SE_START_DASH_2P_01,
    /* 0x39 */ RP_COW_SE_FOOTSTEPS_BOOST_1P_01,
    /* 0x3A */ RP_COW_SE_FOOTSTEPS_BOOST_2P_01,
    /* 0x3B */ RP_COW_SE_BREAK_A_01,
    /* 0x3C */ RP_COW_SE_BREAK_1P_01,
    /* 0x3D */ RP_COW_SE_BREAK_2P_01,
    /* 0x3E */ RP_COW_JUMP_01,
    /* 0x3F */ RP_COW_JUMP_02,
    /* 0x40 */ RP_COW_JUMP_03,
    /* 0x41 */ RP_COW_JUMP_1P_01,
    /* 0x42 */ RP_COW_JUMP_1P_02,
    /* 0x43 */ RP_COW_JUMP_1P_03,
    /* 0x44 */ RP_COW_JUMP_2P_01,
    /* 0x45 */ RP_COW_JUMP_2P_02,
    /* 0x46 */ RP_COW_JUMP_2P_03,
    /* 0x47 */ RP_COW_SE_DOLL_ON_01,
    /* 0x48 */ RP_COW_SE_DOLL_ON_BONUS_01_2P,
    /* 0x49 */ RP_COW_SE_DOLL_ON_BONUS_01,
    /* 0x4A */ RP_COW_CLASH_01,
    /* 0x4B */ RP_COW_CLASH_COW_01,
    /* 0x4C */ RP_COW_CLASH_FENCE_01,
    /* 0x4D */ RP_COW_CLASH_WALL_01,
    /* 0x4E */ RP_COW_CLASH_BONUS_01,
    /* 0x4F */ RP_COW_SE_DOLL_JUMP_01,
    /* 0x50 */ RP_COW_SE_CLASH_TREE_01,
    /* 0x51 */ RP_COW_SE_GOAL_01,
    /* 0x52 */ RP_COW_SE_GOAL_TIMEUP_01,
    /* 0x53 */ RP_COW_SE_GOAL_TIMEOVER_01,
    /* 0x54 */ RP_COW_SE_GOAL_POINT_0,
    /* 0x55 */ RP_COW_SE_GOAL_TIME_BONUS_01,
    /* 0x56 */ RP_COW_SYS_CountPoint_01,
    /* 0x57 */ RP_COW_SYS_CountPoint_02,
    /* 0x58 */ RP_COW_SE_ROD_01,
    /* 0x59 */ RP_COW_SE_ROD_02,
    /* 0x5A */ RP_COW_SE_FOOTSTEPS_01,
    /* 0x5B */ RP_COW_SYS_ALARM_01,
    /* 0x5C */ RP_COW_ROPE_SWING_01,
    /* 0x5D */ RP_COW_SE_FOOTSTEPS_C_01,
    /* 0x5E */ RP_COW_SE_FOOTSTEPS_C_02,
    /* 0x5F */ RP_COW_SE_FOOTSTEPS_C_03,
    /* 0x60 */ RP_COW_SE_FOOTSTEPS_C_04,
    /* 0x61 */ RP_COW_SE_FOOTSTEPS_C_05,
    /* 0x62 */ RP_COW_SE_FOOTSTEPS_D_01,
    /* 0x63 */ RP_COW_SE_FOOTSTEPS_D_02,
    /* 0x64 */ RP_COW_SE_FOOTSTEPS_D_03,
    /* 0x65 */ RP_COW_SE_FOOTSTEPS_D_04,
    /* 0x66 */ RP_COW_SE_FOOTSTEPS_D_05,
    /* 0x67 */ RP_COM_GAME_SE_PAUSE_01_CTSP,
    /* 0x68 */ RP_COM_GAME_SE_PAUSE_01_TVSP,
    /* 0x69 */ RP_COM_GAME_SE_PAUSE_01,
    /* 0x6A */ RP_COM_GAME_PARTY_SE_A_01_CTSP,
    /* 0x6B */ RP_COM_GAME_PARTY_SE_A_01_TVSP,
    /* 0x6C */ RP_COM_GAME_PARTY_SE_A_01,
    /* 0x6D */ RP_COM_GAME_PARTY_SE_Cursor_01,
    /* 0x6E */ RP_COM_GAME_PARTY_SE_LANK_01,
    /* 0x6F */ RP_COM_GAME_PARTY_SE_Win_Open_01,
    /* 0x70 */ RP_COM_GAME_PARTY_SE_Win_Close_01,
    /* 0x71 */ RP_COM_GAME_PARTY_SE_COUNT_TIME_01,
    /* 0x72 */ RP_COM_GAME_PARTY_SE_COUNT_TIME_02,
    /* 0x73 */ RP_COM_GAME_PARTY_SE_COUNT_TIME_03,
    /* 0x74 */ RP_COM_GAME_PARTY_SE_COUNT_TIME_x_01,
    /* 0x75 */ RP_COM_GAME_PARTY_SE_COUNT_01,
    /* 0x76 */ RP_COM_GAME_PARTY_SE_COUNT_02,
    /* 0x77 */ RP_COM_GAME_PARTY_SE_RESULT_KEY_A_01_CTSP,
    /* 0x78 */ RP_COM_GAME_PARTY_SE_RESULT_KEY_A_01_TVSP,
    /* 0x79 */ RP_COM_GAME_PARTY_SE_RESULT_KEY_A_01,
    /* 0x7A */ RP_COM_GAME_PARTY_SE_RESULT_KEY_A_02_CTSP,
    /* 0x7B */ RP_COM_GAME_PARTY_SE_RESULT_KEY_A_02_TVSP,
    /* 0x7C */ RP_COM_GAME_PARTY_SE_RESULT_KEY_A_02,
    /* 0x7D */ RP_COM_GAME_PARTY_SE_BUTTON_03,
    /* 0x7E */ RP_COM_GAME_PARTY_SE_BUTTON_05,
    /* 0x7F */ RP_COM_GAME_SE_PAUSE_KEY_A_01_CTSP,
    /* 0x80 */ RP_COM_GAME_SE_PAUSE_KEY_A_01_TVSP,
    /* 0x81 */ RP_COM_GAME_SE_PAUSE_KEY_A_01,
    /* 0x82 */ RP_COM_GAME_SE_PAUSE_KEY_A_02_CTSP,
    /* 0x83 */ RP_COM_GAME_SE_PAUSE_KEY_A_02_TVSP,
    /* 0x84 */ RP_COM_GAME_SE_PAUSE_KEY_A_02,
    /* 0x85 */ RP_COM_GAME_SE_PAUSE_KEY_A_03_CTSP,
    /* 0x86 */ RP_COM_GAME_SE_PAUSE_KEY_A_03_TVSP,
    /* 0x87 */ RP_COM_GAME_SE_PAUSE_KEY_A_03,
    /* 0x88 */ RP_COM_GAME_SE_PAUSE_Cursor_01,
    /* 0x89 */ RP_COM_GAME_SE_2P_WIN_LOSE_01,
    /* 0x8A */ RP_COMMON_SE_MESS_10,
    /* 0x8B */ RP_COMMON_SE_MESS_20,
    /* 0x8C */ RP_COMMON_SE_KEY_A_04_CTSP,
    /* 0x8D */ RP_COMMON_SE_KEY_A_04_TVSP,
    /* 0x8E */ RP_COMMON_SE_KEY_A_04,
    /* 0x8F */ RP_COMMON_SE_KEY_BACK_01_CTSP,
    /* 0x90 */ RP_COMMON_SE_KEY_BACK_01_TVSP,
    /* 0x91 */ RP_COMMON_SE_KEY_BACK_01,
    /* 0x92 */ RP_COMMON_SE_KEY_CURSOR_01,
    /* 0x93 */ RP_COW_BGM_RESULT,
    /* 0x94 */ RP_COW_BGM_TIMEUPRESULT,
    /* 0x95 */ RP_COW_BGM_PLAY,
    /* 0x96 */ RP_COW_BGM_OPENING,
    /* 0x97 */ RP_COW_BGM_PERFECT,
    /* 0x98 */ RP_COW_BGM_ALLPERFECT,
    /* 0x99 */ RP_COW_BGM_PERFECT_START,
    /* 0x9A */ RP_COW_BGM_PERFECT_OOSAMA,
    /* 0x9B */ RP_COM_BGM_GET_COPPER,
    /* 0x9C */ RP_COM_BGM_GET_SILVER,
    /* 0x9D */ RP_COM_BGM_GET_GOLD,
    /* 0x9E */ RP_COM_BGM_GET_PLATINA,
    /* 0x9F */ RP_COM_BGM_RENEW_RECORD2,
    /* 0xA0 */ RP_COM_BGM_RENEW_RECORD
};

} // namespace rp_Cow_sound

#endif
