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

#pragma region �A�N�Z�T
	/// @remark ����
	using HEALTH_CHANGED_CALLBACK = std::function<void(uint32_t currentHealth, uint32_t prevHealth)>;
	using DEATH_ACTION_CALLBACK	  = std::function<void()>;

	[[nodiscard]] const uint32_t& getCurrentHealth() const { return currentHealth_; }
	[[nodiscard]] const uint32_t& getMaxHealth()	 const { return		maxHealth_;	}
	[[nodiscard]] const bool	  isAlive()			 const { return		  isAlive_; }

#undef min
#undef max
	/// @attention �񐄏��F���������Ɏg�p
	void setMaxHealth(uint32_t maxHealth);
	
	/// @attention �񐄏��F���������ȊO�ł̎g�p
	void setInvincibilityTime(const float& time);

	/// @brief �̗͕ύX���̏�����o�^
	void registerOnHealthChanged(const HEALTH_CHANGED_CALLBACK& healthChangedProcess);

	/// @brief ���S���̏�����o�^
	void registerOnDeath(const DEATH_ACTION_CALLBACK& deathProcess);
	
	/// @remark �̗͑���
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
