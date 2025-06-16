#pragma once

namespace screen_data
{
#ifdef _RELEASE
    static constexpr float kWidthF      = 1920.0f;  /// @brief <float> 画面横サイズ
    static constexpr float kHeightF     = 1080.0f;  /// @brief <float> 画面縦サイズ
    static constexpr int   kWidth       =    1920;  /// @brief 画面横サイズ
    static constexpr int   kHeight      =    1080;  /// @brief 画面縦サイズ

    static constexpr float kHalfWidthF  =  960.0f;  /// @brief <float> 画面横ハーフサイズ
    static constexpr float kHalfHeightF =  540.0f;  /// @brief <float> 画面縦ハーフサイズ
    static constexpr int   kHalfWidth   =     960;  /// @brief 画面横ハーフサイズ
    static constexpr int   kHalfHeight  =     540;  /// @brief 画面縦ハーフサイズ
#else
    static constexpr float kWidthF      = 1280.0f;  /// @brief <float> 画面横サイズ
    static constexpr float kHeightF     =  720.0f;  /// @brief <float> 画面縦サイズ
    static constexpr int   kWidth       =    1280;  /// @brief 画面横サイズ
    static constexpr int   kHeight      =     720;  /// @brief 画面縦サイズ

    static constexpr float kHalfWidthF  =  640.0f;  /// @brief <float> 画面横ハーフサイズ
    static constexpr float kHalfHeightF =  360.0f;  /// @brief <float> 画面縦ハーフサイズ
    static constexpr int   kHalfWidth   =     640;  /// @brief 画面横ハーフサイズ
    static constexpr int   kHalfHeight  =     360;  /// @brief 画面縦ハーフサイズ
#endif
    static constexpr int   kColorBit    =      16;  /// @brief カラービット
}