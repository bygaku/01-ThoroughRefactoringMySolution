#pragma once
#include "Object/game_object.hpp"

#include "Core/Component/health.hpp"
#include "Core/Component/modeler.hpp"
#include "Core/Component/animator.hpp"
          
class CharaObject abstract : public GameObject
{
public:
    CharaObject() = delete;
    explicit CharaObject(const std::string& tag, uint32_t health) noexcept
        : GameObject{ tag }
    {
        health_	  = std::make_shared<Health>  (*this, health);
        model_	  = std::make_shared<Modeler> (*this);
        animator_ = std::make_shared<Animator>(*this, model_);
    }

    virtual ~CharaObject()	  noexcept = default;

    virtual void Initialize() noexcept abstract;
    virtual void Update()	  noexcept abstract;
    virtual void Draw() const noexcept abstract;
    virtual void LastUpdate() noexcept abstract;

protected:
    /// @remark ‘Ì—Í‘€ì
    void OnDamage(uint32_t amount) noexcept { health_->TakeDamage(amount); }
    void OnHeal	 (uint32_t amount) noexcept { health_->TakeHeal  (amount); }

protected:
    std::shared_ptr<Health>   health_;
    std::shared_ptr<Modeler>  model_;
    std::shared_ptr<Animator> animator_;

};
