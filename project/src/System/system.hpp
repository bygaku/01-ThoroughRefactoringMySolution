#pragma once

class System final
{
public:
    System()          noexcept;
    ~System()         noexcept;

    /// @brief ライブラリ初期化処理
    /// @remark 弄らないで
    bool Initialize() noexcept;

private:
    static bool instantiated_;

};