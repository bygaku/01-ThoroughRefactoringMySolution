#pragma once
#include <algorithm>
#include <optional>
#include <array>
#include <unordered_map>

#pragma region Math関連Link
#include "Utility/Math/quaternion.hpp"
#include "Utility/Math/vector2d.hpp"
#pragma endregion

namespace my_math
{
	static constexpr float kDegreesToRadian = DX_PI_F /  180.0f;	/// @brief ディグリーをラジアンに変換(変換対象と掛け算を行う)
	static constexpr float kRadianToDegrees = 180.0f  / DX_PI_F;	/// @brief ラジアンをディグリーに変換(変換対象と掛け算を行う)

	#pragma region ベクトル
	/// @brief 単位ベクトル変換
	/// @return 0ベクトルでなければ、正規化したベクトルを返す
	[[nodiscard]] VECTOR GetUnitVector(const VECTOR& vec);

	#pragma endregion

}