#pragma once
#include "Core/Scene/scene.hpp"

class StageScene final : public Scene
{
public:
	StageScene(SceneManager& manager) noexcept;

	/// @brief �V�[���̏�Ԃ��m�F
	void CheckSceneState() noexcept override;
};
