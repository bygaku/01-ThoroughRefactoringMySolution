#pragma once
#include "string"
#include "Core/Component/component.hpp"
#include "Core/Component/transform.hpp"

class Modeler final : public Component
{
public:
    Modeler() = delete;
    Modeler(GameObject& owner) noexcept
        : Component{ owner }
        , modelHandle_(-1)
    {
    };

    ~Modeler() noexcept;

    void Initialize() noexcept	override;
    void Draw()		  const		noexcept;

    /// @remark ÉnÉìÉhÉãëÄçÏ
    int	 GetModelHandle() const noexcept { return modelHandle_; }
    void SetModelHandle(std::string modelPath) noexcept;
    void ResetModelHandle() noexcept;

    /// @brief çsóÒ
    void SetMatrix(MATRIX mat) noexcept;

private:
    int	   modelHandle_;

};