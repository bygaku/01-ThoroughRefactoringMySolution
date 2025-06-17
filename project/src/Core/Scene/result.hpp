#pragma once
#include "Core/Scene/scene.hpp"

class ResultScene final : public Scene
{
public:
	ResultScene(SceneManager& manager) noexcept
		: Scene(SceneType.kResult, manager)
	{
	};

	/// @brief シーンの状態を確認
	void CheckSceneState()	 noexcept override {};
};
