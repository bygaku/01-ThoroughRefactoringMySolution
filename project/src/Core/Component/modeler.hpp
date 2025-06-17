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

    /// @remark �n���h������
    int	 GetModelHandle() const noexcept { return modelHandle_; }
    void SetModelHandle(std::string modelPath) noexcept;
    void ResetModelHandle() noexcept;

    /// @brief �s��
    void SetMatrix(MATRIX mat) noexcept;

private:
    int	   modelHandle_;

};