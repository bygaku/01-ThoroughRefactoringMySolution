#pragma once
#include "Core/Component/component.hpp"
#include <cstdint>
#include <functional>
#include <algorithm>

class Health final : public Component
{
public:
	Health() = delete;
	explicit Health(GameObject& owner, uint32_t max_health) noexcept
		: Component{ owner }
		, current_health_(max_health)
		, max_health_(max_health)
		, is_alive_(true)
		, is_invincible_(false)
		, invincibility_time_(0.0f)
		, invincibility_timer_(0.0f)
	{
	};

	~Health()		  noexcept = default;

	void Initialize() noexcept  override;

#pragma region アクセサ
	/// @remark 命名
	using HEALTH_CHANGED_CALLBACK = std::function<void(uint32_t current_health, uint32_t prev_health)>;
	using DEATH_ACTION_CALLBACK	  = std::function<void()>;

	[[nodiscard]] const uint32_t& GetCurrentHealth() const { return current_health_; }
	[[nodiscard]] const uint32_t& GetMaxHealth()	 const { return		max_health_; }
	[[nodiscard]] const bool	  IsAlive()			 const { return		  is_alive_; }

#undef min
#undef max
	/// @attention 非推奨：初期化時に使用
	void SetMaxHealth(uint32_t max_health);
	
	/// @attention 非推奨：初期化時以外での使用
	void SetInvincibilityTime(const float& time);

	/// @brief 体力変更時の処理を登録
	void RegisterOnHealthChanged(const HEALTH_CHANGED_CALLBACK& health_changed_process);

	/// @brief 死亡時の処理を登録
	void RegisterOnDeath(const DEATH_ACTION_CALLBACK& death_process);
	
	/// @remark 体力操作
	void TakeDamage(uint32_t amount) noexcept;
	void TakeHeal  (uint32_t amount) noexcept;
	void Reset()					 noexcept;

#pragma endregion

private:
	bool  is_alive_;
	bool  is_invincible_;
	float invincibility_time_;
	float invincibility_timer_;

	HEALTH_CHANGED_CALLBACK on_health_changed_;
	DEATH_ACTION_CALLBACK   on_death_;
	uint32_t				current_health_;
	uint32_t				max_health_;

};
