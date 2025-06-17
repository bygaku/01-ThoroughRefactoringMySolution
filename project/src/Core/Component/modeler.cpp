#include "DxLib.h"
#include "Core/Component/modeler.hpp"

Modeler::~Modeler() noexcept
{
	if (!modelHandle_) return;
	MV1DeleteModel(modelHandle_);
}

void Modeler::Initialize() noexcept
{
}

void Modeler::Draw() const noexcept
{
	if (!modelHandle_) return;
	MV1DrawModel(modelHandle_);
}

void Modeler::SetModelHandle(std::string modelPath) noexcept
{
	if (modelHandle_ && modelHandle_ != -1) return;
	modelHandle_ = MV1LoadModel(modelPath.c_str());
}

void Modeler::ResetModelHandle() noexcept
{
	if (!modelHandle_) return;
	MV1DeleteModel(modelHandle_);
	modelHandle_ = -1;
}

void Modeler::SetMatrix(MATRIX mat) noexcept
{
	if (!modelHandle_) return;

	// YŽ²‚Å180“x‰ñ“]
	//MATRIX modelRotation = MGetRotY(DX_PI_F);
	//MATRIX finalMatrix   = MMult(modelRotation, mat);

	MV1SetMatrix(modelHandle_, mat);
}
