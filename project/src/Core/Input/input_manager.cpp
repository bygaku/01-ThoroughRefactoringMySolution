#include "DxLib.h"
#include <algorithm>
#include <cmath>
#include "Utility/utility_link.hpp"
#include "Core/Input/input_manager.hpp"

using namespace my_math;

InputManager* InputManager::instance_ = nullptr;

InputManager& InputManager::GetInstance() noexcept
{
    ASSERT(instance_ != nullptr, "instance_ isn't existing.");
    return *instance_;
}

void InputManager::Create() noexcept
{
    ASSERT(instance_ == nullptr, "instance_ already exist.");
    ASSERT(DxLib_IsInit() != 0,  "DxLib isn't initialize." );
    instance_ = new InputManager();
}

void InputManager::Destroy() noexcept
{
    ASSERT(instance_ != nullptr, "instance_ is nullptr.");

    // 全てのバイブレーションを停止
    for (int i = 0; i < kMaxGamepads; ++i) {
        instance_->StopPadVibration(i);
    }

    delete instance_;
}

void InputManager::Update() noexcept
{
    ASSERT(instance_ != nullptr, "this->create(); hasn't been called yet.");

    prev_mouse_position_  =     mouse_position_;
    prev_key_state_       =   current_key_state_;
    prev_mouse_state_     = current_mouse_state_;

    // キーボード状態更新
    char keyState[256];
    GetHitKeyStateAll(keyState);

    for (int i = 0; i < kKeyAll; ++i){
        current_key_state_[i] = (keyState[i] != 0);
    }

    // マウス状態更新
    int mouseInput = GetMouseInput();
    current_mouse_state_[static_cast<int>(MouseButton::L)]   = (mouseInput & MOUSE_INPUT_LEFT)   != 0;
    current_mouse_state_[static_cast<int>(MouseButton::R)]   = (mouseInput & MOUSE_INPUT_RIGHT)  != 0;
    current_mouse_state_[static_cast<int>(MouseButton::Mid)] = (mouseInput & MOUSE_INPUT_MIDDLE) != 0;

    // マウス座標更新
    int mouseX, mouseY;
    GetMousePoint(&mouseX, &mouseY);
    mouse_position_ = Vector2D(static_cast<float>(mouseX), static_cast<float>(mouseY));

    // マウスホイール更新
    mouse_wheel_delta_ = GetMouseWheelRotVol();

    // ゲームパッド状態更新
    for (int i = 0; i < kMaxGamepads; ++i) {
        gamepads_[i].prevState_ = gamepads_[i].currentState;

        DWORD result            = GetJoypadXInputState(i, &gamepads_[i].currentState);
        gamepads_[i].connected_ = (result == ERROR_SUCCESS);
    }

    /// HACK: バイブレーションの更新もここですべき.
}

#pragma region キーボードアクセサ
InputState InputManager::GetKeyState(KeyCode key) const noexcept
{
    int keyIndex = static_cast<int>(key);
    return CalculateInputState(current_key_state_[keyIndex], prev_key_state_[keyIndex]);
}

bool InputManager::IsKeyPressed(KeyCode key) const noexcept
{
    return GetKeyState(key) == InputState::kPressed;
}

bool InputManager::IsKeyHeld(KeyCode key) const noexcept
{
    return GetKeyState(key) == InputState::kHeld;
}

bool InputManager::IsKeyReleased(KeyCode key) const noexcept
{
    return GetKeyState(key) == InputState::kReleased;
}
#pragma endregion

#pragma region マウスアクセサ
InputState InputManager::GetMouseButtonState(MouseButton button) const noexcept
{
    int buttonIndex = static_cast<int>(button);
    return CalculateInputState(current_mouse_state_[buttonIndex], prev_mouse_state_[buttonIndex]);
}

bool InputManager::IsMouseButtonPressed(MouseButton button) const noexcept
{
    return GetMouseButtonState(button) == InputState::kPressed;
}

bool InputManager::IsMouseButtonHeld(MouseButton button) const noexcept
{
    return GetMouseButtonState(button) == InputState::kHeld;
}

bool InputManager::IsMouseButtonReleased(MouseButton button) const noexcept
{
    return GetMouseButtonState(button) == InputState::kReleased;
}

Vector2D InputManager::GetMousePosition() const noexcept
{
    return mouse_position_;
}

Vector2D InputManager::GetMouseDelta() const noexcept
{
    return mouse_position_ - prev_mouse_position_;
}

Vector2D InputManager::GetMouseDeltaFromCenter() const noexcept
{
    my_math::Vector2D velocity;
    velocity.x_ = mouse_position_.x_ - screen_data::kHalfWidth;
    velocity.y_ = mouse_position_.y_ - screen_data::kHalfHeight;
    return velocity;
}

int InputManager::GetMouseWheelDelta() const noexcept
{
    return mouse_wheel_delta_;
}

void InputManager::ResetMousePoint() const noexcept
{
    SetMousePoint(screen_data::kHalfWidth, screen_data::kHalfHeight);
}
#pragma endregion

#pragma region ゲームパッドアクセサ
bool InputManager::IsPadConnected(int playerIndex) const noexcept
{
    if (playerIndex < 0 || playerIndex >= kMaxGamepads) {
        return false;
    }
    
    return gamepads_[playerIndex].connected_;
}

InputState InputManager::GetPadButtonState(PadButton button, int playerIndex) const noexcept
{
    if (playerIndex < 0 || playerIndex >= kMaxGamepads || !gamepads_[playerIndex].connected_) {
        return InputState::kNone;
    }

    WORD buttonFlag = static_cast<WORD>(button);
    bool current    = (gamepads_[playerIndex].currentState.Buttons[static_cast<int>(button)] & buttonFlag) != 0;
    bool prev       = (gamepads_[playerIndex].prevState_.Buttons  [static_cast<int>(button)] & buttonFlag) != 0;

    return CalculateInputState(current, prev);
}

bool InputManager::IsPadButtonPressed(PadButton button, int playerIndex) const noexcept
{
    return GetPadButtonState(button, playerIndex) == InputState::kPressed;
}

bool InputManager::IsPadButtonHeld(PadButton button, int playerIndex) const noexcept
{
    return GetPadButtonState(button, playerIndex) == InputState::kHeld;
}

bool InputManager::IsPadButtonReleased(PadButton button, int playerIndex) const noexcept
{
    return GetPadButtonState(button, playerIndex) == InputState::kReleased;
}

float InputManager::GetPadStickValue(PadStick stick, int playerIndex) const noexcept
{
    if (playerIndex < 0 || playerIndex >= kMaxGamepads || !gamepads_[playerIndex].connected_) {
        return 0.0f;
    }

    const auto& gamepad = gamepads_[playerIndex].currentState;

    switch (stick) 
    {
    case PadStick::LX:
        return NormalizeStickValue(gamepad.ThumbLX);
    case PadStick::LY:
        return NormalizeStickValue(gamepad.ThumbLY);
    case PadStick::RX:
        return NormalizeStickValue(gamepad.ThumbRX);
    case PadStick::RY:
        return NormalizeStickValue(gamepad.ThumbRY);
    default:
        return 0.0f;
    }
}

Vector2D InputManager::GetPadLStick(int playerIndex) const noexcept
{
    return Vector2D(
        GetPadStickValue(PadStick::LX, playerIndex),
        GetPadStickValue(PadStick::LY, playerIndex)
    );
}

Vector2D InputManager::GetPadRStick(int playerIndex) const noexcept
{
    return Vector2D(
        GetPadStickValue(PadStick::RX, playerIndex),
        GetPadStickValue(PadStick::RY, playerIndex)
    );
}

float InputManager::GetPadLTrigger(int playerIndex) const noexcept
{
    if (playerIndex < 0 || playerIndex >= kMaxGamepads || !gamepads_[playerIndex].connected_) {
        return 0.0f;
    }

    return NormalizeTriggerValue(gamepads_[playerIndex].currentState.LeftTrigger);
}

float InputManager::GetPadRTrigger(int playerIndex) const noexcept
{
    if (playerIndex < 0 || playerIndex >= kMaxGamepads || !gamepads_[playerIndex].connected_) {
        return 0.0f;
    }

    return NormalizeTriggerValue(gamepads_[playerIndex].currentState.RightTrigger);
}

void InputManager::SetPadVibration(int playerIndex, float leftMotor, float rightMotor, float duration) noexcept
{
    if (playerIndex < 0 || playerIndex >= kMaxGamepads || !gamepads_[playerIndex].connected_) return;

    /// HACK: 振動設定
}

void InputManager::StopPadVibration(int playerIndex) noexcept
{
    if (playerIndex < 0 || playerIndex >= kMaxGamepads) return;

    /// HACK: 振動停止処理
}
#pragma endregion

void InputManager::Debug(int x, int y) const noexcept
{
    printfDx("\n\n");
    int drawY            =  y;
    const int lineHeight = 20;

    // マウス情報
    printfDx("\nMouse: pos(%.1f, %.1f) Wheel(%d)",
             mouse_position_.x_, mouse_position_.y_, mouse_wheel_delta_);

    drawY += lineHeight;

    // ゲームパッド情報
    for (int i = 0; i < kMaxGamepads; ++i)
    {
        if (gamepads_[i].connected_) 
        {
            Vector2D leftStick  = GetPadLStick(i);
            Vector2D rightStick = GetPadRStick(i);
            float leftTrigger   = GetPadLTrigger(i);
            float rightTrigger  = GetPadRTrigger(i);

            printfDx("\nPad[%d]: L(%.1f,%.1f) R(%.1f,%.1f) LT:(%.1f) RT:(%.1f)",
                             i + 1, leftStick.x_, leftStick.y_, rightStick.x_, rightStick.y_,
                             leftTrigger, rightTrigger);

            drawY += lineHeight;
        }
    }
}

#pragma region Privateメソッド
InputManager::InputManager()
{
    SetJoypadDeadZone(DX_INPUT_PAD1, kStickDeadZone * 1000);
    SetJoypadDeadZone(DX_INPUT_PAD2, kStickDeadZone * 1000);
    SetJoypadDeadZone(DX_INPUT_PAD3, kStickDeadZone * 1000);
    SetJoypadDeadZone(DX_INPUT_PAD4, kStickDeadZone * 1000);

    SetMousePoint(screen_data::kHalfWidth, screen_data::kHalfHeight);
}

InputState InputManager::CalculateInputState(bool current, bool prev) const noexcept
{
    if (current && prev)
    {
        return InputState::kHeld;
    }
    else if (current && !prev)
    {
        return InputState::kPressed;
    }
    else if (!current && prev) 
    {
        return InputState::kReleased;
    }
    else {
        return InputState::kNone;
    }
}

float InputManager::NormalizeStickValue(short value) const noexcept
{
    float normalized = static_cast<float>(value) / 32767.0f;

    // デッドゾーン適用
    if (std::abs(normalized) < kStickDeadZone)
    {
        normalized = 0.0f;
    }
    else
    {
        // デッドゾーン外の値を0-1の範囲に再マッピング
        float sign = (normalized >= 0.0f) ? 1.0f : -1.0f;
        normalized = (std::abs(normalized) - 
                      static_cast<float>(kStickDeadZone)) / (1.0f - static_cast<float>(kStickDeadZone))
                      * sign;
    }

    return std::clamp(normalized, -1.0f, 1.0f);
}

float InputManager::NormalizeTriggerValue(UCHAR value) const noexcept
{
    return static_cast<float>(value) / 255.0f;
}

void InputManager::UpdatePadVibration(float deltaTime) noexcept
{
    for (int i = 0; i < kMaxGamepads; ++i) 
    {
        auto& gamepad = gamepads_[i];

        if (gamepad.vibrationDuration_ > 0.0f) 
        {
            gamepad.vibrationDuration_ -= deltaTime;

            if (gamepad.vibrationDuration_ <= 0.0f)
            {
                StopPadVibration(i);
            }
        }
    }
}
#pragma endregion