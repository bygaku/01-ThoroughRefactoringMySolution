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

	#pragma region ソート
	/// @brief unorderd_mapのvalueをソート
	/// @param ascend 昇順ソート：TRUE
	template<typename Key, typename Value>
	[[nodiscard]] std::unordered_map<Key, Value> Sort(const std::unordered_map<Key, Value>& u_map, const bool ascend)
	{
		std::vector<std::pair<Key, Value>> pairs = ConvertUmapToPairs(u_map);

		std::sort(pairs.begin(), pairs.end(), [](const auto& a, const auto& b)
			{
				return (ascend) ? (a.second < b.second) : (a.second > b.second);
			}
		);

		return ConvertPairsToUmap(pairs);
	}

	#pragma endregion

	#pragma region ベクトル
	/// @brief 単位ベクトル変換
	/// @return 0ベクトルでなければ、正規化したベクトルを返す
	[[nodiscard]] VECTOR GetUnitVector(const VECTOR& vector);

	#pragma endregion


	#pragma region 四元数
	/// @brief 2つの回転の内積
	/// @brief 内積（cosθ）
	[[nodiscard]] float QDot(Quaternion q1, Quaternion q2);

	#pragma endregion
}