#pragma once
#include "string"
#include "Component/component.hpp"
#include "Component/transform.hpp"

class Modeler final : public Component
{
public:
    Modeler() = delete;
    explicit Modeler(GameObject& owner) noexcept
        : Component{ owner }
        , modelHandle_(-1)
    {
    };

    ~Modeler() noexcept;

    void initialize() noexcept	override;
    void draw()		  const		noexcept;

    /// @remark ÉnÉìÉhÉãëÄçÏ
    int	 getModelHandle() const noexcept { return modelHandle_; }
    void setModelHandle(std::string modelPath) noexcept;
    void resetModelHandle() noexcept;

    /// @brief çsóÒ
    void setMatrix(MATRIX mat) noexcept;

private:
    int	   modelHandle_;

};