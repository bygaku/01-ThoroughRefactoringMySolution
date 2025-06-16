#include "DxLib.h"
#include "Data/window_data.hpp"
#include "Utility/assert_macro.hpp"
#include "Manager/input_manager.hpp"
#include "algorithm"
#include "cmath"

using namespace my_math;

InputManager* InputManager::instance_ = nullptr;

InputManager& InputManager::getInstance() noexcept
{
    ASSERT(instance_ != nullptr, "instance_ isn't existing.");
    return *instance_;
}

void InputManager::create() noexcept
{
    ASSERT(instance_ == nullptr, "instance_ already exist.");
    ASSERT(DxLib_IsInit() != 0,  "DxLib isn't initialize.");
    instance_ = new InputManager();
}

void InputManager::destroy() noexcept
{
    ASSERT(instance_ != nullptr, "instance_ is nullptr.");

    // �S�Ẵo�C�u���[�V�������~
    for (int i = 0; i < kMaxGamepads; ++i)
    {
        instance_->stopPadVibration(i);
    }

    delete instance_;
}

void InputManager::update() noexcept
{
    ASSERT(instance_ != nullptr, "this->create(); hasn't been called yet.");

    prevMousePosition_  =     mousePosition_;
    prevKeyState_       =   currentKeyState_;
    prevMouseState_     = currentMouseState_;

    // �L�[�{�[�h��ԍX�V
    char keyState[256];
    GetHitKeyStateAll(keyState);

    for (int i = 0; i < kKeyAll; ++i)
    {
        currentKeyState_[i] = (keyState[i] != 0);
    }

    // �}�E�X��ԍX�V
    int mouseInput = GetMouseInput();
    currentMouseState_[static_cast<int>(MouseButton::L)]   = (mouseInput & MOUSE_INPUT_LEFT)   != 0;
    currentMouseState_[static_cast<int>(MouseButton::R)]   = (mouseInput & MOUSE_INPUT_RIGHT)  != 0;
    currentMouseState_[static_cast<int>(MouseButton::Mid)] = (mouseInput & MOUSE_INPUT_MIDDLE) != 0;

    // �}�E�X���W�X�V
    int mouseX, mouseY;
    GetMousePoint(&mouseX, &mouseY);
    mousePosition_ = Vector2D(static_cast<float>(mouseX), static_cast<float>(mouseY));

    // �}�E�X�z�C�[���X�V
    mouseWheelDelta_ = GetMouseWheelRotVol();

    // �Q�[���p�b�h��ԍX�V
    for (int i = 0; i < kMaxGamepads; ++i)
    {
        gamepads_[i].prevState_ = gamepads_[i].currentState;

        DWORD result            = GetJoypadXInputState(i, &gamepads_[i].currentState);
        gamepads_[i].connected_ = (result == ERROR_SUCCESS);
    }

    /// HACK: �o�C�u���[�V�����̍X�V�������ł��ׂ�.
}

#pragma region �L�[�{�[�h�A�N�Z�T
InputState InputManager::getKeyState(KeyCode key) const noexcept
{
    int keyIndex = static_cast<int>(key);
    return calculateInputState(currentKeyState_[keyIndex], prevKeyState_[keyIndex]);
}

bool InputManager::isKeyPressed(KeyCode key) const noexcept
{
    return getKeyState(key) == InputState::kPressed;
}

bool InputManager::isKeyHeld(KeyCode key) const noexcept
{
    return getKeyState(key) == InputState::kHeld;
}

bool InputManager::isKeyReleased(KeyCode key) const noexcept
{
    return getKeyState(key) == InputState::kReleased;
}
#pragma endregion

#pragma region �}�E�X�A�N�Z�T
InputState InputManager::getMouseButtonState(MouseButton button) const noexcept
{
    int buttonIndex = static_cast<int>(button);
    return calculateInputState(currentMouseState_[buttonIndex], prevMouseState_[buttonIndex]);
}

bool InputManager::isMouseButtonPressed(MouseButton button) const noexcept
{
    return getMouseButtonState(button) == InputState::kPressed;
}

bool InputManager::isMouseButtonHeld(MouseButton button) const noexcept
{
    return getMouseButtonState(button) == InputState::kHeld;
}

bool InputManager::isMouseButtonReleased(MouseButton button) const noexcept
{
    return getMouseButtonState(button) == InputState::kReleased;
}

Vector2D InputManager::getMousePosition() const noexcept
{
    return mousePosition_;
}

Vector2D InputManager::getMouseDelta() const noexcept
{
    return mousePosition_ - prevMousePosition_;
}

Vector2D InputManager::getMouseDeltaFromCenter() const noexcept
{
    my_math::Vector2D velocity;
    velocity.x = mousePosition_.x - WindowData::kHalfWidth;
    velocity.y = mousePosition_.y - WindowData::kHalfHeight;
    return velocity;
}

int InputManager::getMouseWheelDelta() const noexcept
{
    return mouseWheelDelta_;
}

void InputManager::resetMousePoint() const noexcept
{
    SetMousePoint(WindowData::kHalfWidth, WindowData::kHalfHeight);
}
#pragma endregion

#pragma region �Q�[���p�b�h�A�N�Z�T
bool InputManager::isPadConnected(int playerIndex) const noexcept
{
    if (playerIndex < 0 || playerIndex >= kMaxGamepads) return false;
    return gamepads_[playerIndex].connected_;
}

InputState InputManager::getPadButtonState(PadButton button, int playerIndex) const noexcept
{
    if (playerIndex < 0 || playerIndex >= kMaxGamepads || !gamepads_[playerIndex].connected_)
    {
        return InputState::kNone;
    }

    WORD buttonFlag = static_cast<WORD>(button);
    bool current    = (gamepads_[playerIndex].currentState.Buttons[static_cast<int>(button)] & buttonFlag) != 0;
    bool prev       = (gamepads_[playerIndex].prevState_.Buttons  [static_cast<int>(button)] & buttonFlag) != 0;

    return calculateInputState(current, prev);
}

bool InputManager::isPadButtonPressed(PadButton button, int playerIndex) const noexcept
{
    return getPadButtonState(button, playerIndex) == InputState::kPressed;
}

bool InputManager::isPadButtonHeld(PadButton button, int playerIndex) const noexcept
{
    return getPadButtonState(button, playerIndex) == InputState::kHeld;
}

bool InputManager::isPadButtonReleased(PadButton button, int playerIndex) const noexcept
{
    return getPadButtonState(button, playerIndex) == InputState::kReleased;
}

float InputManager::getPadStickValue(PadStick stick, int playerIndex) const noexcept
{
    if (playerIndex < 0 || playerIndex >= kMaxGamepads || !gamepads_[playerIndex].connected_)
    {
        return 0.0f;
    }

    const auto& gamepad = gamepads_[playerIndex].currentState;

    switch (stick) 
    {
    case PadStick::LX:
        return normalizeStickValue(gamepad.ThumbLX);
    case PadStick::LY:
        return normalizeStickValue(gamepad.ThumbLY);
    case PadStick::RX:
        return normalizeStickValue(gamepad.ThumbRX);
    case PadStick::RY:
        return normalizeStickValue(gamepad.ThumbRY);
    default:
        return 0.0f;
    }
}

Vector2D InputManager::getPadLStick(int playerIndex) const noexcept
{
    return Vector2D(
        getPadStickValue(PadStick::LX, playerIndex),
        getPadStickValue(PadStick::LY, playerIndex)
    );
}

Vector2D InputManager::getPadRStick(int playerIndex) const noexcept
{
    return Vector2D(
        getPadStickValue(PadStick::RX, playerIndex),
        getPadStickValue(PadStick::RY, playerIndex)
    );
}

float InputManager::getPadLTrigger(int playerIndex) const noexcept
{
    if (playerIndex < 0 || playerIndex >= kMaxGamepads || !gamepads_[playerIndex].connected_) 
    {
        return 0.0f;
    }

    return normalizeTriggerValue(gamepads_[playerIndex].currentState.LeftTrigger);
}

float InputManager::getPadRTrigger(int playerIndex) const noexcept
{
    if (playerIndex < 0 || playerIndex >= kMaxGamepads || !gamepads_[playerIndex].connected_) 
    {
        return 0.0f;
    }

    return normalizeTriggerValue(gamepads_[playerIndex].currentState.RightTrigger);
}

void InputManager::setPadVibration(int playerIndex, float leftMotor, float rightMotor, float duration) noexcept
{
    if (playerIndex < 0 || playerIndex >= kMaxGamepads || !gamepads_[playerIndex].connected_) return;

    /// HACK: �U���ݒ�
}

void InputManager::stopPadVibration(int playerIndex) noexcept
{
    if (playerIndex < 0 || playerIndex >= kMaxGamepads) return;

    /// HACK: �U����~����
}
#pragma endregion

void InputManager::debugView(int x, int y) const noexcept
{
    printfDx("\n\n");
    int drawY            =  y;
    const int lineHeight = 20;

    // �}�E�X���
    printfDx("\nMouse: pos(%.1f, %.1f) Wheel(%d)",
             mousePosition_.x, mousePosition_.y, mouseWheelDelta_);

    drawY += lineHeight;

    // �Q�[���p�b�h���
    for (int i = 0; i < kMaxGamepads; ++i)
    {
        if (gamepads_[i].connected_) 
        {
            Vector2D leftStick  = getPadLStick(i);
            Vector2D rightStick = getPadRStick(i);
            float leftTrigger   = getPadLTrigger(i);
            float rightTrigger  = getPadRTrigger(i);

            printfDx("\nPad[%d]: L(%.1f,%.1f) R(%.1f,%.1f) LT:(%.1f) RT:(%.1f)",
                             i + 1, leftStick.x, leftStick.y, rightStick.x, rightStick.y,
                             leftTrigger, rightTrigger);

            drawY += lineHeight;
        }
    }
}

#pragma region Private���\�b�h
InputManager::InputManager()
{
    SetJoypadDeadZone(DX_INPUT_PAD1, kStickDeadZone * 1000);
    SetJoypadDeadZone(DX_INPUT_PAD2, kStickDeadZone * 1000);
    SetJoypadDeadZone(DX_INPUT_PAD3, kStickDeadZone * 1000);
    SetJoypadDeadZone(DX_INPUT_PAD4, kStickDeadZone * 1000);

    SetMousePoint(WindowData::kHalfWidth, WindowData::kHalfHeight);
}

InputState InputManager::calculateInputState(bool current, bool prev) const noexcept
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

float InputManager::normalizeStickValue(short value) const noexcept
{
    float normalized = static_cast<float>(value) / 32767.0f;

    // �f�b�h�]�[���K�p
    if (std::abs(normalized) < kStickDeadZone)
    {
        normalized = 0.0f;
    }
    else
    {
        // �f�b�h�]�[���O�̒l��0-1�͈̔͂ɍă}�b�s���O
        float sign = (normalized >= 0.0f) ? 1.0f : -1.0f;
        normalized = (std::abs(normalized) - 
                      static_cast<float>(kStickDeadZone)) / (1.0f - static_cast<float>(kStickDeadZone))
                      * sign;
    }

    return std::clamp(normalized, -1.0f, 1.0f);
}

float InputManager::normalizeTriggerValue(UCHAR value) const noexcept
{
    return static_cast<float>(value) / 255.0f;
}

void InputManager::updatePadVibration(float deltaTime) noexcept
{
    for (int i = 0; i < kMaxGamepads; ++i) 
    {
        auto& gamepad = gamepads_[i];

        if (gamepad.vibrationDuration_ > 0.0f) 
        {
            gamepad.vibrationDuration_ -= deltaTime;

            if (gamepad.vibrationDuration_ <= 0.0f)
            {
                stopPadVibration(i);
            }
        }
    }
}
#pragma endregion