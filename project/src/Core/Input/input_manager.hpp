#pragma once
#include <array>
#include <memory>
#include <unordered_map>
#include <string>
#include <optional>
#include "Core/Input/Data/input_data.hpp"
#include "Utility/Math/math.hpp"

class InputManager 
{
public:
    static InputManager& GetInstance() noexcept;
    static void          Create()	   noexcept;
    static void          Destroy()	   noexcept;

    void Update() noexcept;
    void Debug(int x = 10, int y = 10) const noexcept;

    /// @remark キーボード関連
    [[nodiscard]] InputState GetKeyState  (KeyCode key) const noexcept;
    [[nodiscard]] bool       IsKeyPressed (KeyCode key) const noexcept;
    [[nodiscard]] bool       IsKeyHeld    (KeyCode key) const noexcept;
    [[nodiscard]] bool       IsKeyReleased(KeyCode key) const noexcept;

    /// @remark マウス関連
    [[nodiscard]] InputState          GetMouseButtonState  (MouseButton button) const noexcept;
    [[nodiscard]] bool                IsMouseButtonPressed (MouseButton button) const noexcept;
    [[nodiscard]] bool                IsMouseButtonHeld    (MouseButton button) const noexcept;
    [[nodiscard]] bool                IsMouseButtonReleased(MouseButton button) const noexcept;
    [[nodiscard]] my_math::Vector2D   GetMousePosition()                        const noexcept;
    [[nodiscard]] my_math::Vector2D   GetMouseDelta()                           const noexcept;     /// @brief 前フレームからの向きを取得
    [[nodiscard]] my_math::Vector2D   GetMouseDeltaFromCenter()                 const noexcept;     /// @brief 画面中心座標からの向きを取得
    [[nodiscard]] int                 GetMouseWheelDelta()                      const noexcept;     
    void                              ResetMousePoint()                         const noexcept;     /// @brief マウス座標を毎フレーム画面中心に戻す

    /// @remark ゲームパッド関連
    [[nodiscard]] bool       IsPadConnected     (int playerIndex = 0)                   const noexcept;
    [[nodiscard]] InputState GetPadButtonState  (PadButton button, int playerIndex = 0) const noexcept;
    [[nodiscard]] bool       IsPadButtonPressed (PadButton button, int playerIndex = 0) const noexcept;
    [[nodiscard]] bool       IsPadButtonHeld    (PadButton button, int playerIndex = 0) const noexcept;
    [[nodiscard]] bool       IsPadButtonReleased(PadButton button, int playerIndex = 0) const noexcept;

    /// @remark スティック入力（-1.0f～1.0fの範囲）
    [[nodiscard]] float             GetPadStickValue(PadStick stick, int playerIndex = 0) const noexcept;
    [[nodiscard]] my_math::Vector2D GetPadLStick(int playerIndex = 0)                     const noexcept;
    [[nodiscard]] my_math::Vector2D GetPadRStick(int playerIndex = 0)                     const noexcept;

    /// @remark トリガー入力（0.0f～1.0fの範囲）
    [[nodiscard]] float GetPadLTrigger(int playerIndex = 0) const noexcept;
    [[nodiscard]] float GetPadRTrigger(int playerIndex = 0) const noexcept;

    /// @remark バイブレーション制御
    void SetPadVibration (int playerIndex, float leftMotor, float rightMotor, float duration = 0.0f) noexcept;
    void StopPadVibration(int playerIndex)                                                           noexcept;

private:
    static InputManager* instance_;

    InputManager();
    ~InputManager() = default;

    [[nodiscard]] InputState CalculateInputState   (bool current, bool prev) const noexcept;
    [[nodiscard]] float      NormalizeStickValue   (short value)             const noexcept;
    [[nodiscard]] float      NormalizeTriggerValue (UCHAR value)             const noexcept;
    void                     UpdatePadVibration    (float deltaTime)               noexcept;

private:
    /// @remark 禁止
    InputManager(const InputManager&)            = delete;
    InputManager& operator=(const InputManager&) = delete;
    InputManager(InputManager&&)                 = delete;
    InputManager& operator=(InputManager&&)      = delete;

private:
    static constexpr int    kKeyAll              =    256;  /// @brief キーボード数
    static constexpr int    kMouseButtonAll      =      3;  /// @brief マウスボタン数
    static constexpr int    kMaxGamepads         =      4;  /// @brief 最大同時接続
    static constexpr double kStickDeadZone       =    0.2;  /// @brief デッドゾーン

    /// @remark キーボード関連
    std::array<bool, kKeyAll> current_key_state_ {};
    std::array<bool, kKeyAll> prev_key_state_    {};

    /// @remark マウス関連
    std::array<bool, kMouseButtonAll> current_mouse_state_{};
    std::array<bool, kMouseButtonAll> prev_mouse_state_   {};
    my_math::Vector2D                 mouse_position_     {};
    my_math::Vector2D                 prev_mouse_position_{};
    int                               mouse_wheel_delta_ = 0;

    /// @remark パッド関連 
    struct PadState
    {
        XINPUT_STATE     currentState            {};
        XINPUT_STATE     prevState_              {};
        bool             connected_         = false;
        float            vibrationDuration_ =  0.0f;
    };

    std::array<PadState, kMaxGamepads> gamepads_ {};

};