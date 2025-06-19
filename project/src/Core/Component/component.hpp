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

	/// @brief コンポーネント有効化
	void Activate()   noexcept { is_active_ =  true; }
	/// @brief コンポーネントの処理を停止
	void Deactivate() noexcept { is_active_ = false; }

protected:
	/// @brief 所持者のアクセス権取得
	GameObject* GetOwner() const noexcept { return &owner_; };

private:
	GameObject& owner_;
	bool		is_active_;

};