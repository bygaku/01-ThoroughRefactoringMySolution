#pragma once

class System final
{
public:
    System()          noexcept;
    ~System()         noexcept;

    /// @brief ƒ‰ƒCƒuƒ‰ƒŠ‰Šú‰»ˆ—
    /// @remark ˜M‚ç‚È‚¢‚Å
    bool Initialize() noexcept;

private:
    static bool instantiated_;

};