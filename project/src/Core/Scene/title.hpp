#pragma once
#include "Core/Scene/scene.hpp"

class TitleScene final : public Scene
{
public:
	TitleScene(SceneManager& manager) noexcept;

	/// @brief �V�[���̏�Ԃ��m�F
	void CheckSceneState() noexcept override;
};
