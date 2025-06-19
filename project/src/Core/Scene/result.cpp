#include "DxLib.h"
#include <memory>
#include "Utility/utility_link.hpp"
#include "Core/Scene/result.hpp"

//#include "Object/text.hpp"
//#include "Object/camera.hpp"

ResultScene::ResultScene(SceneManager& manager) noexcept
	: Scene(SceneType.kResult, manager)
{
}

void ResultScene::CheckSceneState() noexcept
{
	if (CheckHitKey(KEY_INPUT_S))
	{
		ChangeScene(SceneType.kStage);
	}
}
