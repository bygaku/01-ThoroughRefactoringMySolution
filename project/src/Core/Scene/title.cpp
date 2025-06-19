#include "DxLib.h"
#include <memory>
#include "Utility/utility_link.hpp"
#include "Core/Scene/title.hpp"

//#include "Object/text.hpp"
//#include "Object/camera.hpp"

TitleScene::TitleScene(SceneManager& manager) noexcept
	: Scene(SceneType.kTitle, manager)
{
	//auto main_camera = std::make_shared<Camera>();

	//if (main_camera) {
	//	VECTOR look_at	  = VGet(50.0f, 25.0f, 0.0f);
	//	VECTOR camera_pos = VGet(50.0f, 25.0f, -60.0f);

	//	main_camera->lookAtPosition(look_at);
	//	main_camera->getTransform()->setWorldPosition(camera_pos);

	//	registerSceneObject(main_camera, ObjectTag.CAMERA, "mainCamera");
	//}

	//auto text = std::make_shared<Text>(30);

	//if (text) {
	//	text->setText("TITLE");
	//	text->setTextCenter();

	//	registerSceneObject(text, ObjectTag.TEXT, "TitleText");
	//}
}

void TitleScene::CheckSceneState() noexcept
{
	if (CheckHitKey(KEY_INPUT_S))
	{
		ChangeScene(SceneType.kStage);
	}
}
