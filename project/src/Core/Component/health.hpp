#pragma once
#include "Core/Component/component.hpp"
#include <cstdint>
#include <functional>
#include <algorithm>

class Health final : public Component
{
public:
	Health() = delete;
	explicit Health(GameObject& owner, uint32_t maxHealth) noexcept
		: Component{ owner }
		, currentHealth_(maxHealth)
		, maxHealth_(maxHealth)
		, isAlive_(true)
		, isInvincible_(false)
		, invincibilityTime_(0.0f)
		, invincibilityTimer_(0.0f)
	{
	};

	~Health()		  noexcept = default;

	void initialize() noexcept  override;

#pragma region アクセサ
	/// @remark 命名
	using HEALTH_CHANGED_CALLBACK = std::function<void(uint32_t currentHealth, uint32_t prevHealth)>;
	using DEATH_ACTION_CALLBACK	  = std::function<void()>;

	[[nodiscard]] const uint32_t& getCurrentHealth() const { return currentHealth_; }
	[[nodiscard]] const uint32_t& getMaxHealth()	 const { return		maxHealth_;	}
	[[nodiscard]] const bool	  isAlive()			 const { return		  isAlive_; }

#undef min
#undef max
	/// @attention 非推奨：初期化時に使用
	void setMaxHealth(uint32_t maxHealth);
	
	/// @attention 非推奨：初期化時以外での使用
	void setInvincibilityTime(const float& time);

	/// @brief 体力変更時の処理を登録
	void registerOnHealthChanged(const HEALTH_CHANGED_CALLBACK& healthChangedProcess);

	/// @brief 死亡時の処理を登録
	void registerOnDeath(const DEATH_ACTION_CALLBACK& deathProcess);
	
	/// @remark 体力操作
	void takeDamage(uint32_t amount) noexcept;
	void takeHeal  (uint32_t amount) noexcept;
	void reset()					 noexcept;

#pragma endregion

private:
	bool  isAlive_;
	bool  isInvincible_;
	float invincibilityTime_;
	float invincibilityTimer_;

	HEALTH_CHANGED_CALLBACK onHealthChanged_;
	DEATH_ACTION_CALLBACK   onDeath_;
	uint32_t				currentHealth_;
	uint32_t				maxHealth_;

};
