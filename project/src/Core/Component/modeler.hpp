#pragma once
#include <string>
#include "Core/Component/component.hpp"
#include "Core/Component/transform.hpp"

class Modeler final : public Component
{
public:
    Modeler() = delete;
    Modeler(GameObject& owner) noexcept
        : Component{ owner }
        , model_handle_(-1)
    {
    };

    ~Modeler() noexcept;

    void Initialize() noexcept	override;
    void Draw()		  const		noexcept;

    /// @remark ÉnÉìÉhÉãëÄçÏ
    int	 GetModelHandle() const noexcept { return model_handle_; }
    void SetModelHandle(std::string model_path) noexcept;
    void ResetModelHandle()     noexcept;

    /// @brief çsóÒ
    void SetMatrix(MATRIX mat)  noexcept;

private:
    int model_handle_;

};