#ifndef RP_AUDIO_RP_BOX_SOUND_H
#define RP_AUDIO_RP_BOX_SOUND_H

//! @addtogroup rp_audio
//! @{

//! @file
//! @brief Sound IDs for the RPBoxScene sound archive

//! @}

namespace rp_Box_sound {

enum ESoundID {
    /* 0x000 */ SE_TR_punch_jab,
    /* 0x001 */ SE_TR_punch_jab_TVSP,
    /* 0x002 */ SE_TR_punch_straight,
    /* 0x003 */ SE_TR_punch_straight_TVSP,
    /* 0x004 */ SE_TR_punch_hook,
    /* 0x005 */ SE_TR_punch_hook_TVSP,
    /* 0x006 */ SE_TR_punch_upper,
    /* 0x007 */ SE_TR_punch_upper_TVSP,
    /* 0x008 */ SE_TR_HitPunchGuard,
    /* 0x009 */ SE_TR_HitPunchGuard_TVSP,
    /* 0x00A */ SE_TR_HitPunchSmall,
    /* 0x00B */ SE_TR_HitPunchSmall_TVSP,
    /* 0x00C */ SE_TR_HitPunchMid,
    /* 0x00D */ SE_TR_HitPunchMid_TVSP,
    /* 0x00E */ SE_TR_HitPunchBig,
    /* 0x00F */ SE_TR_HitPunchBig_TVSP,
    /* 0x010 */ SE_TR_HitPunchCounter,
    /* 0x011 */ SE_TR_HitPunchCounter_TVSP,
    /* 0x012 */ SE_TR_HitBody,
    /* 0x013 */ SE_TR_HitBody_TVSP,
    /* 0x014 */ SE_TR_Slow_punch_jab,
    /* 0x015 */ SE_TR_Slow_punch_jab_TVSP,
    /* 0x016 */ SE_TR_Slow_punch_straight,
    /* 0x017 */ SE_TR_Slow_punch_straight_TVSP,
    /* 0x018 */ SE_TR_Slow_punch_hook,
    /* 0x019 */ SE_TR_Slow_punch_hook_TVSP,
    /* 0x01A */ SE_TR_Slow_punch_upper,
    /* 0x01B */ SE_TR_Slow_punch_upper_TVSP,
    /* 0x01C */ SE_TR_Slow_HitPunchGuard,
    /* 0x01D */ SE_TR_Slow_HitPunchGuard_TVSP,
    /* 0x01E */ SE_TR_Slow_HitPunchSmall,
    /* 0x01F */ SE_TR_Slow_HitPunchSmall_TVSP,
    /* 0x020 */ SE_TR_Slow_HitPunchMid,
    /* 0x021 */ SE_TR_Slow_HitPunchMid_TVSP,
    /* 0x022 */ SE_TR_Slow_HitPunchBig,
    /* 0x023 */ SE_TR_Slow_HitPunchBig_TVSP,
    /* 0x024 */ SE_TR_Slow_HitPunchCounter,
    /* 0x025 */ SE_TR_Slow_HitPunchCounter_TVSP,
    /* 0x026 */ SE_TR_Slow_HitBody,
    /* 0x027 */ SE_TR_Slow_HitBody_TVSP,
    /* 0x028 */ SE_TR_body_move,
    /* 0x029 */ SE_TR_body_move_TVSP,
    /* 0x02A */ SE_TR_weave_swey,
    /* 0x02B */ SE_TR_weave_swey_TVSP,
    /* 0x02C */ SE_TR_Training_Sparring_open_mitt_punch,
    /* 0x02D */ SE_TR_Training_Sparring_open_mitt_punch_TVSP,
    /* 0x02E */ SE_TR_Training_Sparring_close_mitt_punch,
    /* 0x02F */ SE_TR_Training_Sparring_close_mitt_punch_TVSP,
    /* 0x030 */ SE_TR_Training_Sparring_trainer_hit_punch,
    /* 0x031 */ SE_TR_Training_Sparring_trainer_hit_punch_TVSP,
    /* 0x032 */ SE_TR_Training_Tamayoke_hit_pinpon,
    /* 0x033 */ SE_TR_Training_Tamayoke_hit_pinpon_TVSP,
    /* 0x034 */ SE_TR_Training_Tamayoke_hit_tennis_ball,
    /* 0x035 */ SE_TR_Training_Tamayoke_hit_tennis_ball_TVSP,
    /* 0x036 */ SE_TR_Training_Tamayoke_hit_baseball_ball,
    /* 0x037 */ SE_TR_Training_Tamayoke_hit_baseball_ball_TVSP,
    /* 0x038 */ SE_TR_Training_Tamayoke_hit_golf_ball,
    /* 0x039 */ SE_TR_Training_Tamayoke_hit_golf_ball_TVSP,
    /* 0x03A */ SE_TR_Training_Tamayoke_hit_bowling_ball,
    /* 0x03B */ SE_TR_Training_Tamayoke_hit_bowling_ball_TVSP,
    /* 0x03C */ SE_TR_Training_SandBag_punch_sandbag,
    /* 0x03D */ SE_TR_Training_SandBag_punch_sandbag_TVSP,
    /* 0x03E */ SE_TR_Training_SandBag_futtobu_sandbag,
    /* 0x03F */ SE_TR_Training_SandBag_futtobu_sandbag_TVSP,
    /* 0x040 */ SE_TR_down_mat,
    /* 0x041 */ SE_TR_down_mat_TVSP,
    /* 0x042 */ SE_TR_Gong1Shot,
    /* 0x043 */ SE_TR_GongKO,
    /* 0x044 */ SE_TR_WalkMat1,
    /* 0x045 */ SE_TR_WalkMat2,
    /* 0x046 */ SE_TR_WalkMat3,
    /* 0x047 */ SE_TR_WalkMat4,
    /* 0x048 */ SE_TR_WalkRoom1,
    /* 0x049 */ SE_TR_WalkRoom2,
    /* 0x04A */ SE_TR_WalkRoom3,
    /* 0x04B */ SE_TR_WalkRoom4,
    /* 0x04C */ SE_TR_Training_Tamayoke_trainer_throw,
    /* 0x04D */ SE_TR_Training_Tamayoke_ok,
    /* 0x04E */ SE_TR_Training_SandBag_down_sandbag,
    /* 0x04F */ SE_TR_Training_SandBag_up_sandbag,
    /* 0x050 */ SE_TR_HanteiKekkaHyouji,
    /* 0x051 */ SE_TR_SandBagHitWall,
    /* 0x052 */ SE_TR_SandBagHitFloor,
    /* 0x053 */ SE_TR_Tired_BOY,
    /* 0x054 */ SE_TR_Tired_BOY_TVSP,
    /* 0x055 */ SE_TR_Tired_GAL,
    /* 0x056 */ SE_TR_Tired_GAL_TVSP,
    /* 0x057 */ SE_TR_Training_punch_jab,
    /* 0x058 */ SE_TR_Training_punch_jab_TVSP,
    /* 0x059 */ SE_TR_Training_punch_straight,
    /* 0x05A */ SE_TR_Training_punch_straight_TVSP,
    /* 0x05B */ SE_TR_Training_punch_hook,
    /* 0x05C */ SE_TR_Training_punch_hook_TVSP,
    /* 0x05D */ SE_TR_Training_punch_upper,
    /* 0x05E */ SE_TR_Training_punch_upper_TVSP,
    /* 0x05F */ SE_TR_Training_HitPunchSmall,
    /* 0x060 */ SE_TR_Training_HitPunchSmall_TVSP,
    /* 0x061 */ SE_TR_Training_HitPunchMid,
    /* 0x062 */ SE_TR_Training_HitPunchMid_TVSP,
    /* 0x063 */ SE_TR_Training_HitPunchBig,
    /* 0x064 */ SE_TR_Training_HitPunchBig_TVSP,
    /* 0x065 */ SE_TR_Training_HitBody,
    /* 0x066 */ SE_TR_Training_HitBody_TVSP,
    /* 0x067 */ SE_TR_Training_CountDown,
    /* 0x068 */ SE_TR_DamageMeter_L,
    /* 0x069 */ SE_TR_DamageMeter_R,
    /* 0x06A */ SE_TR_DamageMeter_C,
    /* 0x06B */ SE_TR_DamageMeter_L_Me,
    /* 0x06C */ SE_TR_DamageMeter_R_Me,
    /* 0x06D */ SE_TR_DamageMeter_C_Me,
    /* 0x06E */ SE_TR_DamageMeter_Recover_L,
    /* 0x06F */ SE_TR_DamageMeter_Recover_R,
    /* 0x070 */ SE_TR_DamageMeter_Recover_C,
    /* 0x071 */ SE_TR_parrying_wait,
    /* 0x072 */ SE_TR_parrying_ok,
    /* 0x073 */ SE_TR_parrying_wait_TVSP,
    /* 0x074 */ SE_TR_parrying_ok_TVSP,
    /* 0x075 */ SE_TR_punch_homing_CTSP,
    /* 0x076 */ SE_TR_punch_homing_TVSP,
    /* 0x077 */ SE_TR_Training_Sparring_trainer_hit_voice,
    /* 0x078 */ SE_TR_Training_Sparring_trainer_punch_NG_voice,
    /* 0x079 */ SE_TR_Training_Sparring_trainer_praise,
    /* 0x07A */ SE_TR_Training_Tamayoke_trainer_throw_voice,
    /* 0x07B */ SE_TR_Training_Tamayoke_TrainerIkigire,
    /* 0x07C */ SE_LV_SlowMotion,
    /* 0x07D */ SE_LV_Kansei,
    /* 0x07E */ SE_LV_Kansei2,
    /* 0x07F */ SE_LV_Kansei3,
    /* 0x080 */ SE_LV_Kansei_small,
    /* 0x081 */ SE_LV_Kansei2_small,
    /* 0x082 */ SE_LV_Kansei3_small,
    /* 0x083 */ SE_LV_TrainingEnv,
    /* 0x084 */ SE_TR_KanseiHit,
    /* 0x085 */ SE_TR_KanseiHitMid,
    /* 0x086 */ SE_TR_Kansei_Daikansei,
    /* 0x087 */ SE_TR_Kansei_Yaji,
    /* 0x088 */ SE_TR_Kansei_Down,
    /* 0x089 */ SE_TR_Kansei_DownFukkatsu,
    /* 0x08A */ SE_TR_Kansei_RoundFinish,
    /* 0x08B */ SE_TR_Kansei_MatchFinish,
    /* 0x08C */ SE_TR_Kansei_KnockoutDaikansei,
    /* 0x08D */ BGM_GameResultDisp,
    /* 0x08E */ BGM_Training_Mituchi,
    /* 0x08F */ BGM_Training_Sandbag,
    /* 0x090 */ BGM_Training_Tamayoke,
    /* 0x091 */ BGM_GameSetReplay,
    /* 0x092 */ Fanfare_VS,
    /* 0x093 */ Fanfare_Round,
    /* 0x094 */ Fanfare_Judgment,
    /* 0x095 */ VOICE_TR_Count_1,
    /* 0x096 */ VOICE_TR_Count_1_TVSP,
    /* 0x097 */ VOICE_TR_Count_2,
    /* 0x098 */ VOICE_TR_Count_2_TVSP,
    /* 0x099 */ VOICE_TR_Count_3,
    /* 0x09A */ VOICE_TR_Count_3_TVSP,
    /* 0x09B */ VOICE_TR_Count_4,
    /* 0x09C */ VOICE_TR_Count_4_TVSP,
    /* 0x09D */ VOICE_TR_Count_5,
    /* 0x09E */ VOICE_TR_Count_5_TVSP,
    /* 0x09F */ VOICE_TR_Count_6,
    /* 0x0A0 */ VOICE_TR_Count_6_TVSP,
    /* 0x0A1 */ VOICE_TR_Count_7,
    /* 0x0A2 */ VOICE_TR_Count_7_TVSP,
    /* 0x0A3 */ VOICE_TR_Count_8,
    /* 0x0A4 */ VOICE_TR_Count_8_TVSP,
    /* 0x0A5 */ VOICE_TR_Count_9,
    /* 0x0A6 */ VOICE_TR_Count_9_TVSP,
    /* 0x0A7 */ VOICE_TR_Count_10,
    /* 0x0A8 */ VOICE_TR_Count_10_TVSP,
    /* 0x0A9 */ VOICE_TR_Down_CTSP,
    /* 0x0AA */ VOICE_TR_Down_TVSP,
    /* 0x0AB */ VOICE_TR_KO,
    /* 0x0AC */ VOICE_TR_KO_TVSP,
    /* 0x0AD */ VOICE_TR_Count_1_CPU,
    /* 0x0AE */ VOICE_TR_Count_2_CPU,
    /* 0x0AF */ VOICE_TR_Count_3_CPU,
    /* 0x0B0 */ VOICE_TR_Count_4_CPU,
    /* 0x0B1 */ VOICE_TR_Count_5_CPU,
    /* 0x0B2 */ VOICE_TR_Count_6_CPU,
    /* 0x0B3 */ VOICE_TR_Count_7_CPU,
    /* 0x0B4 */ VOICE_TR_Count_8_CPU,
    /* 0x0B5 */ VOICE_TR_Count_9_CPU,
    /* 0x0B6 */ VOICE_TR_Count_10_CPU,
    /* 0x0B7 */ VOICE_TR_Down_CPU,
    /* 0x0B8 */ VOICE_TR_KO_CPU,
    /* 0x0B9 */ VOICE_TR_Round1,
    /* 0x0BA */ VOICE_TR_Round1_TVSP,
    /* 0x0BB */ VOICE_TR_Round2,
    /* 0x0BC */ VOICE_TR_Round2_TVSP,
    /* 0x0BD */ VOICE_TR_FinalRound,
    /* 0x0BE */ VOICE_TR_FinalRound_TVSP,
    /* 0x0BF */ VOICE_TR_Fight,
    /* 0x0C0 */ VOICE_TR_Fight_TVSP,
    /* 0x0C1 */ RP_COM_GAME_SE_PAUSE_01_CTSP,
    /* 0x0C2 */ RP_COM_GAME_SE_PAUSE_01_TVSP,
    /* 0x0C3 */ RP_COM_GAME_SE_PAUSE_01,
    /* 0x0C4 */ FANFARE_GameSet,
    /* 0x0C5 */ FANFARE_GameSet_Win,
    /* 0x0C6 */ FANFARE_GameSet_Lose,
    /* 0x0C7 */ FANFARE_GameSet_Tie,
    /* 0x0C8 */ SE_TR_Common_KeikokuForDebug,
    /* 0x0C9 */ SE_TR_Common_Sports_Cursol,
    /* 0x0CA */ SE_TR_Common_Sports_Decide,
    /* 0x0CB */ SE_TR_Common_Sports_Decide_TVSP,
    /* 0x0CC */ SE_TR_Common_Sports_DecideBig,
    /* 0x0CD */ SE_TR_Common_Sports_DecideBig_TVSP,
    /* 0x0CE */ SE_TR_Common_Sports_DemoSkipButton,
    /* 0x0CF */ SE_TR_Common_Sports_DemoSkipButton_TVSP,
    /* 0x0D0 */ SE_TR_Common_Sports_Cancel,
    /* 0x0D1 */ SE_TR_Common_Sports_Cancel_TVSP,
    /* 0x0D2 */ SE_TR_Common_Sports_Wipe,
    /* 0x0D3 */ SE_TR_Common_Jyunban,
    /* 0x0D4 */ SE_TR_Common_Jyunban_TVSP,
    /* 0x0D5 */ SE_TR_Common_Medal_Platinum,
    /* 0x0D6 */ SE_TR_Common_Medal_Gold,
    /* 0x0D7 */ SE_TR_Common_Medal_Silver,
    /* 0x0D8 */ SE_TR_Common_Medal_Copper,
    /* 0x0D9 */ SE_TR_Common_Sports_Game_Start,
    /* 0x0DA */ SE_TR_Common_Sports_Game_End,
    /* 0x0DB */ SE_TR_Common_Sports_MyBestRecord,
    /* 0x0DC */ SE_TR_Common_Sports_KanseiRecord,
    /* 0x0DD */ SE_TR_Common_Sports_Training_Good,
    /* 0x0DE */ SE_TR_Common_Sports_Training_GoodLong,
    /* 0x0DF */ SE_TR_Common_Sports_Training_VeryGoodLong,
    /* 0x0E0 */ SE_TR_Common_Sports_Training_GoodLongSoft,
    /* 0x0E1 */ SE_TR_Common_Sports_Training_NotGood,
    /* 0x0E2 */ RP_COM_GAME_SE_PAUSE_KEY_A_02_CTSP,
    /* 0x0E3 */ RP_COM_GAME_SE_PAUSE_KEY_A_02_TVSP,
    /* 0x0E4 */ RP_COM_GAME_SE_PAUSE_KEY_A_02,
    /* 0x0E5 */ RP_COM_GAME_SE_PAUSE_KEY_A_03_CTSP,
    /* 0x0E6 */ RP_COM_GAME_SE_PAUSE_KEY_A_03_TVSP,
    /* 0x0E7 */ RP_COM_GAME_SE_PAUSE_KEY_A_03,
    /* 0x0E8 */ RP_COM_GAME_SE_PAUSE_Cursor_01,
    /* 0x0E9 */ SE_LV_Common_click_CTSP,
    /* 0x0EA */ SE_LV_Common_click_TVSP,
    /* 0x0EB */ SE_TR_Common_ModeChange_CTSP,
    /* 0x0EC */ SE_TR_Common_ModeChange_TVSP,
    /* 0x0ED */ SE_TR_Common_KeyError_CTSP,
    /* 0x0EE */ SE_TR_Common_KeyError_TVSP,
    /* 0x0EF */ RP_COMMON_SE_KEY_CURSOR_01,
    /* 0x0F0 */ RP_COMMON_SE_KEY_BACK_01_CTSP,
    /* 0x0F1 */ RP_COMMON_SE_KEY_BACK_01_TVSP,
    /* 0x0F2 */ SE_TR_COMMON_win_OPEN,
    /* 0x0F3 */ SE_TR_COMMON_win_CLOSE,
    /* 0x0F4 */ SE_TR_COMMON_Jyukurendo_ScoreFlash,
    /* 0x0F5 */ SE_TR_COMMON_Jyukurendo_NameFlash,
    /* 0x0F6 */ SE_TR_COMMON_Jyukurendo_NameSmoke,
    /* 0x0F7 */ SE_TR_COMMON_Push_AB_CTSP,
    /* 0x0F8 */ SE_TR_COMMON_Push_AB_TVSP,
    /* 0x0F9 */ VOICE_TR_Common_BronzeMedal,
    /* 0x0FA */ VOICE_TR_Common_SilverMedal,
    /* 0x0FB */ VOICE_TR_Common_GoldMedal,
    /* 0x0FC */ VOICE_TR_Common_PlatinumMedal,
    /* 0x0FD */ VOICE_TR_Common_HighScore,
    /* 0x0FE */ STRM_Training_Result01,
    /* 0x0FF */ STRM_Training_Result02,
    /* 0x100 */ STRM_Training_Result03,
    /* 0x101 */ Bgm_Tutorial,
    /* 0x102 */ RP_COMMON_SE_MESS_10,
    /* 0x103 */ RP_COMMON_SE_MESS_20,
    /* 0x104 */ RP_COMMON_SE_KEY_A_04_CTSP,
    /* 0x105 */ RP_COMMON_SE_KEY_A_04_TVSP,
    /* 0x106 */ RP_COMMON_SE_KEY_A_04,
    /* 0x107 */ RP_COM_GAME_SE_PAUSE_KEY_A_01_CTSP,
    /* 0x108 */ RP_COM_GAME_SE_PAUSE_KEY_A_01_TVSP,
    /* 0x109 */ RP_COM_GAME_SE_PAUSE_KEY_A_01
};

} // namespace rp_Box_sound

#endif
