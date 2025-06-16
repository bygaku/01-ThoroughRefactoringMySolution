#include "DxLib.h"
#include "Utility/utility_link.hpp"
#include "Core/GameTime/game_time.hpp"

bool  GameTime::instantiated_ = false;
float GameTime::delta_time_   = 0.0f;

GameTime::GameTime() noexcept
    : time_(0)
    , fps_check_time_(0)
    , fixed_update_check_time_(0)
    , time_since_start_up_(0.0f)
    , fps_(0)
    , fps_counter_(0)
    , last_fixed_frame_count_(0)
    , fps_target_(60)
    , seconds_per_frame_(1.0 / fps_target_)
    , frames_(0)
{
    ASSERT(instantiated_ == 0, "already instantiated.");
    instantiated_ = true;

    // �N�����̃V�X�e�����Ԃ��擾
    time_                    = GetNowHiPerformanceCount();
    fps_check_time_          = GetNowHiPerformanceCount();
    fixed_update_check_time_ = GetNowHiPerformanceCount();
}

GameTime::~GameTime() noexcept
{
    instantiated_ = false;
}

void GameTime::IncrementFrame() noexcept
{ 
    // �t���[���I�����̃V�X�e������
    const LONGLONG now = GetNowHiPerformanceCount();

    // �f���^�^�C���Z�o
    delta_time_        = (now - time_) / 1000000.0f;
    time_              =  now;

    // 1�b�o�߂���ԂɎ��s���ꂽ���C�����[�v�̉񐔂� FPS �Ƃ���
    ++fps_counter_;

    if (now - fps_check_time_ > 1000000.0f) {
        fps_            = fps_counter_;
        fps_counter_    = 0;
        fps_check_time_ = now;
    }

    // delta_time_ �𖈃t���[�������āA�b�Z�o
    time_since_start_up_ += delta_time_;
}

int GameTime::UpdateFixedFrameCount() noexcept
{
    int frame_counts = 0;

    while (fixed_update_check_time_ < time_) {
        fixed_update_check_time_ += 20000;
        frame_counts++;
    }

    last_fixed_frame_count_ = frame_counts;
    
    return frame_counts;
}

double GameTime::GetCurrentFrameTime() const noexcept
{
    // 1�t���[���̎��Ԃ��Z�o���ĕԂ�
    const LONGLONG now = GetNowHiPerformanceCount();
    return (now - time_) / 1000000.0;
}

void GameTime::SleepForNextFrame() const noexcept
{
    auto frametime = GetCurrentFrameTime();

    if (GetSecondsPerFrame() > frametime) {
        WaitTimer(static_cast<int>(GetSecondsPerFrame() - frametime) * 1000);
    }
}

void GameTime::SetFpsTarget(int target) noexcept
{
    if (target < 0) {
        fps_target_        = target;
        seconds_per_frame_ = 0;

        return;
    }

    fps_target_        = target;
    seconds_per_frame_ = 1.0 / fps_target_;
}
