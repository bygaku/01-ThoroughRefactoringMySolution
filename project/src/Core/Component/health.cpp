#include "DxLib.h"
#include "Core/Component/health.hpp"

void Health::initialize() noexcept
{
}

void Health::setMaxHealth(uint32_t maxHealth)
{
	maxHealth_	   = maxHealth; 
	currentHealth_ = std::min(currentHealth_, maxHealth_);
}

void Health::setInvincibilityTime(const float& time)
{
	invincibilityTime_ = time;
}

void Health::registerOnHealthChanged(const HEALTH_CHANGED_CALLBACK& healthChangedProcess)
{
	onHealthChanged_ = healthChangedProcess;
}

void Health::registerOnDeath(const DEATH_ACTION_CALLBACK& deathProcess)
{
	onDeath_ = deathProcess;
}

void Health::takeDamage(uint32_t amount) noexcept
{
	printfDx("\nDamage");
}

void Health::takeHeal(uint32_t amount) noexcept
{
	printfDx("\nHeal");
}

void Health::reset() noexcept
{
	currentHealth_ = maxHealth_; 
	isAlive_	   = true;
}
