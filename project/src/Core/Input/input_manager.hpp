#pragma once
#include "array"
#include "memory"
#include "unordered_map"
#include "string"
#include "optional"
#include "Data/input_data.hpp"
#include "Utility/Math/vector2d.hpp"

class InputManager 
{
public:
    static InputManager& getInstance() noexcept;
    static void          create()	   noexcept;
    static void          destroy()	   noexcept;

    void update() noexcept;
    void debugView(int x = 10, int y = 10) const noexcept;

    /// @remark キーボード関連
    [[nodiscard]] InputState getKeyState  (KeyCode key) const noexcept;
    [[nodiscard]] bool       isKeyPressed (KeyCode key) const noexcept;
    [[nodiscard]] bool       isKeyHeld    (KeyCode key) const noexcept;
    [[nodiscard]] bool       isKeyReleased(KeyCode key) const noexcept;

    /// @remark マウス関連
    [[nodiscard]] InputState          getMouseButtonState  (MouseButton button) const noexcept;
    [[nodiscard]] bool                isMouseButtonPressed (MouseButton button) const noexcept;
    [[nodiscard]] bool                isMouseButtonHeld    (MouseButton button) const noexcept;
    [[nodiscard]] bool                isMouseButtonReleased(MouseButton button) const noexcept;
    [[nodiscard]] my_math::Vector2D   getMousePosition()                        const noexcept;
    [[nodiscard]] my_math::Vector2D   getMouseDelta()                           const noexcept;     /// @brief 前フレームからの向きを取得
    [[nodiscard]] my_math::Vector2D   getMouseDeltaFromCenter()                 const noexcept;     /// @brief 画面中心座標からの向きを取得
    [[nodiscard]] int                 getMouseWheelDelta()                      const noexcept;     
    void                              resetMousePoint()                         const noexcept;     /// @brief マウス座標を毎フレーム画面中心に戻す

    /// @remark ゲームパッド関連
    [[nodiscard]] bool       isPadConnected     (int playerIndex = 0)                   const noexcept;
    //[[nodiscard]] PadState   getPad             (int playerIndex = 0)                   const noexcept;
    [[nodiscard]] InputState getPadButtonState  (PadButton button, int playerIndex = 0) const noexcept;
    [[nodiscard]] bool       isPadButtonPressed (PadButton button, int playerIndex = 0) const noexcept;
    [[nodiscard]] bool       isPadButtonHeld    (PadButton button, int playerIndex = 0) const noexcept;
    [[nodiscard]] bool       isPadButtonReleased(PadButton button, int playerIndex = 0) const noexcept;

    /// @remark スティック入力（-1.0f～1.0fの範囲）
    [[nodiscard]] float             getPadStickValue(PadStick stick, int playerIndex = 0) const noexcept;
    [[nodiscard]] my_math::Vector2D getPadLStick(int playerIndex = 0)                     const noexcept;
    [[nodiscard]] my_math::Vector2D getPadRStick(int playerIndex = 0)                     const noexcept;

    /// @remark トリガー入力（0.0f～1.0fの範囲）
    [[nodiscard]] float getPadLTrigger(int playerIndex = 0) const noexcept;
    [[nodiscard]] float getPadRTrigger(int playerIndex = 0) const noexcept;

    /// @remark バイブレーション制御
    void setPadVibration (int playerIndex, float leftMotor, float rightMotor, float duration = 0.0f) noexcept;
    void stopPadVibration(int playerIndex)                                                           noexcept;

private:
    static InputManager* instance_;

    InputManager();
    ~InputManager() = default;

    [[nodiscard]] InputState calculateInputState   (bool current, bool prev) const noexcept;
    [[nodiscard]] float      normalizeStickValue   (short value)             const noexcept;
    [[nodiscard]] float      normalizeTriggerValue (UCHAR value)             const noexcept;
    void                     updatePadVibration    (float deltaTime)               noexcept;

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
    std::array<bool, kKeyAll> currentKeyState_ {};
    std::array<bool, kKeyAll> prevKeyState_    {};

    /// @remark マウス関連
    std::array<bool, kMouseButtonAll> currentMouseState_{};
    std::array<bool, kMouseButtonAll> prevMouseState_   {};
    my_math::Vector2D                 mousePosition_    {};
    my_math::Vector2D                 prevMousePosition_{};
    int                               mouseWheelDelta_ = 0;

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