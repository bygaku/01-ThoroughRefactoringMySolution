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

	#pragma region �x�N�g��
	/// @brief �P�ʃx�N�g���ϊ�
	/// @return 0�x�N�g���łȂ���΁A���K�������x�N�g����Ԃ�
	[[nodiscard]] VECTOR GetUnitVector(const VECTOR& vector);

	#pragma endregion


	#pragma region �l����
	/// @brief 2�̉�]�̓���
	/// @brief ���ρicos�Ɓj
	[[nodiscard]] float QDot(Quaternion q1, Quaternion q2)
	{
		return (q1.x_ * q2.x_) + (q1.y_ * q2.y_) + (q1.z_ * q2.z_) + (q1.w_ + q2.w_);
	};

	#pragma endregion
}