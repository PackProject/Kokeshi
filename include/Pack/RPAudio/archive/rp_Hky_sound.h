#ifndef RP_AUDIO_RP_HKY_SOUND_H
#define RP_AUDIO_RP_HKY_SOUND_H

namespace rp_Hky_sound {

enum ESoundID {
    /* 0x00 */ RP_HKY_SE_HIT_A_01,
    /* 0x01 */ RP_HKY_SE_HIT_B_01,
    /* 0x02 */ RP_HKY_SE_Bound_Wall_A_01,
    /* 0x03 */ RP_HKY_SE_Bound_Wall_B_01,
    /* 0x04 */ RP_HKY_SE_GOAL_A_01,
    /* 0x05 */ RP_HKY_SE_GOAL_B_01,
    /* 0x06 */ RP_HKY_SE_GOAL_A_CON_01,
    /* 0x07 */ RP_HKY_SE_GOAL_B_CON_01,
    /* 0x08 */ RP_HKY_SE_Count_01,
    /* 0x09 */ RP_HKY_SE_Count_02,
    /* 0x0A */ RP_HKY_SE_Count_POINT_01,
    /* 0x0B */ RP_HKY_SE_Count_POINT_02,
    /* 0x0C */ RP_HKY_SYS_TIME_COUNT_01,
    /* 0x0D */ RP_HKY_SYS_TIME_COUNT_02,
    /* 0x0E */ RP_HKY_SYS_TIME_COUNT_03,
    /* 0x0F */ RP_HKY_SYS_TIME_COUNT_10,
    /* 0x10 */ RP_HKY_SE_SWING_01,
    /* 0x11 */ RP_HKY_SE_SWING_02,
    /* 0x12 */ RP_HKY_SE_FINISH_01,
    /* 0x13 */ RP_HKY_SE_Pack_Change_01,
    /* 0x14 */ RP_HKY_SE_Pack_Change_02,
    /* 0x15 */ RP_COM_GAME_SE_PAUSE_01_CTSP,
    /* 0x16 */ RP_COM_GAME_SE_PAUSE_01_TVSP,
    /* 0x17 */ RP_COM_GAME_SE_PAUSE_01,
    /* 0x18 */ RP_COM_GAME_PARTY_SE_A_01_CTSP,
    /* 0x19 */ RP_COM_GAME_PARTY_SE_A_01_TVSP,
    /* 0x1A */ RP_COM_GAME_PARTY_SE_A_01,
    /* 0x1B */ RP_COM_GAME_PARTY_SE_Cursor_01,
    /* 0x1C */ RP_COM_GAME_PARTY_SE_LANK_01,
    /* 0x1D */ RP_COM_GAME_PARTY_SE_Win_Open_01,
    /* 0x1E */ RP_COM_GAME_PARTY_SE_Win_Close_01,
    /* 0x1F */ RP_COM_GAME_PARTY_SE_COUNT_TIME_01,
    /* 0x20 */ RP_COM_GAME_PARTY_SE_COUNT_TIME_02,
    /* 0x21 */ RP_COM_GAME_PARTY_SE_COUNT_TIME_03,
    /* 0x22 */ RP_COM_GAME_PARTY_SE_COUNT_TIME_x_01,
    /* 0x23 */ RP_COM_GAME_PARTY_SE_COUNT_01,
    /* 0x24 */ RP_COM_GAME_PARTY_SE_COUNT_02,
    /* 0x25 */ RP_COM_GAME_PARTY_SE_RESULT_KEY_A_01_CTSP,
    /* 0x26 */ RP_COM_GAME_PARTY_SE_RESULT_KEY_A_01_TVSP,
    /* 0x27 */ RP_COM_GAME_PARTY_SE_RESULT_KEY_A_01,
    /* 0x28 */ RP_COM_GAME_PARTY_SE_RESULT_KEY_A_02_CTSP,
    /* 0x29 */ RP_COM_GAME_PARTY_SE_RESULT_KEY_A_02_TVSP,
    /* 0x2A */ RP_COM_GAME_PARTY_SE_RESULT_KEY_A_02,
    /* 0x2B */ RP_COM_GAME_PARTY_SE_BUTTON_03,
    /* 0x2C */ RP_COM_GAME_PARTY_SE_BUTTON_05,
    /* 0x2D */ RP_COM_GAME_SE_PAUSE_KEY_A_01_CTSP,
    /* 0x2E */ RP_COM_GAME_SE_PAUSE_KEY_A_01_TVSP,
    /* 0x2F */ RP_COM_GAME_SE_PAUSE_KEY_A_01,
    /* 0x30 */ RP_COM_GAME_SE_PAUSE_KEY_A_02_CTSP,
    /* 0x31 */ RP_COM_GAME_SE_PAUSE_KEY_A_02_TVSP,
    /* 0x32 */ RP_COM_GAME_SE_PAUSE_KEY_A_02,
    /* 0x33 */ RP_COM_GAME_SE_PAUSE_KEY_A_03_CTSP,
    /* 0x34 */ RP_COM_GAME_SE_PAUSE_KEY_A_03_TVSP,
    /* 0x35 */ RP_COM_GAME_SE_PAUSE_KEY_A_03,
    /* 0x36 */ RP_COM_GAME_SE_PAUSE_Cursor_01,
    /* 0x37 */ RP_COM_GAME_SE_2P_WIN_LOSE_01,
    /* 0x38 */ RP_COMMON_SE_MESS_10,
    /* 0x39 */ RP_COMMON_SE_MESS_20,
    /* 0x3A */ RP_COMMON_SE_KEY_A_04_CTSP,
    /* 0x3B */ RP_COMMON_SE_KEY_A_04_TVSP,
    /* 0x3C */ RP_COMMON_SE_KEY_A_04,
    /* 0x3D */ RP_COMMON_SE_KEY_BACK_01_CTSP,
    /* 0x3E */ RP_COMMON_SE_KEY_BACK_01_TVSP,
    /* 0x3F */ RP_COMMON_SE_KEY_BACK_01,
    /* 0x40 */ RP_COMMON_SE_KEY_CURSOR_01,
    /* 0x41 */ RP_HKY_GOLL_STRM,
    /* 0x42 */ RP_HKY_GOLL2_STRM,
    /* 0x43 */ RP_HKY_BGM_START,
    /* 0x44 */ RP_HKY_BGM_ALL_PLAY,
    /* 0x45 */ RP_HKY_BGM_ALL_RESULT,
    /* 0x46 */ RP_COM_BGM_GET_COPPER,
    /* 0x47 */ RP_COM_BGM_GET_SILVER,
    /* 0x48 */ RP_COM_BGM_GET_GOLD,
    /* 0x49 */ RP_COM_BGM_GET_PLATINA,
    /* 0x4A */ RP_COM_BGM_RENEW_RECORD2,
    /* 0x4B */ RP_COM_BGM_RENEW_RECORD
};

} // namespace rp_Hky_sound

#endif