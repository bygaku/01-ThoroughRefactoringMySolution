#include "DxLib.h"
#include "Core/Component/modeler.hpp"

Modeler::Modeler(GameObject& owner) noexcept
	: Component{ owner }
	, model_handle_(-1)
{
};

Modeler::~Modeler() noexcept
{
	if (!model_handle_) return;
	MV1DeleteModel(model_handle_);
}

void Modeler::Initialize() noexcept
{
}

void Modeler::Draw() const noexcept
{
	if (!model_handle_) return;
	MV1DrawModel(model_handle_);
}

int Modeler::GetModelHandle() const noexcept
{
	return model_handle_;
}

void Modeler::SetModelHandle(std::string model_path) noexcept
{
	if (model_handle_ && model_handle_ != -1) return;
	model_handle_ = MV1LoadModel(model_path.c_str());
}

void Modeler::ResetModelHandle() noexcept
{
	if (!model_handle_) return;
	MV1DeleteModel(model_handle_);
	model_handle_ = -1;
}

void Modeler::SetMatrix(MATRIX mat) noexcept
{
	if (!model_handle_) return;

	// YŽ²‚Å180“x‰ñ“]
	//MATRIX modelRotation = MGetRotY(DX_PI_F);
	//MATRIX finalMatrix   = MMult(modelRotation, mat);

	MV1SetMatrix(model_handle_, mat);
}
