#pragma once
#include <algorithm>
#include <optional>
#include <array>
#include <unordered_map>
#include "Utility/Math/quaternion.hpp"

namespace my_math
{
	static constexpr float kDegreesToRadian = DX_PI_F /  180.0f;	/// @brief �f�B�O���[�����W�A���ɕϊ�(�ϊ��ΏۂƊ|���Z���s��)
	static constexpr float kRadianToDegrees = 180.0f  / DX_PI_F;	/// @brief ���W�A�����f�B�O���[�ɕϊ�(�ϊ��ΏۂƊ|���Z���s��)

	#pragma region �\�[�g
	/// @brief unorderd_map��value���\�[�g
	/// @param ascend �����\�[�g�FTRUE
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

	#pragma region �x�N�g��
	/// @brief �P�ʃx�N�g���ϊ�
	/// @return 0�x�N�g���łȂ���΁A���K�������x�N�g����Ԃ�
	[[nodiscard]] VECTOR GetUnitVector(const VECTOR& vector);

	#pragma endregion


	#pragma region �l����
	/// @brief 2�̉�]�̓���
	/// @brief ���ρicos�Ɓj
	[[nodiscard]] float QDot(Quaternion q1, Quaternion q2);

	#pragma endregion
}