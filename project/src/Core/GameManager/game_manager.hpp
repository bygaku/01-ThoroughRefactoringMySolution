#pragma once
#include <memory>
#include <unordered_map>
#include <string>

class GameTime;
class SceneManager;

class GameManager final
{
public:
	GameManager()  noexcept;
	~GameManager() noexcept;

	/// @brief ���[�v�R���e�B�j���[
	/// @retval TRUE
	/// @retval FALSE: �Q�[���I��.
	bool IsRunning() const noexcept;
	void Update()		   noexcept;

private:
	/// @brief �N�����Ԃ̍X�V
	void UpdateBootTime()  noexcept;
#ifdef _DEBUG
	/// @brief �f�o�b�O�̕\��
	void DebugOnDisplay() const noexcept;

#endif

private:
	std::shared_ptr<SceneManager> scene_manager_;
	std::shared_ptr<GameTime>	  game_time_;

private:
	float		time_;			/// @brief �N������̎���
	static bool instantiated_;

};
