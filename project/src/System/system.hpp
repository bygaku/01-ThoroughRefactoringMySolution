#pragma once

class System final
{
public:
    System()          noexcept;
    ~System()         noexcept;

    /// @brief ���C�u��������������
    /// @remark �M��Ȃ���
    bool Initialize() noexcept;

private:
    static bool instantiated_;

};