#pragma once
#include <memory>

class GameObject;

class Component abstract
{
public:
	Component() = delete;
	Component(GameObject& object) noexcept
		: owner_(object)
		, is_active_(true)
	{
	};

	virtual ~Component()	  noexcept = default;

	virtual void Initialize() noexcept	abstract;

	/// @brief �R���|�[�l���g�L����
	void Activate()   noexcept { is_active_ =  true; }
	/// @brief �R���|�[�l���g�̏������~
	void Deactivate() noexcept { is_active_ = false; }

protected:
	/// @brief �����҂̃A�N�Z�X���擾
	GameObject* GetOwner() const noexcept { return &owner_; };

private:
	GameObject& owner_;
	bool		is_active_;

};