#include "DxLib.h"
#include "Utility/Math/math.hpp"

namespace my_math
{
	#pragma region ベクトル
	[[nodiscard]] VECTOR GetUnitVector(const VECTOR& vector)
	{
		return VSize(vector) != 0 ? VNorm(vector) : vector;
	}
	#pragma endregion

	#pragma region 四元数
	float QDot(Quaternion q1, Quaternion q2)
	{
		return (q1.x_ * q2.x_) + (q1.y_ * q2.y_) + (q1.z_ * q2.z_) + (q1.w_ + q2.w_);
	}
	#pragma endregion
}