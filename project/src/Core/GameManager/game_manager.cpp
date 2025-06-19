#include "DxLib.h"
#include "Utility/utility_link.hpp"
#include "Core/GameManager/game_manager.hpp"
#include "Core/GameTime/game_time.hpp"
#include "Core/SceneManager/scene_manager.hpp"
#include "Core/Input/input_manager.hpp"

bool GameManager::instantiated_ = false;

GameManager::GameManager() noexcept
    : game_time_	(std::make_shared<GameTime>())
    , scene_manager_(std::make_shared<SceneManager>(*this))
    , time_(0.0f)
{
    ASSERT(instantiated_ == 0, "already instantiated.");
    instantiated_ = true;
}

GameManager::~GameManager() noexcept
{
    DxLib_End();
    instantiated_ = false;
}

bool GameManager::IsRunning() const noexcept
{
    return !ProcessMessage() && !InputManager::GetInstance().IsPadButtonPressed(PadButton::Back, 1);
}

void GameManager::Update() noexcept
{
    auto& input = InputManager::GetInstance();

    ClearDrawScreen();
    UpdateBootTime();

    scene_manager_->Update();
    input.Update();

    DebugOnDisplay();
    
    scene_manager_->Draw();
    scene_manager_->LastUpdate();

    game_time_->SleepForNextFrame();
    ScreenFlip();

    game_time_->IncrementFrame();
}

void GameManager::DebugOnDisplay() const noexcept
{
    printfDx("Game Update!!\n");
    printfDx("GameTime: %.2f\n", time_);

    scene_manager_->Debug();

    auto& input = InputManager::GetInstance();
    input.Debug();
}

void GameManager::UpdateBootTime() noexcept
{
    time_ += GameTime::GetDeltaTime();
}
