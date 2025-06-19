#include "DxLib.h"
#include "Core/Component/transform.hpp"

Transform::Transform(GameObject& object) noexcept
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

Transform::~Transform() noexcept
{
    // 親から自分を削除
    if (parent_) {
        parent_->RemoveChild(this);
    }

    // 子供たちの親をnullに設定
    for (auto* child : children_) {
        if (child) {
            child->parent_ = nullptr;
        }
    }
}

void Transform::Initialize() noexcept
{
    is_dirty_ = true;
}

#pragma region GET AND SET LOCAL TRANSFORM
const VECTOR& Transform::GetLocalPosition() const
{
    return local_position_;
}

void Transform::SetLocalPosition(const VECTOR& pos)
{
    local_position_ = pos;
    MarkDirty();
}

void Transform::SetLocalPosition(float x, float y, float z)
{
    local_position_ = VGet(x, y, z);
    MarkDirty();
}

const Quaternion& Transform::GetLocalRotation() const
{
    return local_rotation_;
}

void Transform::SetLocalRotation(const Quaternion& rot)
{
    local_rotation_ = rot.Normalized();
    MarkDirty();
}

void Transform::SetLocalRotationEuler(const VECTOR& euler)
{
    local_rotation_ = Quaternion::FromEulerRadians(euler.x, euler.y, euler.z);
    MarkDirty();
}

void Transform::SetLocalRotationEuler(float x, float y, float z)
{
    local_rotation_ = Quaternion::FromEulerRadians(x, y, z);
    MarkDirty();
}

const VECTOR& Transform::GetLocalScale() const
{
    return local_scale_;
}

void Transform::SetLocalScale(const VECTOR& scale)
{
    local_scale_ = scale;
    MarkDirty();
}

void Transform::SetLocalScale(float x, float y, float z)
{
    local_scale_ = VGet(x, y, z);
    MarkDirty();
}

void Transform::SetLocalScale(float uni_scale)
{
    local_scale_ = VGet(uni_scale, uni_scale, uni_scale);
    MarkDirty();
}
#pragma endregion

#pragma region UPDATE AND GET WORLD TRANSFORM
/// @brief ワールド座標系を強制的に更新
/// @remark 更新はDirtyがTRUEのときのみ
void Transform::UpdateWorldTransform()
{
    if (!is_dirty_) return;

    if (parent_) {
        parent_->UpdateWorldTransform();

        // 親のワールド変換を適用
        world_position_ = parent_->GetWorldPosition();
        world_rotation_ = parent_->GetWorldRotation();
        world_scale_    = parent_->GetWorldScale();

        // ローカル変換を適用
        VECTOR scaled_pos  = VGet(
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
        // 親がいない場合はローカルがワールド
        world_position_ = local_position_;
        world_rotation_ = local_rotation_;
        world_scale_    = local_scale_;
    }

    // ワールド行列を更新
    MATRIX scale_matrix = MGetScale(world_scale_);
    MATRIX rot_matrix   = world_rotation_.ToMatrix();
    MATRIX trans_matrix = MGetTranslate(world_position_);

    world_matrix_   = MMult(scale_matrix, MMult(rot_matrix, trans_matrix));
    is_dirty_       = false;
}

/// @brief ワールド座標を取得
const VECTOR& Transform::GetWorldPosition()
{
    UpdateWorldTransform();
    return world_position_;
}

/// @brief ワールド上の姿勢を取得
const Quaternion& Transform::GetWorldRotation()
{
    UpdateWorldTransform();
    return world_rotation_;
}

/// @brief ワールド上のスケールを取得
const VECTOR& Transform::GetWorldScale()
{
    UpdateWorldTransform();
    return world_scale_;
}

/// @brief ワールド座標を設定
void Transform::SetWorldPosition(const VECTOR& pos)
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

        local_position_  = local_pos;
    }
    else {
        local_position_  = pos;
    }

    MarkDirty();
}

/// @brief ワールド上の姿勢を設定
void Transform::SetWorldRotation(const Quaternion& rot)
{
    if (parent_) {
        UpdateWorldTransform();

        Quaternion parent_rot_inv = parent_->GetWorldRotation().Inverse();
        local_rotation_           = parent_rot_inv * rot;
    }
    else {
        local_rotation_           = rot;
    }

    MarkDirty();
}
#pragma endregion

#pragma region MOVE/ROTATE FOR OBJECTS
/// @brief オブジェクトの平行移動
/// @param world_space ワールド座標上
void Transform::Translate(const VECTOR& translation, bool world_space)
{
    if (world_space) {
        local_position_ = VAdd(local_position_, translation);
    }
    else {
        VECTOR local_translation = local_rotation_.RotateVector(translation);
        local_position_ = VAdd(local_position_, local_translation);
    }

    MarkDirty();
}

/// @brief オブジェクトを回転させる
/// @param world_space ワールド座標上
void Transform::Rotate(const Quaternion& rotation, bool world_space)
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

/// @brief オブジェクトをオイラー角で回転させる
/// @param world_space ワールド座標上
void Transform::RotateEuler(const VECTOR& euler_angles, bool world_space)
{
    Quaternion rotation = Quaternion::FromEulerRadians(euler_angles.x, euler_angles.y, euler_angles.z);
    Rotate(rotation, world_space);
}

/// @brief target の方向を向く
/// @param up 回転軸 
void Transform::LookAt(const VECTOR& target, const VECTOR& up)
{
    VECTOR world_pos = GetWorldPosition();
    VECTOR forward   = VNorm(VSub(target, world_pos));

    Quaternion look_rotation = Quaternion::FromToRotation(VGet(0.0f, 0.0f, 1.0f), forward);
    SetWorldRotation(look_rotation);
}
#pragma endregion

#pragma region OBJECT DIRECTION VECTOR
/// @brief オブジェクトの前情報：+Z
VECTOR Transform::GetForward()
{
    return GetWorldRotation().RotateVector(VGet(0.0f, 0.0f, 1.0f));
}

/// @brief オブジェクトの右情報：+X
VECTOR Transform::GetRight()
{
    return GetWorldRotation().RotateVector(VGet(1.0f, 0.0f, 0.0f));
}

/// @brief オブジェクトの上情報：+Y
VECTOR Transform::GetUp()
{
    return GetWorldRotation().RotateVector(VGet(0.0f, 1.0f, 0.0f));
}
#pragma endregion

#pragma region PARENT
/// @brief 親の情報を取得
Transform* Transform::GetParent() const
{
    return parent_;
}

/// @brief 親を登録する
void Transform::SetParent(Transform* new_parent)
{
    if (parent_ == new_parent) return;

    // 古い親から削除
    if (parent_) {
        parent_->RemoveChild(this);
    }

    // 新しい親に追加
    parent_ = new_parent;
    if (parent_) {
        parent_->AddChild(this);
    }

    MarkDirty();
}

/// @brief 登録されている子のコンテナを取得
const std::vector<Transform*>& Transform::GetChildren() const
{
    return children_;
}

/// @brief 登録されている子の情報を取得
/// @param index 番目に登録された子
Transform* Transform::GetChild(size_t index) const
{
    if (index < children_.size()) {
        return children_[index];
    }

    return nullptr;
}

/// @brief 子の接続数を取得
size_t Transform::GetChildCount() const
{
    return children_.size();
}
#pragma endregion

#pragma region GET MATRIX
/// @brief ワールド座標行列
const MATRIX& Transform::GetWorldMatrix()
{
    UpdateWorldTransform();
    return world_matrix_;
}

/// @brief ローカル座標行列
MATRIX Transform::GetLocalMatrix() const
{
    MATRIX scale_matrix = MGetScale(local_scale_);
    MATRIX rot_matrix   = local_rotation_.ToMatrix();
    MATRIX trans_matrix = MGetTranslate(local_position_);

    return MMult(scale_matrix, MMult(rot_matrix, trans_matrix));
}
#pragma endregion

#pragma region PRIVATE METHODS
void Transform::AddChild(Transform* child)
{
    if (child && std::find(children_.begin(), children_.end(), child) == children_.end()) {
        children_.push_back(child);
    }
}

void Transform::RemoveChild(Transform* child)
{
    auto it = std::find(children_.begin(), children_.end(), child);
    if (it != children_.end()) {
        children_.erase(it);
    }
}

void Transform::MarkDirty()
{
    is_dirty_ = true;

    for (auto* child : children_) {
        if (child) {
            child->MarkDirty();
        }
    }
}
#pragma endregion