#pragma once
#include "Core/Scene/scene.hpp"

class TitleScene final : public Scene
{
public:
	TitleScene(SceneManager& manager) noexcept;

	/// @brief シーンの状態を確認
	void CheckSceneState() noexcept override;
};
