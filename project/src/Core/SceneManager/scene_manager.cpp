#include "DxLib.h"
#include "Utility/utility_link.hpp"

#include "Scene/scene.hpp"
#include "Scene/title.hpp"
#include "Scene/load.hpp"
#include "Scene/stage.hpp"
#include "Scene/result.hpp"
#include "Scene/shared.hpp"

#include "Scene/scene_manager.hpp"

bool SceneManager::instantiated_ = false;

SceneManager::SceneManager(GameManager& game) noexcept
	: game_(game)
{
	ASSERT(instantiated_ == 0, "already instantiated.");
	instantiated_ = true;

	scenes_[SceneType.TITLE]  = std::make_shared<TitleScene> (*this);
	scenes_[SceneType.LOAD]   = std::make_shared<LoadScene>  (*this);
	scenes_[SceneType.STAGE]  = std::make_shared<StageScene> (*this);
	scenes_[SceneType.RESULT] = std::make_shared<ResultScene>(*this);

	start_up_scene_ = scenes_[SceneType.kTitle];
	start_up_scene_->Initialize();

	current_scene_ = start_up_scene_;
}

SceneManager::~SceneManager() noexcept
{
	instantiated_ = false;
}

void SceneManager::Update() noexcept
{
	current_scene_->update();
}

void SceneManager::Draw() noexcept
{
	current_scene_->draw();
}

void SceneManager::Debug() noexcept
{
	auto length_half = 100.f;
	DrawLine3D(VGet(0, 0, 0), VGet(length_half, 0, 0), GetColor(255, 0, 0));
	DrawLine3D(VGet(0, 0, 0), VGet(0, length_half, 0), GetColor(0, 255, 0));
	DrawLine3D(VGet(0, 0, 0), VGet(0, 0, length_half), GetColor(0, 0, 255));

	current_scene_->debugView();
}

void SceneManager::LastUpdate() noexcept
{
	current_scene_->lastUpdate();
	current_scene_->checkSceneState();
}

void SceneManager::ChangeScene(std::string tag) noexcept
{
	ASSERT(tag != SceneType.LOAD, "LoadScene�𖾎��I�ɌĂяo�����Ƃ��Ă��܂�.");

	if (current_scene_ == scenes_[tag]) return;
	if (!scenes_.count(tag))		   return;

	// LOAD�V�[���ł�TaskComplete�ɂ��A���̃V�[���֑����ɐ؂�ւ�
	auto currentLoadScene = std::dynamic_pointer_cast<LoadScene>(current_scene_);
	if (currentLoadScene) 
	{
		changeSceneImmediate(tag);
		return;
	}

	// ���̃V�[���̏ꍇ�́ALoadScene���o�R���Ĕ񓯊��ǂݍ���
	auto loadScene = std::dynamic_pointer_cast<LoadScene>(scenes_[SceneType.LOAD]);
	if (loadScene) 
	{
		// �܂�Loading��ʂɑ����ɐ؂�ւ�
		changeSceneImmediate(SceneType.LOAD);

		// �񓯊��ŖړI�̃V�[���̓ǂݍ��݂��J�n
		loadScene->startAsyncLoad(tag);
	}
	else {
		// LoadScene�����p�ł��Ȃ��ꍇ�͏]���ʂ葦���ɐ؂�ւ�
		changeSceneImmediate(tag);
	}
}

void SceneManager::ChangeSceneImmediate(std::string tag) noexcept
{
	if (current_scene_ == scenes_[tag]) return;
	if (!scenes_.count(tag))		   return;

	// �V�[���ړ��O������
	scenes_[tag]->initialize();

	current_scene_ = scenes_[tag];
}

std::shared_ptr<Scene> SceneManager::GetScene(const std::string& tag) const noexcept
{
	if (!scenes_.count(tag)) return nullptr;
	return scenes_.at(tag);
}
