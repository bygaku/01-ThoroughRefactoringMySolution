#pragma once
#include <memory>
#include <string>
#include "Core/Component/component.hpp"
#include "Utility/Math/quaternion.hpp"

using namespace my_math;

class Transform final : public Component
{
public:
    Transform(GameObject& object) noexcept;

    ~Transform()                  noexcept override;

    void Initialize()             noexcept override;

    #pragma region ローカル座標系の操作
    /// @brief オブジェクトの座標を取得
    const VECTOR&     GetLocalPosition() const;

    /// @brief オブジェクトの姿勢を取得
    const Quaternion& GetLocalRotation() const;

    /// @brief オブジェクトのスケール取得
    const VECTOR&     GetLocalScale()    const;

    /// @remark オブジェクトの座標を設定 
    void              SetLocalPosition     (const VECTOR& pos);
    void              SetLocalPosition     (float x, float y, float z);

    /// @brief オブジェクトの姿勢を設定
    void              SetLocalRotation     (const Quaternion& rot);

    /// @remark オブジェクトの姿勢をオイラー角で設定
    void              SetLocalRotationEuler(const VECTOR& euler);
    void              SetLocalRotationEuler(float x, float y, float z);

    /// @remark オブジェクトのスケール設定
    void              SetLocalScale        (const VECTOR& scale);
    void              SetLocalScale        (float x, float y, float z);
    void              SetLocalScale        (float uni_scale);
    #pragma endregion

    #pragma region ワールド座標の更新と取得
    /// @brief ワールド座標系を強制的に更新
    void              UpdateWorldTransform();

    /// @brief ワールド座標を取得
    const VECTOR&     GetWorldPosition();

    /// @brief ワールド上の姿勢を取得
    const Quaternion& GetWorldRotation();

    /// @brief ワールド上のスケールを取得
    const VECTOR&     GetWorldScale();

    /// @brief ワールド座標を設定
    void              SetWorldPosition(const VECTOR& pos);

    /// @brief ワールド上の姿勢を設定
    void              SetWorldRotation(const Quaternion& rot);
    #pragma endregion

    #pragma region 移動・回転操作
    /// @brief オブジェクトの平行移動
    /// @param world_space ワールド座標上
    void Translate  (const VECTOR& translation,  bool world_space = false);

    /// @brief オブジェクトを回転させる
    /// @param world_space ワールド座標上
    void Rotate     (const Quaternion& rotation, bool world_space = false);

    /// @brief オブジェクトをオイラー角で回転させる
    /// @param world_space ワールド座標上
    void RotateEuler(const VECTOR& euler_angles, bool world_space = false);

    /// @brief target の方向を向く
    /// @param up 回転軸 
    void LookAt     (const VECTOR& target, const VECTOR& up = VGet(0.0f, 1.0f, 0.0f));
    #pragma endregion

    #pragma region 方向ベクトル
    /// @brief オブジェクトの前情報：+Z
    VECTOR GetForward();

    /// @brief オブジェクトの右情報：+X
    VECTOR GetRight();

    /// @brief オブジェクトの上情報：+Y
    VECTOR GetUp();
    #pragma endregion

    #pragma region 親子関係
    /// @brief 親の情報を取得
    Transform*                     GetParent()            const;

    /// @brief 親を登録する
    void                           SetParent(Transform* new_parent);

    /// @brief 登録されている子のコンテナを取得
    const std::vector<Transform*>& GetChildren()          const;

    /// @brief 登録されている子の情報を取得
    /// @param index 番目に登録された子
    Transform*                     GetChild(size_t index) const;

    /// @brief 子の接続数を取得
    size_t                         GetChildCount()        const;
    #pragma endregion

    #pragma region 行列取得
    /// @brief ワールド座標行列
    const MATRIX& GetWorldMatrix();

    /// @brief ローカル座標行列
    MATRIX GetLocalMatrix() const;
    #pragma endregion

private:
    VECTOR     local_position_;
    Quaternion local_rotation_;
    VECTOR     local_scale_;

    VECTOR     world_position_;
    Quaternion world_rotation_;
    VECTOR     world_scale_;
    MATRIX     world_matrix_;
    bool       is_dirty_;

    Transform*              parent_;
    std::vector<Transform*> children_;

private:
    void AddChild(Transform* child);

    void RemoveChild(Transform* child);

    void MarkDirty();

};