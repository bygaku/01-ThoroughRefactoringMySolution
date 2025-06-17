#include "DxLib.h"
#include <memory>
#include "Utility/utility_link.hpp"
#include "Core/Scene/stage.hpp"

//#include "Object/text.hpp"
//#include "Object/camera_follow.hpp"
//#include "Object/player.hpp"

StageScene::StageScene(SceneManager& manager) noexcept
	: Scene(SceneType.kStage, manager)
{
	//auto follow_camera = std::make_shared<FollowCamera>();
	//auto player		   = std::make_shared<Player>(10);

	//if (follow_camera)
	//{
	//	// カメラの親オブジェクトとしてPlayerを追加
	//	follow_camera->getTransform()->setParent(player->getTransform());
	//	follow_camera->setOffset(0.0f, 50.0f, 50.0f);

	//	registerSceneObject(follow_camera, ObjectTag.CAMERA, "follow");
	//}

	//if (player) {
	//	registerSceneObject(player, ObjectTag.PLAYABLE, "player");
	//}
};

void StageScene::CheckSceneState() noexcept
{
	if (CheckHitKey(KEY_INPUT_R))
	{
		ChangeScene(SceneType.kResult);
	}

	if (CheckHitKey(KEY_INPUT_T))
	{
		ChangeScene(SceneType.kTitle);
	}
}
