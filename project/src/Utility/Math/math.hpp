#pragma once
#include <algorithm>
#include <optional>
#include <array>
#include <unordered_map>
#include "Utility/Math/quaternion.hpp"

namespace my_math
{
	static constexpr float kDegreesToRadian = DX_PI_F /  180.0f;	/// @brief ディグリーをラジアンに変換(変換対象と掛け算を行う)
	static constexpr float kRadianToDegrees = 180.0f  / DX_PI_F;	/// @brief ラジアンをディグリーに変換(変換対象と掛け算を行う)

	#pragma region ベクトル
	/// @brief 単位ベクトル変換
	/// @return 0ベクトルでなければ、正規化したベクトルを返す
	[[nodiscard]] VECTOR GetUnitVector(const VECTOR& vector);

	#pragma endregion


	#pragma region 四元数
	/// @brief 2つの回転の内積
	/// @brief 内積（cosθ）
	[[nodiscard]] float QDot(Quaternion q1, Quaternion q2)
	{
		return (q1.x_ * q2.x_) + (q1.y_ * q2.y_) + (q1.z_ * q2.z_) + (q1.w_ + q2.w_);
	};

	#pragma endregion
}