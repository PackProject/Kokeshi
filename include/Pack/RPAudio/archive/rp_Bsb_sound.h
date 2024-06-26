#ifndef RP_AUDIO_RP_BSB_SOUND_H
#define RP_AUDIO_RP_BSB_SOUND_H

//! @addtogroup rp_audio
//! @{

//! @file
//! @brief Sound IDs for the RPBsbScene sound archive

//! @}

namespace rp_Bsb_sound {

enum ESoundID {
    /* 0x00 */ SE_TR_BallFall,
    /* 0x01 */ SE_TR_ScoreDispChange,
    /* 0x02 */ SE_TR_Furikaburi,
    /* 0x03 */ SE_TR_Furikaburi_TVSP,
    /* 0x04 */ SE_TR_Swing,
    /* 0x05 */ SE_TR_Swing_TVSP,
    /* 0x06 */ SE_TR_just_meet,
    /* 0x07 */ SE_TR_just_meet_TVSP,
    /* 0x08 */ SE_TR_Straight,
    /* 0x09 */ SE_TR_Straight_TVSP,
    /* 0x0A */ SE_TR_BallFlyJustMeet,
    /* 0x0B */ SE_TR_BallFlyJustMeet_TVSP,
    /* 0x0C */ SE_TR_meet,
    /* 0x0D */ SE_TR_meet_TVSP,
    /* 0x0E */ SE_TR_FaulChip,
    /* 0x0F */ SE_TR_FaulChip_TVSP,
    /* 0x10 */ SE_TR_CatcherCatch,
    /* 0x11 */ SE_TR_FieldPlayerCatch,
    /* 0x12 */ SE_TR_BallHitFence,
    /* 0x13 */ SE_TR_BallBoundGrass,
    /* 0x14 */ SE_TR_BallBoundDirt,
    /* 0x15 */ SE_TR_BallBound_Dummy,
    /* 0x16 */ SE_TR_FieldPlayerDiving,
    /* 0x17 */ SE_TR_FieldPlayerKokeru,
    /* 0x18 */ SE_TR_BatterStep,
    /* 0x19 */ SE_LV_BallRollGrass,
    /* 0x1A */ SE_LV_BallRollDirt,
    /* 0x1B */ SE_TR_Training_PushA,
    /* 0x1C */ SE_TR_Training_RenzokuBatting_BallHitCombo1,
    /* 0x1D */ SE_TR_Training_RenzokuBatting_BallHitCombo2,
    /* 0x1E */ SE_TR_Training_RenzokuBatting_BallHitCombo3,
    /* 0x1F */ SE_TR_Training_RenzokuBatting_BallHitCombo4,
    /* 0x20 */ SE_TR_Training_RenzokuBatting_BallHitCombo5,
    /* 0x21 */ SE_TR_Training_RenzokuBatting_BallHitCombo6,
    /* 0x22 */ SE_TR_Training_RenzokuBatting_BallHitCombo7,
    /* 0x23 */ SE_TR_Training_RenzokuBatting_BallHitCombo8,
    /* 0x24 */ SE_TR_Training_RenzokuBatting_BallHitCombo9,
    /* 0x25 */ SE_TR_Training_RenzokuBatting_BallHitCombo10,
    /* 0x26 */ SE_TR_Training_RenzokuBatting_BallFaul,
    /* 0x27 */ SE_TR_Training_Uchiwake_HittingAngleDecide,
    /* 0x28 */ SE_TR_Training_Uchiwake_C,
    /* 0x29 */ SE_TR_Training_Uchiwake_L,
    /* 0x2A */ SE_TR_Training_Uchiwake_R,
    /* 0x2B */ SE_TR_Training_Uchiwake_NG,
    /* 0x2C */ SE_TR_Training_Uchiwake_CenterZone,
    /* 0x2D */ SE_TR_Training_Uchiwake_InsideZone,
    /* 0x2E */ SE_TR_Training_Uchiwake_OutsideZone,
    /* 0x2F */ SE_TR_Training_RenzokuBatting_BallHitComboOver,
    /* 0x30 */ SE_TR_Training_Uchiwake_Gyuiiiin,
    /* 0x31 */ SE_TR_KanseiChange,
    /* 0x32 */ SE_LV_Kansei,
    /* 0x33 */ SE_TR_KanseiFun,
    /* 0x34 */ SE_TR_KanseiFun_soon,
    /* 0x35 */ SE_TR_KanseiBooing,
    /* 0x36 */ SE_TR_KanseiSurprise,
    /* 0x37 */ SE_TR_KanseiChangeSide,
    /* 0x38 */ SE_TR_KanseiPlayBall,
    /* 0x39 */ SE_TR_KanseiPlayballShort,
    /* 0x3A */ SE_TR_KanseiGameSet,
    /* 0x3B */ SE_TR_Kansei_UchiwakeZone1,
    /* 0x3C */ SE_TR_Kansei_UchiwakeZone2,
    /* 0x3D */ SE_TR_Kansei_UchiwakeZone3,
    /* 0x3E */ SE_TR_Kansei_UchiwakeFailure,
    /* 0x3F */ SE_TR_Kansei_RenzokuSuccess,
    /* 0x40 */ SE_TR_Kansei_RenzokuFailure,
    /* 0x41 */ SE_TR_Kansei_HomerunFailure,
    /* 0x42 */ BGM_GameResultDisp,
    /* 0x43 */ BGM_TrainingResultDisp,
    /* 0x44 */ BGM_Training_Homerun,
    /* 0x45 */ BGM_Training_Renzoku,
    /* 0x46 */ BGM_Training_Uchiwake,
    /* 0x47 */ FANFARE_SingleHit,
    /* 0x48 */ FANFARE_TwoBaseHit,
    /* 0x49 */ FANFARE_ThreeBaseHit,
    /* 0x4A */ FANFARE_HomeRun,
    /* 0x4B */ FANFARE_OutOfTheParkHomer,
    /* 0x4C */ FANFARE_VS,
    /* 0x4D */ FANFARE_Form_A_Line,
    /* 0x4E */ VOICE_TR_PlayBall,
    /* 0x4F */ VOICE_TR_Change,
    /* 0x50 */ VOICE_TR_GameSet,
    /* 0x51 */ VOICE_TR_Strike,
    /* 0x52 */ VOICE_TR_Ball,
    /* 0x53 */ VOICE_TR_Faul,
    /* 0x54 */ VOICE_TR_HomeRun,
    /* 0x55 */ VOICE_TR_Hit,
    /* 0x56 */ VOICE_TR_BatterOut,
    /* 0x57 */ VOICE_TR_FourBall,
    /* 0x58 */ VOICE_TR_Out,
    /* 0x59 */ VOICE_TR_Fair,
    /* 0x5A */ VOICE_TR_EntitledTwoBase,
    /* 0x5B */ VOICE_TR_TwoBaseHit,
    /* 0x5C */ VOICE_TR_ThreeBaseHit,
    /* 0x5D */ VOICE_TR_DoublePlay,
    /* 0x5E */ VOICE_TR_TriplePlay,
    /* 0x5F */ VOICE_TR_OutOfThePark,
    /* 0x60 */ VOICE_TR_Uchiwake1,
    /* 0x61 */ VOICE_TR_Uchiwake2,
    /* 0x62 */ VOICE_TR_Uchiwake3,
    /* 0x63 */ VOICE_TR_Renzoku5,
    /* 0x64 */ VOICE_TR_Renzoku10,
    /* 0x65 */ VOICE_TR_Renzoku15,
    /* 0x66 */ VOICE_TR_Renzoku20,
    /* 0x67 */ VOICE_TR_Renzoku25,
    /* 0x68 */ VOICE_TR_Renzoku30,
    /* 0x69 */ FANFARE_GameSet,
    /* 0x6A */ FANFARE_GameSet_Win,
    /* 0x6B */ FANFARE_GameSet_Lose,
    /* 0x6C */ FANFARE_GameSet_Tie,
    /* 0x6D */ SE_TR_Common_KeikokuForDebug,
    /* 0x6E */ SE_TR_Common_Sports_Cursol,
    /* 0x6F */ SE_TR_Common_Sports_Decide,
    /* 0x70 */ SE_TR_Common_Sports_Decide_TVSP,
    /* 0x71 */ SE_TR_Common_Sports_DecideBig,
    /* 0x72 */ SE_TR_Common_Sports_DecideBig_TVSP,
    /* 0x73 */ SE_TR_Common_Sports_DemoSkipButton,
    /* 0x74 */ SE_TR_Common_Sports_DemoSkipButton_TVSP,
    /* 0x75 */ SE_TR_Common_Sports_Cancel,
    /* 0x76 */ SE_TR_Common_Sports_Cancel_TVSP,
    /* 0x77 */ SE_TR_Common_Sports_Wipe,
    /* 0x78 */ SE_TR_Common_Jyunban,
    /* 0x79 */ SE_TR_Common_Jyunban_TVSP,
    /* 0x7A */ SE_TR_Common_Medal_Platinum,
    /* 0x7B */ SE_TR_Common_Medal_Gold,
    /* 0x7C */ SE_TR_Common_Medal_Silver,
    /* 0x7D */ SE_TR_Common_Medal_Copper,
    /* 0x7E */ SE_TR_Common_Sports_Game_Start,
    /* 0x7F */ SE_TR_Common_Sports_Game_End,
    /* 0x80 */ SE_TR_Common_Sports_MyBestRecord,
    /* 0x81 */ SE_TR_Common_Sports_KanseiRecord,
    /* 0x82 */ SE_TR_Common_Sports_Training_Good,
    /* 0x83 */ SE_TR_Common_Sports_Training_GoodLong,
    /* 0x84 */ SE_TR_Common_Sports_Training_VeryGoodLong,
    /* 0x85 */ SE_TR_Common_Sports_Training_GoodLongSoft,
    /* 0x86 */ SE_TR_Common_Sports_Training_NotGood,
    /* 0x87 */ RP_COM_GAME_SE_PAUSE_KEY_A_02_CTSP,
    /* 0x88 */ RP_COM_GAME_SE_PAUSE_KEY_A_02_TVSP,
    /* 0x89 */ RP_COM_GAME_SE_PAUSE_KEY_A_02,
    /* 0x8A */ RP_COM_GAME_SE_PAUSE_KEY_A_03_CTSP,
    /* 0x8B */ RP_COM_GAME_SE_PAUSE_KEY_A_03_TVSP,
    /* 0x8C */ RP_COM_GAME_SE_PAUSE_KEY_A_03,
    /* 0x8D */ RP_COM_GAME_SE_PAUSE_Cursor_01,
    /* 0x8E */ SE_LV_Common_click_CTSP,
    /* 0x8F */ SE_LV_Common_click_TVSP,
    /* 0x90 */ SE_TR_Common_ModeChange_CTSP,
    /* 0x91 */ SE_TR_Common_ModeChange_TVSP,
    /* 0x92 */ SE_TR_Common_KeyError_CTSP,
    /* 0x93 */ SE_TR_Common_KeyError_TVSP,
    /* 0x94 */ RP_COMMON_SE_KEY_CURSOR_01,
    /* 0x95 */ RP_COMMON_SE_KEY_BACK_01_CTSP,
    /* 0x96 */ RP_COMMON_SE_KEY_BACK_01_TVSP,
    /* 0x97 */ SE_TR_COMMON_win_OPEN,
    /* 0x98 */ SE_TR_COMMON_win_CLOSE,
    /* 0x99 */ SE_TR_COMMON_Jyukurendo_ScoreFlash,
    /* 0x9A */ SE_TR_COMMON_Jyukurendo_NameFlash,
    /* 0x9B */ SE_TR_COMMON_Jyukurendo_NameSmoke,
    /* 0x9C */ SE_TR_COMMON_Push_AB_CTSP,
    /* 0x9D */ SE_TR_COMMON_Push_AB_TVSP,
    /* 0x9E */ VOICE_TR_Common_BronzeMedal,
    /* 0x9F */ VOICE_TR_Common_SilverMedal,
    /* 0xA0 */ VOICE_TR_Common_GoldMedal,
    /* 0xA1 */ VOICE_TR_Common_PlatinumMedal,
    /* 0xA2 */ VOICE_TR_Common_HighScore,
    /* 0xA3 */ RP_COM_GAME_SE_PAUSE_01_CTSP,
    /* 0xA4 */ RP_COM_GAME_SE_PAUSE_01_TVSP,
    /* 0xA5 */ RP_COM_GAME_SE_PAUSE_01,
    /* 0xA6 */ STRM_Training_Result01,
    /* 0xA7 */ STRM_Training_Result02,
    /* 0xA8 */ STRM_Training_Result03,
    /* 0xA9 */ Bgm_Tutorial,
    /* 0xAA */ RP_COMMON_SE_MESS_10,
    /* 0xAB */ RP_COMMON_SE_MESS_20,
    /* 0xAC */ RP_COMMON_SE_KEY_A_04_CTSP,
    /* 0xAD */ RP_COMMON_SE_KEY_A_04_TVSP,
    /* 0xAE */ RP_COMMON_SE_KEY_A_04,
    /* 0xAF */ RP_COM_GAME_SE_PAUSE_KEY_A_01_CTSP,
    /* 0xB0 */ RP_COM_GAME_SE_PAUSE_KEY_A_01_TVSP,
    /* 0xB1 */ RP_COM_GAME_SE_PAUSE_KEY_A_01
};

} // namespace rp_Bsb_sound

#endif
