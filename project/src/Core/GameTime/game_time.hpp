#pragma once
#include <memory>

class GameTime final
{
public:	
    GameTime()  noexcept;
    ~GameTime() noexcept;

    /// @brief �t���[���J�E���g�� +1
    /// @remark ���t���[�������O�ɌĂяo��
    void IncrementFrame()                             noexcept;

    /// @brief �Œ�t���[���̍X�V�񐔂��X�V���ĕԂ�
    int UpdateFixedFrameCount()                       noexcept;

    /// @brief ���݂�1�t���[���������� (seconds)
    [[nodiscard]] double GetCurrentFrameTime()  const noexcept;

    /// @brief fps_target_ �Ƃ��Ďw�肳��Ă��鎞�Ԃ܂ő҂�
    void SleepForNextFrame()                    const noexcept;

    /// @brief fps �������w��
    void SetFpsTarget(int target)                     noexcept;

    /// @brief 2�O��1�O�̃t���[���Ԃ̎��ԍ� (seconds)
    static const float& GetDeltaTime() noexcept { return delta_time_; }

private:
#pragma region private���\�b�h
    /// @brief ���݂̃t���[���̎���
    [[nodiscard]] const LONGLONG& GetTime()                 const noexcept { return time_; }

    /// @brief �Ō�� fps ���X�V�������� (nanoseconds)
    [[nodiscard]] const LONGLONG& GetLastCheckedTime()      const noexcept { return fps_check_time_; }

    /// @brief �N������̎��� (seconds)
    [[nodiscard]] const float&    GetTimeFromBoot()         const noexcept { return time_since_start_up_; }

    /// @brief ���݂� frames per second
    [[nodiscard]] const int&      GetFps()                  const noexcept { return fps_; };

    /// @brief �Ō�� fps �X�V����̃t���[���J�E���g
    [[nodiscard]] const int&      GetFpsCounter()           const noexcept { return fps_counter_; };

    /// @brief ���݂� fps ����
    [[nodiscard]] const int&      GetFpsTarget()            const noexcept { return fps_target_; };

    /// @brief ���݂� fps �����ɂ��t���[���Ԃ̕b�� (seconds)
    [[nodiscard]] const double&   GetSecondsPerFrame()      const noexcept { return seconds_per_frame_; };

    /// @brief �Ō�̌Œ�Ԋu�X�V�̉�
    [[nodiscard]] const int&      GetLastFixedFrameCount()  const noexcept { return last_fixed_frame_count_; };

    /// @brief �N�����Ă���̃t���[����
    [[nodiscard]] const UINT&     GetFrames()               const noexcept { return frames_; };
#pragma endregion

private:
    static bool  instantiated_;
    static float delta_time_;

private:
    LONGLONG time_;
    LONGLONG fps_check_time_;
    LONGLONG fixed_update_check_time_;
    float    time_since_start_up_;

    int      fps_;
    int      fps_target_;
    int      fps_counter_;
    double   seconds_per_frame_;
    int      last_fixed_frame_count_;

    unsigned int frames_;

};