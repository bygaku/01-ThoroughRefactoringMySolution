#pragma once
#include "Core/GameManager/game_manager.hpp"

class Scene;

class SceneManager final
{
public:
	SceneManager() = delete;
	SceneManager(GameManager& game) noexcept;
	~SceneManager()					noexcept;

	void Update()					noexcept;
	void Draw()						noexcept;
	void Debug()					noexcept;
	void LastUpdate()				noexcept;

	/// @brief �V�[����ύX
	void ChangeScene(std::string tag)							  noexcept;

	/// @brief �V�[���𑦍��ɕύX
	void ChangeSceneImmediate(std::string tag)					  noexcept;

	/// @brief �w��V�[�����擾
	std::shared_ptr<Scene> GetScene(const std::string& tag) const noexcept;

private:
	GameManager& game_;
	static bool  instantiated_;

	std::unordered_map<std::string, std::shared_ptr<Scene>> scenes_;
	std::shared_ptr<Scene> current_scene_;
	std::shared_ptr<Scene> start_up_scene_;

};
