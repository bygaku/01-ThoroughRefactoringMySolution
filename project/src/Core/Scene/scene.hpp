#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include "Core/Scene/Data/scene_type.hpp"
#include "Object/game_object.hpp"

class SceneManager;

class Scene abstract
{
public:
    Scene() = delete;
    Scene(const std::string& tag, SceneManager& manager) noexcept
        : manager_(manager)
        , type_(tag)
        , time_(0.0f)
    {
    };

    virtual	~Scene()			   noexcept = default;

    virtual void Initialize()	   noexcept;
    virtual void Update()		   noexcept;
    virtual void Draw()		 const noexcept;
    virtual void Debug()	 const noexcept;
    virtual void LastUpdate()	   noexcept;

    /// @brief シーンの状態を確認
    virtual void CheckSceneState()														 noexcept;

    /// @brief シーンにオブジェクトを登録
    void RegisterSceneObject(std::shared_ptr<GameObject> object, const std::string& tag) noexcept;

    /// @brief 登録されたオブジェクトを取得
    std::shared_ptr<GameObject> GetSceneObject(const std::string& tag)					 noexcept;

    /// @brief すべてのシーンに適応するオブジェクトを追加
    void DontDestroyOnLoad(std::shared_ptr<GameObject> object, std::string tag)			 noexcept;

    /// @brief 破壊しないオブジェクトを追加
    std::unordered_map<std::string, std::shared_ptr<GameObject>> GetDontDestroyObjects() noexcept;

    /// @brief SceneManagerのchangeSceneImmediateを呼出し.
    /// HACK: 特殊処理なので、Load実装完了後は消すこと。
    void ChangeSceneImmediate(std::string tag)											 noexcept;

protected:
    SceneManager& manager_;
    
    /// @brief シーンを切り替え
    void ChangeScene(std::string tag);

    /// @brief シーンのタイムを取得
    const float& GetSceneTime() const noexcept { return time_; }

private:
    std::string type_;
    std::unordered_map<std::string, std::shared_ptr<GameObject>> objects_;
    std::unordered_map<std::string, std::shared_ptr<GameObject>> dont_destroy_objects_;

    float time_;

};
