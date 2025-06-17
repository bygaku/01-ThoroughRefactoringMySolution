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

	/// @brief ループコンティニュー
	/// @retval TRUE
	/// @retval FALSE: ゲーム終了.
	bool IsRunning() const noexcept;
	void Update()		   noexcept;

private:
	/// @brief 起動時間の更新
	void UpdateBootTime()  noexcept;
#ifdef _DEBUG
	/// @brief デバッグの表示
	void DebugOnDisplay() const noexcept;

#endif

private:
	std::shared_ptr<SceneManager> scene_manager_;
	std::shared_ptr<GameTime>	  game_time_;

private:
	float		time_;			/// @brief 起動からの時間
	static bool instantiated_;

};
