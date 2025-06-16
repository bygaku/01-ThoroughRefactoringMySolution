#include "DxLib.h"
#include "Utility/utility_link.hpp"
#include "System/system.hpp"

bool System::instantiated_ = false;

System::System() noexcept
{
	ASSERT(instantiated_ == 0, "already instantiated.");
	instantiated_ = true;
}

System::~System() noexcept
{
	instantiated_ = false;
}

bool System::Initialize() noexcept 
{
	ChangeWindowMode(true);
	SetMainWindowText("Dev");
	
	if (DxLib_Init() == -1) {
		return false;
	}

	SetGraphMode(screen_data::kWidth, screen_data::kHeight, screen_data::kColorBit);
	SetDrawScreen(DX_SCREEN_BACK);
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	
	// XInputのみ有効化
	SetUseDirectInputFlag(false);
	SetUseXInputFlag(true);

	// Zバッファ設定
	SetUseZBuffer3D(true);
	SetWriteZBuffer3D(true);
	SetUseBackCulling(true);

	return true;
}
