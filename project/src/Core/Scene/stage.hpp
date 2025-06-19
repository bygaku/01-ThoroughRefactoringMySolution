#pragma once
#include "Core/Scene/scene.hpp"

class StageScene final : public Scene
{
public:
	StageScene(SceneManager& manager) noexcept;

	/// @brief シーンの状態を確認
	void CheckSceneState() noexcept override;
};
