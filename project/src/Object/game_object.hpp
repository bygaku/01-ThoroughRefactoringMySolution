#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Object/Data/object_tag.hpp"
#include "Core/Component/transform.hpp"

class GameObject abstract
{
public:
    GameObject() = delete;
    GameObject(const std::string& tag) noexcept
        : transform_(std::make_shared<Transform>(*this))
        , name_(tag)
        , is_active_(false)
    {
    };

    virtual ~GameObject() noexcept = default;

    virtual void Initialize() noexcept abstract;
    virtual void Update()	  noexcept abstract;
    virtual void Draw() const noexcept abstract;
    virtual void LastUpdate() noexcept abstract;

    /// @brief オブジェクトを有効化
    void Activate()  noexcept  { is_active_ =  true; };
    /// @brief オブジェクトを削除せずに処理を停止
    void Deactivate() noexcept { is_active_ = false; };

    /// @remark 情報取得
    std::string GetName()      const noexcept { return	          name_; };
    bool		IsActive()     const noexcept { return       is_active_; };
    Transform*  GetTransform() const noexcept { return transform_.get(); };

protected:
    std::shared_ptr<Transform> transform_;

private:
    bool	    is_active_;
    std::string name_;

};