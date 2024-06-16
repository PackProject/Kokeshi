#ifndef RP_AUDIO_RP_PNP_SOUND_H
#define RP_AUDIO_RP_PNP_SOUND_H

namespace rp_Pnp_sound {

enum ESoundID {
    /* 0x00 */ RP_PNP_SE_HIT_ME_L_01,
    /* 0x01 */ RP_PNP_SE_HIT_ME_L_02,
    /* 0x02 */ RP_PNP_SE_HIT_ME_L_03,
    /* 0x03 */ RP_PNP_SE_HIT_ME_A_1P_01,
    /* 0x04 */ RP_PNP_SE_HIT_ME_A_1P_02,
    /* 0x05 */ RP_PNP_SE_HIT_ME_A_1P_03,
    /* 0x06 */ RP_PNP_SE_HIT_ME_A_2P_01,
    /* 0x07 */ RP_PNP_SE_HIT_ME_A_2P_02,
    /* 0x08 */ RP_PNP_SE_HIT_ME_A_2P_03,
    /* 0x09 */ RP_PNP_SE_HIT_ME_B_1P_01,
    /* 0x0A */ RP_PNP_SE_HIT_ME_B_1P_02,
    /* 0x0B */ RP_PNP_SE_HIT_ME_C_1P_01,
    /* 0x0C */ RP_PNP_SE_HIT_ME_D_1P_01,
    /* 0x0D */ RP_PNP_AMB_01,
    /* 0x0E */ RP_PNP_AMB_02,
    /* 0x0F */ RP_PNP_SE_HIT_ME_01_2,
    /* 0x10 */ RP_PNP_SE_HIT_ME_01_3,
    /* 0x11 */ RP_PNP_SE_HIT_ME_01_4,
    /* 0x12 */ RP_PNP_SE_HIT_ME_01_5,
    /* 0x13 */ RP_PNP_SE_HIT_ME_01,
    /* 0x14 */ RP_PNP_SE_HIT_YOU_01,
    /* 0x15 */ RP_PNP_SE_BOUND_ME_01,
    /* 0x16 */ RP_PNP_SE_BOUND_YOU_01,
    /* 0x17 */ RP_PNP_SE_HIT_ME_NG_01,
    /* 0x18 */ RP_PNP_SE_HIT_YOU_NG_01,
    /* 0x19 */ RP_PNP_SE_HIT_ME_SMASH_01,
    /* 0x1A */ RP_PNP_SE_HIT_YOU_SMASH_01,
    /* 0x1B */ RP_PNP_SE_HIT_1P_01,
    /* 0x1C */ RP_PNP_SE_HIT_2P_01,
    /* 0x1D */ RP_PNP_SE_HIT_1P_NG_01,
    /* 0x1E */ RP_PNP_SE_HIT_2P_NG_01,
    /* 0x1F */ RP_PNP_SE_HIT_1P_SMASH_01,
    /* 0x20 */ RP_PNP_SE_HIT_2P_SMASH_01,
    /* 0x21 */ RP_PNP_SE_BOUND_1P_01,
    /* 0x22 */ RP_PNP_SE_BOUND_2P_01,
    /* 0x23 */ RP_PNP_SE_BOUND_FLOOR_01,
    /* 0x24 */ RP_PNP_SE_BOUND_FLOOR_02,
    /* 0x25 */ RP_PNP_SE_BOUND_FLOOR_03,
    /* 0x26 */ RP_PNP_SE_BOUND_FLOOR_04,
    /* 0x27 */ RP_PNP_SE_CATCH_01,
    /* 0x28 */ RP_PNP_SE_CATCH_02,
    /* 0x29 */ RP_PNP_SE_DETACH_01,
    /* 0x2A */ RP_PNP_SE_NET_01,
    /* 0x2B */ RP_PNP_SE_TOSSUP_01,
    /* 0x2C */ RP_PNP_SE_TOSSUP_02,
    /* 0x2D */ RP_PNP_SE_POINT_01,
    /* 0x2E */ RP_PNP_SE_POINT_02,
    /* 0x2F */ RP_PNP_SE_SWING_A_01,
    /* 0x30 */ RP_PNP_SE_SWING_B_01,
    /* 0x31 */ RP_PNP_SE_SERVECHANGE_ME_01,
    /* 0x32 */ RP_PNP_SE_SERVECHANGE_YOU_01,
    /* 0x33 */ RP_PNP_SE_SERVECHANGE_1P_01,
    /* 0x34 */ RP_PNP_SE_SERVECHANGE_2P_01,
    /* 0x35 */ RP_PNP_SE_SERVECHANGE_TURN_01,
    /* 0x36 */ RP_PNP_SE_CHEER_2P_01,
    /* 0x37 */ RP_PNP_SE_CHEER_01,
    /* 0x38 */ RP_PNP_SE_CHEER_02,
    /* 0x39 */ RP_PNP_SE_SLOWBALL_01,
    /* 0x3A */ RP_PNP_SE_TIMES_FF_01,
    /* 0x3B */ RP_PNP_SE_APPLAUSE_01,
    /* 0x3C */ RP_PNP_SE_APPLAUSE_02,
    /* 0x3D */ RP_PNP_SE_APPLAUSE_03,
    /* 0x3E */ RP_COM_GAME_SE_PAUSE_01_CTSP,
    /* 0x3F */ RP_COM_GAME_SE_PAUSE_01_TVSP,
    /* 0x40 */ RP_COM_GAME_SE_PAUSE_01,
    /* 0x41 */ RP_COM_GAME_PARTY_SE_A_01_CTSP,
    /* 0x42 */ RP_COM_GAME_PARTY_SE_A_01_TVSP,
    /* 0x43 */ RP_COM_GAME_PARTY_SE_A_01,
    /* 0x44 */ RP_COM_GAME_PARTY_SE_Cursor_01,
    /* 0x45 */ RP_COM_GAME_PARTY_SE_LANK_01,
    /* 0x46 */ RP_COM_GAME_PARTY_SE_Win_Open_01,
    /* 0x47 */ RP_COM_GAME_PARTY_SE_Win_Close_01,
    /* 0x48 */ RP_COM_GAME_PARTY_SE_COUNT_TIME_01,
    /* 0x49 */ RP_COM_GAME_PARTY_SE_COUNT_TIME_02,
    /* 0x4A */ RP_COM_GAME_PARTY_SE_COUNT_TIME_03,
    /* 0x4B */ RP_COM_GAME_PARTY_SE_COUNT_TIME_x_01,
    /* 0x4C */ RP_COM_GAME_PARTY_SE_COUNT_01,
    /* 0x4D */ RP_COM_GAME_PARTY_SE_COUNT_02,
    /* 0x4E */ RP_COM_GAME_PARTY_SE_RESULT_KEY_A_01_CTSP,
    /* 0x4F */ RP_COM_GAME_PARTY_SE_RESULT_KEY_A_01_TVSP,
    /* 0x50 */ RP_COM_GAME_PARTY_SE_RESULT_KEY_A_01,
    /* 0x51 */ RP_COM_GAME_PARTY_SE_RESULT_KEY_A_02_CTSP,
    /* 0x52 */ RP_COM_GAME_PARTY_SE_RESULT_KEY_A_02_TVSP,
    /* 0x53 */ RP_COM_GAME_PARTY_SE_RESULT_KEY_A_02,
    /* 0x54 */ RP_COM_GAME_PARTY_SE_BUTTON_03,
    /* 0x55 */ RP_COM_GAME_PARTY_SE_BUTTON_05,
    /* 0x56 */ RP_COM_GAME_SE_PAUSE_KEY_A_01_CTSP,
    /* 0x57 */ RP_COM_GAME_SE_PAUSE_KEY_A_01_TVSP,
    /* 0x58 */ RP_COM_GAME_SE_PAUSE_KEY_A_01,
    /* 0x59 */ RP_COM_GAME_SE_PAUSE_KEY_A_02_CTSP,
    /* 0x5A */ RP_COM_GAME_SE_PAUSE_KEY_A_02_TVSP,
    /* 0x5B */ RP_COM_GAME_SE_PAUSE_KEY_A_02,
    /* 0x5C */ RP_COM_GAME_SE_PAUSE_KEY_A_03_CTSP,
    /* 0x5D */ RP_COM_GAME_SE_PAUSE_KEY_A_03_TVSP,
    /* 0x5E */ RP_COM_GAME_SE_PAUSE_KEY_A_03,
    /* 0x5F */ RP_COM_GAME_SE_PAUSE_Cursor_01,
    /* 0x60 */ RP_COM_GAME_SE_2P_WIN_LOSE_01,
    /* 0x61 */ RP_COMMON_SE_MESS_10,
    /* 0x62 */ RP_COMMON_SE_MESS_20,
    /* 0x63 */ RP_COMMON_SE_KEY_A_04_CTSP,
    /* 0x64 */ RP_COMMON_SE_KEY_A_04_TVSP,
    /* 0x65 */ RP_COMMON_SE_KEY_A_04,
    /* 0x66 */ RP_COMMON_SE_KEY_BACK_01_CTSP,
    /* 0x67 */ RP_COMMON_SE_KEY_BACK_01_TVSP,
    /* 0x68 */ RP_COMMON_SE_KEY_BACK_01,
    /* 0x69 */ RP_COMMON_SE_KEY_CURSOR_01,
    /* 0x6A */ RP_PNP_BGM_START,
    /* 0x6B */ RP_PNP_BGM_PLAY,
    /* 0x6C */ RP_PNP_BGM_PLAY_VS,
    /* 0x6D */ RP_PNP_BGM_FANFARE_WIN,
    /* 0x6E */ RP_PNP_BGM_FANFARE_DRAW,
    /* 0x6F */ RP_PNP_BGM_FANFARE_LOSE,
    /* 0x70 */ RP_PNP_BGM_FANFARE_CONGRATU,
    /* 0x71 */ RP_PNP_BGM_FANFARE_CHAMP,
    /* 0x72 */ RP_PNP_BGM_RESULT,
    /* 0x73 */ RP_COM_BGM_GET_COPPER,
    /* 0x74 */ RP_COM_BGM_GET_SILVER,
    /* 0x75 */ RP_COM_BGM_GET_GOLD,
    /* 0x76 */ RP_COM_BGM_GET_PLATINA,
    /* 0x77 */ RP_COM_BGM_RENEW_RECORD2,
    /* 0x78 */ RP_COM_BGM_RENEW_RECORD
};

} // namespace rp_Pnp_sound

#endif