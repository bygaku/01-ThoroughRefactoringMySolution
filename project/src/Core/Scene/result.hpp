#pragma once
#include "Core/Scene/scene.hpp"

class ResultScene final : public Scene
{
public:
	ResultScene(SceneManager& manager) noexcept;

	/// @brief �V�[���̏�Ԃ��m�F
	void CheckSceneState() noexcept override;
};
