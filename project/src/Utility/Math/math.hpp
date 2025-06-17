#pragma once
#include <algorithm>
#include <optional>
#include <array>
#include <unordered_map>

#pragma region Math�֘ALink
#include "Utility/Math/quaternion.hpp"
#include "Utility/Math/vector2d.hpp"
#pragma endregion

namespace my_math
{
	static constexpr float kDegreesToRadian = DX_PI_F /  180.0f;	/// @brief �f�B�O���[�����W�A���ɕϊ�(�ϊ��ΏۂƊ|���Z���s��)
	static constexpr float kRadianToDegrees = 180.0f  / DX_PI_F;	/// @brief ���W�A�����f�B�O���[�ɕϊ�(�ϊ��ΏۂƊ|���Z���s��)

	#pragma region �x�N�g��
	/// @brief �P�ʃx�N�g���ϊ�
	/// @return 0�x�N�g���łȂ���΁A���K�������x�N�g����Ԃ�
	[[nodiscard]] VECTOR GetUnitVector(const VECTOR& vec);

	#pragma endregion

}