#pragma once
#include "memory"
#include "string"
#include "Core/Component/component.hpp"
#include "Utility/Math/quaternion.hpp"

//class Transform final : public Component
//{
//public:
//	Transform() = delete;
//	explicit Transform(GameObject& owner) noexcept
//		: Component{ owner }
//		, localMatrix_(MGetIdent())
//	{
//	}
//	
//	explicit Transform(GameObject& owner, const VECTOR& localPosition) noexcept
//		: Component{ owner }
//		, localMatrix_(MGetIdent())
//	{
//		localMatrix_.m[3][0] = localPosition.x;
//		localMatrix_.m[3][1] = localPosition.y;
//		localMatrix_.m[3][2] = localPosition.z;
//	}
//
//	~Transform()	  noexcept = default;
//
//	void initialize() noexcept  override;
//
//	void move		(const VECTOR& velocity) noexcept;
//	void setPosition(const VECTOR& position) noexcept;
//	void setRotation(const VECTOR& rotation) noexcept;
//	void setScale	(const VECTOR& scale   ) noexcept;
//	void setScale   (const float&  uniform ) noexcept;
//
//private:
//	VECTOR localPosition_;
//	VECTOR localRotation_;
//	VECTOR localScale_;
//	VECTOR eulerAngles_;
//
//	MATRIX localMatrix_;
//	
//};

using namespace my_math;

class Transform final : public Component
{
private:
    // ローカル座標系
    VECTOR     localPosition_;
    Quaternion localRotation_;
    VECTOR     localScale_;

    VECTOR     worldPosition_;
    Quaternion worldRotation_;
    VECTOR     worldScale_;
    MATRIX     worldMatrix_;
    bool       isDirty_;

    // 親子関係
    Transform* parent_;
    std::vector<Transform*> children_;

public:
    explicit Transform(GameObject& object) noexcept
        : Component(object)
        , localPosition_(VGet(0.0f, 0.0f, 0.0f))
        , localRotation_(Quaternion::Identity())
        , localScale_(VGet(1.0f, 1.0f, 1.0f))
        , worldPosition_(VGet(0.0f, 0.0f, 0.0f))
        , worldRotation_(Quaternion::Identity())
        , worldScale_(VGet(1.0f, 1.0f, 1.0f))
        , isDirty_(true)
        , parent_(nullptr)
    {
        worldMatrix_ = MGetIdent();
    }

    virtual ~Transform() noexcept override
    {
        // 親から自分を削除
        if (parent_) {
            parent_->removeChild(this);
        }

        // 子供たちの親をnullに設定
        for (auto* child : children_) {
            if (child) {
                child->parent_ = nullptr;
            }
        }
    }

    virtual void Initialize() noexcept override
    {
        isDirty_ = true;
    }

    // ==================================================
    // ローカル座標系の操作
    // ==================================================

    // ローカル位置
    const VECTOR& getLocalPosition() const { return localPosition_; }
    void setLocalPosition(const VECTOR& pos)
    {
        localPosition_ = pos;
        markDirty();
    }
    void setLocalPosition(float x, float y, float z)
    {
        localPosition_ = VGet(x, y, z);
        markDirty();
    }

    // ローカル回転
    const Quaternion& getLocalRotation() const { return localRotation_; }
    void setLocalRotation(const Quaternion& rot)
    {
        localRotation_ = rot.Normalized();
        markDirty();
    }
    void setLocalRotationEuler(const VECTOR& euler)
    {
        localRotation_ = Quaternion::FromEulerRadians(euler.x, euler.y, euler.z);
        markDirty();
    }
    void setLocalRotationEuler(float x, float y, float z)
    {
        localRotation_ = Quaternion::FromEulerRadians(x, y, z);
        markDirty();
    }

    // ローカルスケール
    const VECTOR& getLocalScale() const { return localScale_; }
    void setLocalScale(const VECTOR& scale)
    {
        localScale_ = scale;
        markDirty();
    }
    void setLocalScale(float x, float y, float z)
    {
        localScale_ = VGet(x, y, z);
        markDirty();
    }
    void setLocalScale(float uniScale)
    {
        localScale_ = VGet(uniScale, uniScale, uniScale);
        markDirty();
    }

    // ==================================================
    // ワールド座標の更新と取得
    // ==================================================

    // ワールド座標系を強制的に更新
    void updateWorldTransform()
    {
        if (!isDirty_) return;

        if (parent_) {
            parent_->updateWorldTransform();

            // 親のワールド変換を適用
            worldPosition_ = parent_->getWorldPosition();
            worldRotation_ = parent_->getWorldRotation();
            worldScale_    = parent_->getWorldScale();

            // ローカル変換を適用
            VECTOR scaledPos = VGet(
                localPosition_.x * worldScale_.x,
                localPosition_.y * worldScale_.y,
                localPosition_.z * worldScale_.z
            );
            VECTOR rotatedPos = worldRotation_.rotateVector(scaledPos);
            worldPosition_ = VAdd(worldPosition_, rotatedPos);

            worldRotation_ = worldRotation_ * localRotation_;

            worldScale_ = VGet(
                worldScale_.x * localScale_.x,
                worldScale_.y * localScale_.y,
                worldScale_.z * localScale_.z
            );
        }
        else {
            // 親がいない場合はローカルがワールド
            worldPosition_ = localPosition_;
            worldRotation_ = localRotation_;
            worldScale_    = localScale_;
        }

        // ワールド行列を更新
        MATRIX scaleMatrix = MGetScale(worldScale_);
        MATRIX rotMatrix   = worldRotation_.toMatrix();
        MATRIX transMatrix = MGetTranslate(worldPosition_);

        worldMatrix_ = MMult(scaleMatrix, MMult(rotMatrix, transMatrix));

        isDirty_ = false;
    }

    const VECTOR& getWorldPosition()
    {
        updateWorldTransform();
        return worldPosition_;
    }

    const Quaternion& getWorldRotation()
    {
        updateWorldTransform();
        return worldRotation_;
    }

    const VECTOR& getWorldScale()
    {
        updateWorldTransform();
        return worldScale_;
    }

    void setWorldPosition(const VECTOR& pos)
    {
        if (parent_) {
            updateWorldTransform();
            VECTOR     parentPos    = parent_->getWorldPosition();
            Quaternion parentRotInv = parent_->getWorldRotation().inverse();
            VECTOR     parentScale  = parent_->getWorldScale();

            VECTOR localPos = VSub(pos, parentPos);
            localPos = parentRotInv.rotateVector(localPos);
            localPos = VGet(
                localPos.x / parentScale.x,
                localPos.y / parentScale.y,
                localPos.z / parentScale.z
            );

            localPosition_ = localPos;
        }
        else {
            localPosition_ = pos;
        }
        markDirty();
    }

    void setWorldRotation(const Quaternion& rot)
    {
        if (parent_) {
            updateWorldTransform();
            Quaternion parentRotInv = parent_->getWorldRotation().inverse();
            localRotation_          = parentRotInv * rot;
        }
        else {
            localRotation_          = rot;
        }
        markDirty();
    }

    // ==================================================
    // 移動・回転操作
    // ==================================================

    void translate(const VECTOR& translation, bool worldSpace = false)
    {
        if (worldSpace) {
            localPosition_ = VAdd(localPosition_, translation);
        }
        else {
            VECTOR localTranslation = localRotation_.rotateVector(translation);
            localPosition_          = VAdd(localPosition_, localTranslation);
        }
        markDirty();
    }

    void rotate(const Quaternion& rotation, bool worldSpace = false)
    {
        if (worldSpace) {
            localRotation_ = rotation * localRotation_;
        }
        else {
            localRotation_ = localRotation_ * rotation;
        }

        localRotation_.normalize();
        markDirty();
    }

    void rotateEuler(const VECTOR& eulerAngles, bool worldSpace = false)
    {
        Quaternion rotation = Quaternion::fromEulerRadians(eulerAngles.x, eulerAngles.y, eulerAngles.z);
        rotate(rotation, worldSpace);
    }

    void lookAt(const VECTOR& target, const VECTOR& up = VGet(0.0f, 1.0f, 0.0f))
    {
        VECTOR worldPos = getWorldPosition();
        VECTOR forward  = VNorm(VSub(target, worldPos));

        Quaternion lookRotation = Quaternion::fromToRotation(VGet(0.0f, 0.0f, 1.0f), forward);
        setWorldRotation(lookRotation);
    }

    // ==================================================
    // 方向ベクトル
    // ==================================================

    VECTOR getForward()
    {
        return getWorldRotation().rotateVector(VGet(0.0f, 0.0f, 1.0f));
    }

    VECTOR getRight()
    {
        return getWorldRotation().rotateVector(VGet(1.0f, 0.0f, 0.0f));
    }

    VECTOR getUp()
    {
        return getWorldRotation().rotateVector(VGet(0.0f, 1.0f, 0.0f));
    }

    // ==================================================
    // 親子関係
    // ==================================================

    Transform* getParent() const { return parent_; }

    void setParent(Transform* newParent)
    {
        if (parent_ == newParent) return;

        // 古い親から削除
        if (parent_) {
            parent_->removeChild(this);
        }

        // 新しい親に追加
        parent_ = newParent;
        if (parent_) {
            parent_->addChild(this);
        }

        markDirty();
    }

    const std::vector<Transform*>& getChildren() const { return children_; }

    Transform* getChild(size_t index) const
    {
        if (index < children_.size()) {
            return children_[index];
        }
        return nullptr;
    }

    size_t getChildCount() const { return children_.size(); }

    // ==================================================
    // 行列取得
    // ==================================================

    const MATRIX& getWorldMatrix()
    {
        updateWorldTransform();
        return worldMatrix_;
    }

    MATRIX getLocalMatrix() const
    {
        MATRIX scaleMatrix = MGetScale(localScale_);
        MATRIX rotMatrix   = localRotation_.toMatrix();
        MATRIX transMatrix = MGetTranslate(localPosition_);

        return MMult(scaleMatrix, MMult(rotMatrix, transMatrix));
    }

private:
    void addChild(Transform* child)
    {
        if (child && std::find(children_.begin(), children_.end(), child) == children_.end()) {
            children_.push_back(child);
        }
    }

    void removeChild(Transform* child)
    {
        auto it = std::find(children_.begin(), children_.end(), child);
        if (it != children_.end()) {
            children_.erase(it);
        }
    }

    void markDirty()
    {
        isDirty_ = true;
        // 子供たちもダーティにする
        for (auto* child : children_) {
            if (child) {
                child->markDirty();
            }
        }
    }
};