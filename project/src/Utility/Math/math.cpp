#include "DxLib.h"
#include "Utility/Math/math.hpp"

namespace my_math
{
	#pragma region ƒxƒNƒgƒ‹
	[[nodiscard]] VECTOR GetUnitVector(const VECTOR& vec)
	{
		return VSize(vec) != 0 ? VNorm(vec) : vec;
	}
	#pragma endregion

}