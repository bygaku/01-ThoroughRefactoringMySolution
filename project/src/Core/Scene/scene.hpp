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

    /// @brief �V�[���̏�Ԃ��m�F
    virtual void CheckSceneState()														 noexcept;

    /// @brief �V�[���ɃI�u�W�F�N�g��o�^
    void RegisterSceneObject(std::shared_ptr<GameObject> object, const std::string& tag) noexcept;

    /// @brief �o�^���ꂽ�I�u�W�F�N�g���擾
    std::shared_ptr<GameObject> GetSceneObject(const std::string& tag)					 noexcept;

    /// @brief ���ׂẴV�[���ɓK������I�u�W�F�N�g��ǉ�
    void DontDestroyOnLoad(std::shared_ptr<GameObject> object, std::string tag)			 noexcept;

    /// @brief �j�󂵂Ȃ��I�u�W�F�N�g��ǉ�
    std::unordered_map<std::string, std::shared_ptr<GameObject>> GetDontDestroyObjects() noexcept;

    /// @brief SceneManager��changeSceneImmediate���ďo��.
    /// HACK: ���ꏈ���Ȃ̂ŁALoad����������͏������ƁB
    void ChangeSceneImmediate(std::string tag)											 noexcept;

protected:
    SceneManager& manager_;
    
    /// @brief �V�[����؂�ւ�
    void ChangeScene(std::string tag);

    /// @brief �V�[���̃^�C�����擾
    const float& GetSceneTime() const noexcept { return time_; }

private:
    std::string type_;
    std::unordered_map<std::string, std::shared_ptr<GameObject>> objects_;
    std::unordered_map<std::string, std::shared_ptr<GameObject>> dont_destroy_objects_;

    float time_;

};
