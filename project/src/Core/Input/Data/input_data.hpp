#pragma once
#include "DxLib.h"

/// @brief キーボード
enum class KeyCode
{
    A       = KEY_INPUT_A,
    B       = KEY_INPUT_B,
    C       = KEY_INPUT_C,
    D       = KEY_INPUT_D,
    E       = KEY_INPUT_E,
    F       = KEY_INPUT_F,
    G       = KEY_INPUT_G,
    H       = KEY_INPUT_H,
    I       = KEY_INPUT_I,
    J       = KEY_INPUT_J,
    K       = KEY_INPUT_K,
    L       = KEY_INPUT_L,
    M       = KEY_INPUT_M,
    N       = KEY_INPUT_N,
    O       = KEY_INPUT_O,
    P       = KEY_INPUT_P,
    Q       = KEY_INPUT_Q,
    R       = KEY_INPUT_R,
    S       = KEY_INPUT_S,
    T       = KEY_INPUT_T,
    U       = KEY_INPUT_U,
    V       = KEY_INPUT_V,
    W       = KEY_INPUT_W,
    X       = KEY_INPUT_X,
    Y       = KEY_INPUT_Y,
    Z       = KEY_INPUT_Z,

    Num0    = KEY_INPUT_0,
    Num1    = KEY_INPUT_1,
    Num2    = KEY_INPUT_2,
    Num3    = KEY_INPUT_3,
    Num4    = KEY_INPUT_4,
    Num5    = KEY_INPUT_5,
    Num6    = KEY_INPUT_6,
    Num7    = KEY_INPUT_7,
    Num8    = KEY_INPUT_8,
    Num9    = KEY_INPUT_9,

    Space   = KEY_INPUT_SPACE,
    Enter   = KEY_INPUT_RETURN,
    Escape  = KEY_INPUT_ESCAPE,
    Delete  = KEY_INPUT_DELETE,
    Tab     = KEY_INPUT_TAB,
    LShift  = KEY_INPUT_LSHIFT,
    RShift  = KEY_INPUT_RSHIFT,
    LCtrl   = KEY_INPUT_LCONTROL,
    RCtrl   = KEY_INPUT_RCONTROL,
    LAlt    = KEY_INPUT_LALT,
    RAlt    = KEY_INPUT_RALT,

    Up      = KEY_INPUT_UP,
    Down    = KEY_INPUT_DOWN,
    Left    = KEY_INPUT_LEFT,
    Right   = KEY_INPUT_RIGHT
};

/// @brief マウスボタン
enum class MouseButton
{
    L       = 0,
    R       = 1,
    Mid     = 2
};

/// @brief ゲームパッドボタン
enum class PadButton
{
    A           = XINPUT_BUTTON_A,
    B           = XINPUT_BUTTON_B,
    X           = XINPUT_BUTTON_X,
    Y           = XINPUT_BUTTON_Y,
    LShoulder   = XINPUT_BUTTON_LEFT_SHOULDER,
    RShoulder   = XINPUT_BUTTON_RIGHT_SHOULDER,
    Back        = XINPUT_BUTTON_BACK,
    Start       = XINPUT_BUTTON_START,
    LStickThumb = XINPUT_BUTTON_LEFT_THUMB,
    RStickThumb = XINPUT_BUTTON_RIGHT_THUMB,
    DPadUp      = XINPUT_BUTTON_DPAD_UP,
    DPadDown    = XINPUT_BUTTON_DPAD_DOWN,
    DPadLeft    = XINPUT_BUTTON_DPAD_LEFT,
    DPadRight   = XINPUT_BUTTON_DPAD_RIGHT
};

/// @brief ゲームパッドスティック
enum class PadStick
{
    LX,
    LY,
    RX,
    RY
};

/// @brief 入力状態
enum class InputState
{
    kNone,       /// @brief 押されていない
    kSingle,
    kPressed,    /// @brief 今フレーム押された
    kHeld,       /// @brief 押し続けている
    kReleased    /// @brief 今フレーム離された
};