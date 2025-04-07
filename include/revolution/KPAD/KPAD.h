#ifndef RVL_SDK_KPAD_H
#define RVL_SDK_KPAD_H
#ifdef __cplusplus
#include <types.h>

#include <revolution/MTX.h>
extern "C" {
#endif

//! @addtogroup rvl_kpad
//! @{

typedef enum {
    KPAD_RESULT_OK = 0,
} KPADResult;

typedef enum {
    KPAD_CHAN_0,
    KPAD_CHAN_1,
    KPAD_CHAN_2,
    KPAD_CHAN_3,
    KPAD_MAX_CONTROLLERS
} KPADChan;

typedef enum {
    KPAD_BTN_DLEFT = (1 << 0),
    KPAD_BTN_DRIGHT = (1 << 1),
    KPAD_BTN_DDOWN = (1 << 2),
    KPAD_BTN_DUP = (1 << 3),

    KPAD_BTN_PLUS = (1 << 4),
    KPAD_BTN_MINUS = (1 << 12),

    KPAD_BTN_1 = (1 << 9),
    KPAD_BTN_2 = (1 << 8),

    KPAD_BTN_A = (1 << 11),
    KPAD_BTN_B = (1 << 10),

    KPAD_BTN_HOME = (1 << 15),
} KPADButton;

typedef struct Vec2 {
    f32 x, y;
} Vec2;

typedef union KPADExStatus {
    // Wii Remote ("free style")
    struct {
        Vec2 stick;    // at 0x0
        Vec acc;       // at 0x8
        f32 acc_value; // at 0x14
        f32 acc_speed; // at 0x18
    } fs;

    // Classic Controller
    struct {
        u32 hold;     // at 0x0
        u32 trig;     // at 0x4
        u32 release;  // at 0x8
        Vec2 lstick;  // at 0xC
        Vec2 rstick;  // at 0x14
        f32 ltrigger; // at 0x1C
        f32 rtrigger; // at 0x20
    } cl;

    // Balance Board
    struct {
        f64 tgc_weight;     // at 0x0
        f64 weight[4];      // at 0x8
        f64 weight_ave[4];  // at 0x28
        s32 weight_err;     // at 0x48
        s32 tgc_weight_err; // at 0x4C
    } bl;
} KPADExStatus;

typedef struct KPADStatus {
    u32 hold;               // at 0x0
    u32 trig;               // at 0x4
    u32 release;            // at 0x8
    Vec acc;                // at 0xC
    f32 acc_value;          // at 0x18
    f32 acc_speed;          // at 0x1C
    Vec2 pos;               // at 0x20
    Vec2 vec;               // at 0x28
    f32 speed;              // at 0x30
    Vec2 horizon;           // at 0x34
    Vec2 hori_vec;          // at 0x3C
    f32 hori_speed;         // at 0x44
    f32 dist;               // at 0x48
    f32 dist_vec;           // at 0x4C
    f32 dist_speed;         // at 0x50
    Vec2 acc_vertical;      // at 0x54
    u8 dev_type;            // at 0x5C
    s8 wpad_err;            // at 0x5D
    s8 dpd_valid_fg;        // at 0x5E
    u8 data_format;         // at 0x5F
    KPADExStatus ex_status; // at 0x60
} KPADStatus;

s32 KPADRead(s32 chan, KPADStatus* status, u32 num);

//! @}

#ifdef __cplusplus
}
#endif
#endif
