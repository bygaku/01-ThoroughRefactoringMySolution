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
//	VECTOR local_position_;
//	VECTOR local_rotation_;
//	VECTOR local_scale_;
//	VECTOR eulerAngles_;
//
//	MATRIX localMatrix_;
//	
//};

using namespace my_math;

class Transform final : public Component
{
private:
    // ���[�J�����W�n
    VECTOR     local_position_;
    Quaternion local_rotation_;
    VECTOR     local_scale_;

    VECTOR     world_position_;
    Quaternion world_rotation_;
    VECTOR     world_scale_;
    MATRIX     world_matrix_;
    bool       is_dirty_;

    // �e�q�֌W
    Transform* parent_;
    std::vector<Transform*> children_;

public:
    Transform(GameObject& object) noexcept
        : Component(object)
        , local_position_(VGet(0.0f, 0.0f, 0.0f))
        , local_rotation_(Quaternion::Identity())
        , local_scale_(VGet(1.0f, 1.0f, 1.0f))
        , world_position_(VGet(0.0f, 0.0f, 0.0f))
        , world_rotation_(Quaternion::Identity())
        , world_scale_(VGet(1.0f, 1.0f, 1.0f))
        , is_dirty_(true)
        , parent_(nullptr)
    {
        world_matrix_ = MGetIdent();
    }

    ~Transform() noexcept override
    {
        // �e���玩�����폜
        if (parent_) {
            parent_->RemoveChild(this);
        }

        // �q�������̐e��null�ɐݒ�
        for (auto* child : children_) {
            if (child) {
                child->parent_ = nullptr;
            }
        }
    }

    void Initialize() noexcept override
    {
        is_dirty_ = true;
    }

    // ==================================================
    // ���[�J�����W�n�̑���
    // ==================================================

    // ���[�J���ʒu
    const VECTOR& GetLocalPosition() const { return local_position_; }
    void SetLocalPosition(const VECTOR& pos)
    {
        local_position_ = pos;
        MarkDirty();
    }
    void SetLocalPosition(float x, float y, float z)
    {
        local_position_ = VGet(x, y, z);
        MarkDirty();
    }

    // ���[�J����]
    const Quaternion& GetLocalRotation() const { return local_rotation_; }
    void SetLocalRotation(const Quaternion& rot)
    {
        local_rotation_ = rot.Normalized();
        MarkDirty();
    }
    void SetLocalRotationEuler(const VECTOR& euler)
    {
        local_rotation_ = Quaternion::FromEulerRadians(euler.x, euler.y, euler.z);
        MarkDirty();
    }
    void SetLocalRotationEuler(float x, float y, float z)
    {
        local_rotation_ = Quaternion::FromEulerRadians(x, y, z);
        MarkDirty();
    }

    // ���[�J���X�P�[��
    const VECTOR& GetLocalScale() const { return local_scale_; }
    void SetLocalScale(const VECTOR& scale)
    {
        local_scale_ = scale;
        MarkDirty();
    }
    void SetLocalScale(float x, float y, float z)
    {
        local_scale_ = VGet(x, y, z);
        MarkDirty();
    }
    void SetLocalScale(float uni_scale)
    {
        local_scale_ = VGet(uni_scale, uni_scale, uni_scale);
        MarkDirty();
    }

    // ==================================================
    // ���[���h���W�̍X�V�Ǝ擾
    // ==================================================

    // ���[���h���W�n�������I�ɍX�V
    void UpdateWorldTransform()
    {
        if (!is_dirty_) return;

        if (parent_) {
            parent_->UpdateWorldTransform();

            // �e�̃��[���h�ϊ���K�p
            world_position_ = parent_->GetWorldPosition();
            world_rotation_ = parent_->GetWorldRotation();
            world_scale_    = parent_->GetWorldScale();

            // ���[�J���ϊ���K�p
            VECTOR scaled_pos = VGet(
                local_position_.x * world_scale_.x,
                local_position_.y * world_scale_.y,
                local_position_.z * world_scale_.z
            );

            VECTOR rotated_pos = world_rotation_.RotateVector(scaled_pos);

            world_position_    = VAdd(world_position_, rotated_pos);
            world_rotation_    = world_rotation_ * local_rotation_;
            world_scale_       = VGet(
                world_scale_.x * local_scale_.x,
                world_scale_.y * local_scale_.y,
                world_scale_.z * local_scale_.z
            );
        }
        else {
            // �e�����Ȃ��ꍇ�̓��[�J�������[���h
            world_position_ = local_position_;
            world_rotation_ = local_rotation_;
            world_scale_    = local_scale_;
        }

        // ���[���h�s����X�V
        MATRIX scale_matrix = MGetScale(world_scale_);
        MATRIX rot_matrix   = world_rotation_.ToMatrix();
        MATRIX trans_matrix = MGetTranslate(world_position_);

        world_matrix_ = MMult(scale_matrix, MMult(rot_matrix, trans_matrix));
        is_dirty_     = false;
    }

    const VECTOR& GetWorldPosition()
    {
        UpdateWorldTransform();
        return world_position_;
    }

    const Quaternion& GetWorldRotation()
    {
        UpdateWorldTransform();
        return world_rotation_;
    }

    const VECTOR& GetWorldScale()
    {
        UpdateWorldTransform();
        return world_scale_;
    }

    void SetWorldPosition(const VECTOR& pos)
    {
        if (parent_) {
            UpdateWorldTransform();

            VECTOR     parent_pos     = parent_->GetWorldPosition();
            Quaternion parent_rot_inv = parent_->GetWorldRotation().Inverse();
            VECTOR     parent_scale   = parent_->GetWorldScale();

            VECTOR local_pos = VSub(pos, parent_pos);
            local_pos        = parent_rot_inv.RotateVector(local_pos);
            local_pos        = VGet(
                              local_pos.x / parent_scale.x,
                              local_pos.y / parent_scale.y,
                              local_pos.z / parent_scale.z
                              );

            local_position_ = local_pos;
        }
        else {
            local_position_ = pos;
        }

        MarkDirty();
    }

    void SetWorldRotation(const Quaternion& rot)
    {
        if (parent_) {
            UpdateWorldTransform();

            Quaternion parent_rot_inv = parent_->GetWorldRotation().Inverse();
            local_rotation_           = parent_rot_inv * rot;
        }
        else {
            local_rotation_ = rot;
        }

        MarkDirty();
    }

    // ==================================================
    // �ړ��E��]����
    // ==================================================

    void Translate(const VECTOR& translation, bool world_space = false)
    {
        if (world_space) {
            local_position_ = VAdd(local_position_, translation);
        }
        else {
            VECTOR local_translation = local_rotation_.RotateVector(translation);
            local_position_          = VAdd(local_position_, local_translation);
        }

        MarkDirty();
    }

    void Rotate(const Quaternion& rotation, bool world_space = false)
    {
        if (world_space) {
            local_rotation_ = rotation * local_rotation_;
        }
        else {
            local_rotation_ = local_rotation_ * rotation;
        }

        local_rotation_.Normalize();

        MarkDirty();
    }

    void RotateEuler(const VECTOR& euler_angles, bool world_space = false)
    {
        Quaternion rotation = Quaternion::FromEulerRadians(euler_angles.x, euler_angles.y, euler_angles.z);
        Rotate(rotation, world_space);
    }

    void LookAt(const VECTOR& target, const VECTOR& up = VGet(0.0f, 1.0f, 0.0f))
    {
        VECTOR world_pos = GetWorldPosition();
        VECTOR forward   = VNorm(VSub(target, world_pos));

        Quaternion look_rotation = Quaternion::FromToRotation(VGet(0.0f, 0.0f, 1.0f), forward);
        SetWorldRotation(look_rotation);
    }

    // ==================================================
    // �����x�N�g��
    // ==================================================

    VECTOR GetForward()
    {
        return GetWorldRotation().RotateVector(VGet(0.0f, 0.0f, 1.0f));
    }

    VECTOR GetRight()
    {
        return GetWorldRotation().RotateVector(VGet(1.0f, 0.0f, 0.0f));
    }

    VECTOR GetUp()
    {
        return GetWorldRotation().RotateVector(VGet(0.0f, 1.0f, 0.0f));
    }

    // ==================================================
    // �e�q�֌W
    // ==================================================

    Transform* GetParent() const { return parent_; }

    void SetParent(Transform* new_parent)
    {
        if (parent_ == new_parent) return;

        // �Â��e����폜
        if (parent_) {
            parent_->RemoveChild(this);
        }

        // �V�����e�ɒǉ�
        parent_ = new_parent;
        if (parent_) {
            parent_->AddChild(this);
        }

        MarkDirty();
    }

    const std::vector<Transform*>& GetChildren() const { return children_; }

    Transform* GetChild(size_t index) const
    {
        if (index < children_.size()) {
            return children_[index];
        }

        return nullptr;
    }

    size_t GetChildCount() const { return children_.size(); }

    // ==================================================
    // �s��擾
    // ==================================================

    const MATRIX& GetWorldMatrix()
    {
        UpdateWorldTransform();
        return world_matrix_;
    }

    MATRIX GetLocalMatrix() const
    {
        MATRIX scale_matrix = MGetScale(local_scale_);
        MATRIX rot_matrix   = local_rotation_.ToMatrix();
        MATRIX trans_matrix = MGetTranslate(local_position_);

        return MMult(scale_matrix, MMult(rot_matrix, trans_matrix));
    }

private:
    void AddChild(Transform* child)
    {
        if (child && std::find(children_.begin(), children_.end(), child) == children_.end()) {
            children_.push_back(child);
        }
    }

    void RemoveChild(Transform* child)
    {
        auto it = std::find(children_.begin(), children_.end(), child);
        if (it != children_.end()) {
            children_.erase(it);
        }
    }

    void MarkDirty()
    {
        is_dirty_ = true;
        // �q���������_�[�e�B�ɂ���
        for (auto* child : children_) {
            if (child) {
                child->MarkDirty();
            }
        }
    }
};