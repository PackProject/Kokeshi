#ifndef RP_AUDIO_RP_DUC_SOUND_H
#define RP_AUDIO_RP_DUC_SOUND_H

namespace rp_Duc_sound {

enum ESoundID {
    /* 0x00 */ RP_DUC_AMB_01,
    /* 0x01 */ RP_DUC_SE_TARGET_BOUND_GRASS,
    /* 0x02 */ RP_DUC_SE_TARGET_GRASS_01,
    /* 0x03 */ RP_DUC_SE_TARGET_GRASS_02,
    /* 0x04 */ RP_DUC_SE_GUN_SHOT_01,
    /* 0x05 */ RP_DUC_SE_GUN_SHOT_02,
    /* 0x06 */ RP_DUC_SE_GUN_SHOT_03,
    /* 0x07 */ RP_DUC_SE_GUN_SHOT_04,
    /* 0x08 */ RP_DUC_SE_GUN_SHOT_05,
    /* 0x09 */ RP_DUC_SE_GUN_SHOT_06,
    /* 0x0A */ RP_DUC_SE_GUN_SHOT_BLANK_01,
    /* 0x0B */ RP_DUC_SE_TARGET_BOMB_A_01,
    /* 0x0C */ RP_DUC_SE_TARGET_BOMB_B_01,
    /* 0x0D */ RP_DUC_SE_TARGET_BOMB_B_OK_01,
    /* 0x0E */ RP_DUC_SE_TARGET_BOMB_B_NG_01,
    /* 0x0F */ RP_DUC_SE_TARGET_BOMB_C_01,
    /* 0x10 */ RP_DUC_SE_TARGET_BOMB_C_02,
    /* 0x11 */ RP_DUC_SE_TARGET_BOMB_C_03,
    /* 0x12 */ RP_DUC_SE_TARGET_BOMB_D_01,
    /* 0x13 */ RP_DUC_SE_TARGET_BOMB_E_01,
    /* 0x14 */ RP_DUC_SE_TARGET_BOMB_F_01,
    /* 0x15 */ RP_DUC_SE_TARGET_BOMB_F_02,
    /* 0x16 */ RP_DUC_SE_TARGET_BOMB_SP_A_01,
    /* 0x17 */ RP_DUC_SE_TARGET_BOMB_SP_B_01,
    /* 0x18 */ RP_DUC_SE_TARGET_BOMB_SP_C_01,
    /* 0x19 */ RP_DUC_SE_TARGET_BOMB_SP_D_01,
    /* 0x1A */ RP_DUC_SE_TARGET_BOMB_SP_E_01,
    /* 0x1B */ RP_DUC_SE_TARGET_FLY_A_01,
    /* 0x1C */ RP_DUC_SE_TARGET_FLY_B_01,
    /* 0x1D */ RP_DUC_SE_TARGET_FLY_C_01,
    /* 0x1E */ RP_DUC_SE_TARGET_FLY_C_CON_01,
    /* 0x1F */ RP_DUC_SE_TARGET_FLY_D_01,
    /* 0x20 */ RP_DUC_SE_TARGET_FLY_E_01,
    /* 0x21 */ RP_DUC_SE_TARGET_FLY_F_01,
    /* 0x22 */ RP_DUC_SE_OPEN_FLY_01,
    /* 0x23 */ RP_DUC_SE_SIGN_LEVEL_01,
    /* 0x24 */ RP_DUC_SE_SIGN_LEVEL_FINAL,
    /* 0x25 */ RP_DUC_SE_SIGN_AMMO_01,
    /* 0x26 */ RP_DUC_SE_SIGN_AMMO_CON_01,
    /* 0x27 */ RP_DUC_SE_SIGN_TARGETS_01,
    /* 0x28 */ RP_DUC_SE_GUN_SHOT_21,
    /* 0x29 */ RP_DUC_SE_GUN_SHOT_22,
    /* 0x2A */ RP_DUC_SE_GUN_SHOT_23,
    /* 0x2B */ RP_DUC_SE_GUN_SHOT_24,
    /* 0x2C */ RP_DUC_SE_GUN_SHOT_25,
    /* 0x2D */ RP_DUC_SE_GUN_SHOT_26,
    /* 0x2E */ RP_DUC_SE_PLAY_DOG_01,
    /* 0x2F */ RP_DUC_SE_DEMO_DOG_01,
    /* 0x30 */ RP_DUC_SE_DOLL_AWAY_01,
    /* 0x31 */ RP_DUC_SE_DOLL_SAFE_01,
    /* 0x32 */ RP_DUC_SE_DOLL_HAPPY_01,
    /* 0x33 */ RP_DUC_SE_DOLL_TALK_01,
    /* 0x34 */ RP_DUC_SE_DOLL_ON_01,
    /* 0x35 */ RP_DUC_SE_DOLL_OFF_01,
    /* 0x36 */ RP_DUC_SE_2P_CON_ON_1,
    /* 0x37 */ RP_DUC_SE_2P_CON_ON_2,
    /* 0x38 */ RP_COM_GAME_SE_PAUSE_01_CTSP,
    /* 0x39 */ RP_COM_GAME_SE_PAUSE_01_TVSP,
    /* 0x3A */ RP_COM_GAME_SE_PAUSE_01,
    /* 0x3B */ RP_COM_GAME_PARTY_SE_A_01_CTSP,
    /* 0x3C */ RP_COM_GAME_PARTY_SE_A_01_TVSP,
    /* 0x3D */ RP_COM_GAME_PARTY_SE_A_01,
    /* 0x3E */ RP_COM_GAME_PARTY_SE_Cursor_01,
    /* 0x3F */ RP_COM_GAME_PARTY_SE_LANK_01,
    /* 0x40 */ RP_COM_GAME_PARTY_SE_Win_Open_01,
    /* 0x41 */ RP_COM_GAME_PARTY_SE_Win_Close_01,
    /* 0x42 */ RP_COM_GAME_PARTY_SE_COUNT_TIME_01,
    /* 0x43 */ RP_COM_GAME_PARTY_SE_COUNT_TIME_02,
    /* 0x44 */ RP_COM_GAME_PARTY_SE_COUNT_TIME_03,
    /* 0x45 */ RP_COM_GAME_PARTY_SE_COUNT_TIME_x_01,
    /* 0x46 */ RP_COM_GAME_PARTY_SE_COUNT_01,
    /* 0x47 */ RP_COM_GAME_PARTY_SE_COUNT_02,
    /* 0x48 */ RP_COM_GAME_PARTY_SE_RESULT_KEY_A_01_CTSP,
    /* 0x49 */ RP_COM_GAME_PARTY_SE_RESULT_KEY_A_01_TVSP,
    /* 0x4A */ RP_COM_GAME_PARTY_SE_RESULT_KEY_A_01,
    /* 0x4B */ RP_COM_GAME_PARTY_SE_RESULT_KEY_A_02_CTSP,
    /* 0x4C */ RP_COM_GAME_PARTY_SE_RESULT_KEY_A_02_TVSP,
    /* 0x4D */ RP_COM_GAME_PARTY_SE_RESULT_KEY_A_02,
    /* 0x4E */ RP_COM_GAME_PARTY_SE_BUTTON_03,
    /* 0x4F */ RP_COM_GAME_PARTY_SE_BUTTON_05,
    /* 0x50 */ RP_COM_GAME_SE_PAUSE_KEY_A_01_CTSP,
    /* 0x51 */ RP_COM_GAME_SE_PAUSE_KEY_A_01_TVSP,
    /* 0x52 */ RP_COM_GAME_SE_PAUSE_KEY_A_01,
    /* 0x53 */ RP_COM_GAME_SE_PAUSE_KEY_A_02_CTSP,
    /* 0x54 */ RP_COM_GAME_SE_PAUSE_KEY_A_02_TVSP,
    /* 0x55 */ RP_COM_GAME_SE_PAUSE_KEY_A_02,
    /* 0x56 */ RP_COM_GAME_SE_PAUSE_KEY_A_03_CTSP,
    /* 0x57 */ RP_COM_GAME_SE_PAUSE_KEY_A_03_TVSP,
    /* 0x58 */ RP_COM_GAME_SE_PAUSE_KEY_A_03,
    /* 0x59 */ RP_COM_GAME_SE_PAUSE_Cursor_01,
    /* 0x5A */ RP_COM_GAME_SE_2P_WIN_LOSE_01,
    /* 0x5B */ RP_COMMON_SE_MESS_10,
    /* 0x5C */ RP_COMMON_SE_MESS_20,
    /* 0x5D */ RP_COMMON_SE_KEY_A_04_CTSP,
    /* 0x5E */ RP_COMMON_SE_KEY_A_04_TVSP,
    /* 0x5F */ RP_COMMON_SE_KEY_A_04,
    /* 0x60 */ RP_COMMON_SE_KEY_BACK_01_CTSP,
    /* 0x61 */ RP_COMMON_SE_KEY_BACK_01_TVSP,
    /* 0x62 */ RP_COMMON_SE_KEY_BACK_01,
    /* 0x63 */ RP_COMMON_SE_KEY_CURSOR_01,
    /* 0x64 */ RP_DUC_BGM_START,
    /* 0x65 */ RP_DUC_BGM_LEVEL_UP,
    /* 0x66 */ RP_DUC_BGM_LEVEL_FIN,
    /* 0x67 */ RP_DUC_BGM_FANFARE_WIN,
    /* 0x68 */ RP_DUC_BGM_FANFARE_DRAW,
    /* 0x69 */ RP_DUC_BGM_COMBO_HIT,
    /* 0x6A */ RP_DUC_BGM_COMBO_BONUS,
    /* 0x6B */ RP_COM_BGM_GET_COPPER,
    /* 0x6C */ RP_COM_BGM_GET_SILVER,
    /* 0x6D */ RP_COM_BGM_GET_GOLD,
    /* 0x6E */ RP_COM_BGM_GET_PLATINA,
    /* 0x6F */ RP_COM_BGM_RENEW_RECORD2,
    /* 0x70 */ RP_COM_BGM_RENEW_RECORD
};

} // namespace rp_Duc_sound

#endif
