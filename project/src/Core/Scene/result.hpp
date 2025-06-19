#pragma once
#include "Core/Scene/scene.hpp"

class ResultScene final : public Scene
{
public:
	ResultScene(SceneManager& manager) noexcept;

	/// @brief シーンの状態を確認
	void CheckSceneState() noexcept override;
};
