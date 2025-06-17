#include "DxLib.h"
#include "Utility/Math/math.hpp"

namespace my_math
{
	#pragma region �x�N�g��
	[[nodiscard]] VECTOR GetUnitVector(const VECTOR& vector)
	{
		return VSize(vector) != 0 ? VNorm(vector) : vector;
	}
	#pragma endregion

}