#pragma once
#include "Core/Scene/scene.hpp"
#include <thread>
#include <mutex>
#include <memory>

class LoadScene final : public Scene
{
public:
    LoadScene(SceneManager& manager) noexcept;
	~LoadScene()            noexcept override;

	void Update()           noexcept override;
	void Draw()       const noexcept override;
	void LastUpdate()       noexcept override;

    /// @brief �񓯊��ŃV�[���̓ǂݍ��݂��J�n
    void StartAsyncLoad(const std::string& target_scene_tag) noexcept;

    /// @brief �ǂݍ��݊����`�F�b�N
    bool IsLoadComplete()                              const noexcept;

    /// @brief �ǂݍ��݊�����̖ړI�̃V�[���^�O���擾
    const std::string& GetTargetSceneTag()             const noexcept;

private:
    std::unique_ptr<std::thread> load_thread_;
    mutable std::mutex           mutex_;

    /// @brief �o�b�N�O���E���h�Ŏ��s�����ǂݍ��ݏ���
    void BackgroundLoadProcess()   noexcept;

    /// @brief �X���b�h�̏I����҂�
    void WaitForThreadCompletion() noexcept;

private:
    bool        is_loading_;
    bool        is_complete_;
    std::string target_scene_tag_;
    int         background_graph_;

};