#include "DxLib.h"
#include <thread>
#include <chrono>
#include "Core/Scene/load.hpp"
#include "Core/SceneManager/scene_manager.hpp"
#include "Core/GameTime/game_time.hpp"
#include "Utility/utility_link.hpp"

LoadScene::LoadScene(SceneManager& manager) noexcept
    : Scene(SceneType.kLoad, manager)
    , load_thread_(nullptr)
    , is_loading_(false)
    , is_complete_(false)
    , background_graph_(-1)
{
    if (background_graph_ == -1) {
        background_graph_ = LoadGraph("Resource/Texture/load_debug.jpg");
    }
}

LoadScene::~LoadScene() noexcept
{
    WaitForThreadCompletion();
    if (background_graph_) {
        DeleteGraph(background_graph_);
    }
}

void LoadScene::Update() noexcept
{
    Scene::Update();
}

void LoadScene::LastUpdate() noexcept
{
    // 読み込み完了チェック
    bool should_change_scene = false;
    std::string target_tag;

    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (is_complete_) {
            should_change_scene = true;
            target_tag = target_scene_tag_;
            is_complete_ = false;
            is_loading_  = false;
        }
    }

    if (should_change_scene) {
        ChangeScene(target_tag);
    }
}

void LoadScene::Draw() const noexcept
{
    // 画面の基本情報取得
    int screen_width  = screen_data::kWidth;
    int screen_height = screen_data::kHeight;

    // 読み込み状態を安全に取得してステータス表示
    bool loading = false;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        loading = is_loading_;
    }

    if (loading) {
        DrawExtendGraph(0, 0, screen_width, screen_height, background_graph_, true);
    }
}

void LoadScene::StartAsyncLoad(const std::string& target_scene_tag) noexcept
{
    // 既に読み込み中の場合は何もしない
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (is_loading_) return;
        
        is_loading_       =             true;
        is_complete_      =            false;
        target_scene_tag_ = target_scene_tag;
    }

    // 前のスレッドが残っていれば安全に終了を待つ
    WaitForThreadCompletion();

    // 新しいスレッドで読み込み開始
    try {
        load_thread_ = std::make_unique<std::thread>(&LoadScene::BackgroundLoadProcess, this);
    }
    catch (const std::exception&) {
        // スレッド作成に失敗時フォールバック
        std::lock_guard<std::mutex> lock(mutex_);
        is_loading_  = false;
        is_complete_ = false;
    }
}

bool LoadScene::IsLoadComplete() const noexcept
{
    std::lock_guard<std::mutex> lock(mutex_);
    return is_complete_;
}

const std::string& LoadScene::GetTargetSceneTag() const noexcept
{
    std::lock_guard<std::mutex> lock(mutex_);
    return target_scene_tag_;
}

void LoadScene::BackgroundLoadProcess() noexcept
{
    try {
        std::string target_tag;
        // 安全に目的のシーンタグを取得
        {
            std::lock_guard<std::mutex> lock(mutex_);
            target_tag = target_scene_tag_;
        }

        // 初期化処理を実行
        auto target_scene = manager_.GetScene(target_tag);
        if (target_scene != nullptr) {
            target_scene->Initialize();
        }            
        
        // 最小表示時間を確保
        if (GetSceneTime() > 1.0f) {
        }

        // 読み込み完了フラグを設定
        {
            std::lock_guard<std::mutex> lock(mutex_);
            is_complete_ = true;
        }
    }
    catch (const std::exception&) {
        // エラーが発生した場合の処理
        std::lock_guard<std::mutex> lock(mutex_);
        is_loading_  = false;
        is_complete_ = false;
        // ログ出力やエラーハンドリングをここに追加可能
    }
    catch (...) {
        // 予期しないエラーの処理
        std::lock_guard<std::mutex> lock(mutex_);
        is_loading_  = false;
        is_complete_ = false;
    }
}

void LoadScene::WaitForThreadCompletion() noexcept
{
    if (load_thread_ && load_thread_->joinable()) {
        try {
            load_thread_->join();
        }
        catch (...) {
            /// HACK: join失敗時の処理
        }

        load_thread_.reset();
    }
}