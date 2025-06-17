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

    /// @brief 非同期でシーンの読み込みを開始
    void StartAsyncLoad(const std::string& target_scene_tag) noexcept;

    /// @brief 読み込み完了チェック
    bool IsLoadComplete()                              const noexcept;

    /// @brief 読み込み完了後の目的のシーンタグを取得
    const std::string& GetTargetSceneTag()             const noexcept;

private:
    std::unique_ptr<std::thread> load_thread_;
    mutable std::mutex           mutex_;

    /// @brief バックグラウンドで実行される読み込み処理
    void BackgroundLoadProcess()   noexcept;

    /// @brief スレッドの終了を待つ
    void WaitForThreadCompletion() noexcept;

private:
    bool        is_loading_;
    bool        is_complete_;
    std::string target_scene_tag_;
    int         background_graph_;

};