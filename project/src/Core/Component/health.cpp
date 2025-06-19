#include "DxLib.h"
#include "Core/Component/health.hpp"

Health::Health(GameObject& owner, uint32_t max_health) noexcept
	: Component{ owner }
	, current_health_(max_health)
	, max_health_(max_health)
	, is_alive_(true)
	, is_invincible_(false)
	, invincibility_time_(0.0f)
	, invincibility_timer_(0.0f)
{
};

void Health::Initialize() noexcept
{
}

void Health::SetMaxHealth(uint32_t max_health)
{
	max_health_	    = max_health; 
	current_health_ = std::min(current_health_, max_health_);
}

void Health::SetInvincibilityTime(const float& time)
{
	invincibility_time_ = time;
}

void Health::RegisterOnHealthChanged(const HEALTH_CHANGED_CALLBACK& health_changed_process)
{
	on_health_changed_ = health_changed_process;
}

void Health::RegisterOnDeath(const DEATH_ACTION_CALLBACK& death_process)
{
	on_death_ = death_process;
}

void Health::TakeDamage(uint32_t amount) noexcept
{
	printfDx("\nDamage");
}

void Health::TakeHeal(uint32_t amount) noexcept
{
	printfDx("\nHeal");
}

void Health::Reset() noexcept
{
	current_health_ = max_health_; 
	is_alive_	    = true;
}
