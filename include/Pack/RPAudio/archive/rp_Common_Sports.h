#ifndef RP_AUDIO_RP_COMMON_SOUND_H
#define RP_AUDIO_RP_COMMON_SOUND_H

//! @addtogroup rp_audio
//! @{

//! @file
//! @brief Sound IDs for the RPSportsCommon sound archive

//! @}

namespace rp_Common_Sports {

enum ESoundID {
    /* 0x00 */ RP_COMMON_SE_LOGO_01,
    /* 0x01 */ RP_COMMON_SE_WIN_OPEN_01,
    /* 0x02 */ RP_COMMON_SE_WIN_CLOSE_01,
    /* 0x03 */ RP_COMMON_SE_MESS_OK_01_CTSP,
    /* 0x04 */ RP_COMMON_SE_MESS_OK_01_TVSP,
    /* 0x05 */ RP_COMMON_SE_MESS_OK_02_CTSP,
    /* 0x06 */ RP_COMMON_SE_MESS_OK_02_TVSP,
    /* 0x07 */ RP_COMMON_SE_SEARCH_01,
    /* 0x08 */ RP_COMMON_SE_CON_ADD_01,
    /* 0x09 */ RP_COMMON_SE_MESS_FLASH_01,
    /* 0x0A */ STRM_SportsPackTittle,
    /* 0x0B */ STRM_SportsPack_PhysicalMeasure_bridge,
    /* 0x0C */ STRM_Training_Select,
    /* 0x0D */ STRM_Measurement_Bow,
    /* 0x0E */ STRM_Measurement_Box,
    /* 0x0F */ STRM_Measurement_Bsb,
    /* 0x10 */ STRM_Measurement_Gol,
    /* 0x11 */ STRM_Measurement_Tns,
    /* 0x12 */ STRM_Measurement_Result,
    /* 0x13 */ STRM_Measurement_Start,
    /* 0x14 */ STRM_SportsPackSelect,
    /* 0x15 */ RP_COMMON_SE_KEY_AB_pri_A_01_CTSP,
    /* 0x16 */ RP_COMMON_SE_KEY_AB_pri_A_01_TVSP,
    /* 0x17 */ RP_COMMON_SE_KEY_AB_pri_A_01,
    /* 0x18 */ RP_COMMON_SE_KEY_AB_pri_B_01_CTSP,
    /* 0x19 */ RP_COMMON_SE_KEY_AB_pri_B_01_TVSP,
    /* 0x1A */ RP_COMMON_SE_KEY_AB_pri_B_01,
    /* 0x1B */ RP_COMMON_SE_KEY_AB_01_CTSP,
    /* 0x1C */ RP_COMMON_SE_KEY_AB_01_TVSP,
    /* 0x1D */ RP_COMMON_SE_KEY_AB_01,
    /* 0x1E */ RP_COMMON_SE_KEY_A_01_CTSP,
    /* 0x1F */ RP_COMMON_SE_KEY_A_01_TVSP,
    /* 0x20 */ RP_COMMON_SE_KEY_A_01,
    /* 0x21 */ RP_COMMON_SE_KEY_A_02_CTSP,
    /* 0x22 */ RP_COMMON_SE_KEY_A_02_TVSP,
    /* 0x23 */ RP_COMMON_SE_KEY_A_02,
    /* 0x24 */ RP_COMMON_SE_KEY_A_03_CTSP,
    /* 0x25 */ RP_COMMON_SE_KEY_A_03_TVSP,
    /* 0x26 */ RP_COMMON_SE_KEY_A_03,
    /* 0x27 */ RP_COMMON_SE_KEY_NG_01_CTSP,
    /* 0x28 */ RP_COMMON_SE_KEY_NG_01_TVSP,
    /* 0x29 */ RP_COMMON_SE_KEY_NG_01,
    /* 0x2A */ RP_COMMON_SE_KEY_BACK_01_CTSP,
    /* 0x2B */ RP_COMMON_SE_KEY_BACK_01_TVSP,
    /* 0x2C */ RP_COMMON_SE_KEY_BACK_01,
    /* 0x2D */ RP_COMMON_SE_KEY_CURSOR_01,
    /* 0x2E */ RP_COMMON_SE_KEY_CURSOR_02,
    /* 0x2F */ RP_COMMON_SE_KEY_CURSOR_03,
    /* 0x30 */ RP_COMMON_SE_KEY_CURSOR_04,
    /* 0x31 */ RP_COMMON_SE_KEY_CURSOR_05,
    /* 0x32 */ RP_COMMON_SE_CON_Change_01_CTSP,
    /* 0x33 */ RP_COMMON_SE_CON_Change_01_TVSP,
    /* 0x34 */ RP_COMMON_SE_CON_Change_01,
    /* 0x35 */ RP_COMMON_SE_CON_Change_02,
    /* 0x36 */ RP_COMMON_SE_KEY_NG_Decide_CTSP,
    /* 0x37 */ RP_COMMON_SE_KEY_NG_Decide_TVSP,
    /* 0x38 */ RP_COMMON_SE_TR_TrainingPanelOpen,
    /* 0x39 */ SE_TR_COMMON_Push_AB_CTSP,
    /* 0x3A */ SE_TR_COMMON_Push_AB_TVSP,
    /* 0x3B */ RP_COMMON_SE_TR_StartMeasureAge,
    /* 0x3C */ RP_COMMON_SE_TR_MeasureAgeCount,
    /* 0x3D */ RP_COMMON_SE_TR_StartMeasureAgeLong,
    /* 0x3E */ RP_COMMON_SE_TR_AgeDisp,
    /* 0x3F */ RP_COMMON_SE_TR_AgeKatamuku,
    /* 0x40 */ RP_COMMON_SE_TR_AgeStop,
    /* 0x41 */ RP_COMMON_SE_TR_AgeSmoke,
    /* 0x42 */ RP_COMMON_SE_TR_AgePush,
    /* 0x43 */ RP_COMMON_SE_TR_Decide,
    /* 0x44 */ RP_COMMON_SE_TR_Decide_CTSP,
    /* 0x45 */ RP_COMMON_SE_TR_Decide_TVSP,
    /* 0x46 */ RP_COMMON_SE_MESS_10,
    /* 0x47 */ RP_COMMON_SE_MESS_20,
    /* 0x48 */ RP_COMMON_SE_KEY_A_04_CTSP,
    /* 0x49 */ RP_COMMON_SE_KEY_A_04_TVSP,
    /* 0x4A */ RP_COMMON_SE_KEY_A_04,
    /* 0x4B */ RP_COM_GAME_SE_PAUSE_KEY_A_01_CTSP,
    /* 0x4C */ RP_COM_GAME_SE_PAUSE_KEY_A_01_TVSP,
    /* 0x4D */ RP_COM_GAME_SE_PAUSE_KEY_A_01
};

} // namespace rp_Common_Sports

#endif
