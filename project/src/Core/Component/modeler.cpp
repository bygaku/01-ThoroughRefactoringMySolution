#include "DxLib.h"
#include "Component/modeler.hpp"

Modeler::~Modeler() noexcept
{
	if (!modelHandle_) return;
	MV1DeleteModel(modelHandle_);
}

void Modeler::initialize() noexcept
{
}

void Modeler::draw() const noexcept
{
	if (!modelHandle_) return;
	MV1DrawModel(modelHandle_);
}

void Modeler::setModelHandle(std::string modelPath) noexcept
{
	if (modelHandle_ && modelHandle_ != -1) return;
	modelHandle_ = MV1LoadModel(modelPath.c_str());
}

void Modeler::resetModelHandle() noexcept
{
	if (!modelHandle_) return;
	MV1DeleteModel(modelHandle_);
	modelHandle_ = -1;
}

void Modeler::setMatrix(MATRIX mat) noexcept
{
	if (!modelHandle_) return;

	// YŽ²‚Å180“x‰ñ“]
	//MATRIX modelRotation = MGetRotY(DX_PI_F);
	//MATRIX finalMatrix   = MMult(modelRotation, mat);

	MV1SetMatrix(modelHandle_, mat);
}
