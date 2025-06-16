#include "DxLib.h"
#include "Utility/utility_link.hpp"
#include "Core/GameTime/game_time.hpp"
#include "Core/SceneManager/scene_manager.hpp"
#include "Core/Scene/scene.hpp"
#include "Core/Input/input_manager.hpp"

void Scene::Initialize() noexcept
{
	time_ = 0.0f;

	for (auto&& obj : objects_) {
		obj.second->Initialize();
	}

	for (auto&& obj : objects_) {
		obj.second->Activate();
	}
}

void Scene::Update() noexcept
{
	time_ += GameTime::GetDeltaTime();

	for (auto&& obj : objects_) {
		if (!obj.second->IsActive()) {
			continue;
		}

		obj.second->Update();
	}
}

void Scene::Draw() const noexcept
{
	for (auto&& obj : objects_) {
		if (!obj.second->IsActive()) {
			continue;
		}

		obj.second->Draw();
	}
}

void Scene::Debug() const noexcept
{
#ifdef _DEBUG
	printfDx("%sTime: %.1f \n", type_.c_str(), time_);
	printfDx("\n");
	printfDx("パッド接続数：%d\n", GetJoypadNum());
	printfDx("\n");

	for (auto&& obj : objects_) {
		auto  name = obj.second->GetName();
		auto& pos  = obj.second->GetTransform()->GetWorldPosition();

		if (CheckHitKey(KEY_INPUT_TAB)) {
			DrawLine3D(pos, VAdd(pos, VScale(obj.second->GetTransform()->GetForward(), 5.0f)), color_data::B);
			DrawLine3D(pos, VAdd(pos, VScale(obj.second->GetTransform()->GetRight(),   5.0f)), color_data::R);
			DrawLine3D(pos, VAdd(pos, VScale(obj.second->GetTransform()->GetUp(),	   5.0f)), color_data::G);
		}

		printfDx("%s... x: %.2f, y: %.2f, z: %.2f. \n", name.c_str(), pos.x, pos.y, pos.z);
	}
#endif // DEBUG
}

void Scene::LastUpdate() noexcept
{
	for (auto&& obj : objects_) {
		obj.second->LastUpdate();
	}
}

void Scene::CheckSceneState() noexcept
{
	/// no processing on base class.
}

void Scene::RegisterSceneObject(std::shared_ptr<GameObject> object, const std::string& tag) noexcept
{
	objects_[tag] = object;
}

std::shared_ptr<GameObject> Scene::GetSceneObject(const std::string& tag) noexcept
{
	return std::shared_ptr<GameObject>();
}

void Scene::DontDestroyOnLoad(std::shared_ptr<GameObject> object, std::string tag) noexcept
{
	dont_destroy_objects_[tag] = object;
}

std::unordered_map<std::string, std::shared_ptr<GameObject>> Scene::GetDontDestroyObjects() noexcept
{
	return dont_destroy_objects_;
}

void Scene::ChangeSceneImmediate(std::string tag) noexcept
{
	manager_.ChangeSceneImmediate(tag);
}

void Scene::ChangeScene(std::string tag)
{
	manager_.ChangeScene(tag);
}
