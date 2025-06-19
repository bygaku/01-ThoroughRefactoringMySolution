#pragma once
#include <string>
#include "Core/Component/component.hpp"
#include "Core/Component/transform.hpp"

class Modeler final : public Component
{
public:
    Modeler() = delete;
    Modeler(GameObject& owner) noexcept;

    ~Modeler()        noexcept override;

    void Initialize() noexcept override;
    void Draw() const noexcept;

    /// @remark モデルハンドル操作
    int	 GetModelHandle() const                 noexcept;
    void SetModelHandle(std::string model_path) noexcept;
    void ResetModelHandle()                     noexcept;

    /// @brief 行列
    void SetMatrix(MATRIX mat)                  noexcept;

private:
    int model_handle_;

};