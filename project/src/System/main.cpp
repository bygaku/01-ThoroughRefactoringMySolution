#include "DxLib.h"
#include "System/system.hpp"
#include "Utility/utility_link.hpp"
#include "Core/Input/input_manager.hpp"
#include "Core/GameManager/game_manager.hpp"

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) 
{
    /// @remark DISABLE LOG FILES OUTPUT
    SetOutApplicationLogValidFlag(false);

    /// @remark DXLIB BUILD
    auto system = System();
    if (!system.Initialize()) {
        return -1;
    }

    /// @remark ADX2 BUILD
    /// TODO: ADX2ÇÃèâä˙âªÇ∆ê⁄ë±

    /// @remark MAIN
    InputManager::Create();
    auto game_manager = std::make_unique<GameManager>();

    while (game_manager->IsRunning()) {
        clsDx();
        game_manager->Update();
    }

    InputManager::Destroy();

    return 0;
}