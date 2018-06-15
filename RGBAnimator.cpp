#include "RGBAnimator.h"

/*virtual RGBAnimation RGBTask::GetAnimation()
{
    printf("nothing to get");
    return 0;
};*/

RGBTask::~RGBTask()
{

};

RGBFadeTask::RGBFadeTask()
{

};
RGBFadeTask::RGBFadeTask(color_t color_from_new, color_t color_to_new, uint8_t time_on_new, uint8_t time_off_new, uint8_t num_repetitions_new, bool b_repeat_new)
{

};    
RGBFadeTask::~RGBFadeTask()
{

};

RGBFlashTask::RGBFlashTask()
{

};
RGBFlashTask::~RGBFlashTask()
{

};

RGBAnimation::~RGBAnimation()
{
};

RGBAnimation* RGBFlashTask::GetAnimation()
{
    printf("getting flash animation pointer\n");
    RGBAnimation* rgb_animation_ptr = new RGBFlashAnimation(color_from, color_to, time_on, time_off, num_repetitions, b_repeat);
    return rgb_animation_ptr;
};

RGBAnimation* RGBFadeTask::GetAnimation()
{
    printf("getting fade animation pointer\n");
    RGBAnimation* rgb_animation_ptr = new RGBFadeAnimation(color_from, color_to, time_duration, b_repeat);
    return rgb_animation_ptr;
};

/*virtual void RGBAnimation::Update()
{
    printf("nothing to update");
};*/


RGBFlashAnimation::RGBFlashAnimation(color_t color_from, color_t color_to, uint8_t time_on, uint8_t time_off, uint8_t num_repetitions, bool b_repeat)
{

};
RGBFlashAnimation::~RGBFlashAnimation()
{

};

uint8_t RGBFlashAnimation::Update(uint8_t time_delta)
{
    printf("updating flash\n");
    return true;
};

RGBFadeAnimation::RGBFadeAnimation(RGBFadeTask* task_new)
{
    task_ = task_new;
    // Color hasn't changed
    // overload operartor!
    if (task_->color_from_new==task_->color_to_new) {
        return;
    }

    // Fade duration is smaller than TIME_MIN_DELTA
    if (time_duration_new <= TIME_MIN_DELTA) {
        color_current = color_to_new;
        return;
    }
    time_collective_delta_ = 0;
    time_progress_ = 0;
    time_duration = time_duration_new;
    fac_progress_ = 0;
    color_from_ = color_from_new;
    color_to_ = color_to_new;
    time_min_delta_ = MIN(TIME_MAX_DELTA,(MAX(TIME_MIN_DELTA, time_duration_/color_to_.maxDiff(color_from_))));

};
RGBFadeAnimation::~RGBFadeAnimation()
{

};

//return time until next. if opened with time_delta == 0, update to next step.
// this way it can be used with interrupts
uint8_t RGBFadeAnimation::Update(uint8_t time_delta)
{
    printf("updating fade\n");

    // Animation finished already
    if(time_duration == 0)
        return 0;

    // Animation needs more time for the next frame to be displayed
    time_collective_delta_ += time_delta;
    if (time_collective_delta_ < time_min_delta_)    
        return time_min_delta_ - time_collective_delta_;
    else
        time_progress_ += time_collective_delta_;
        time_collective_delta_ = 0;

    // Get progress in percent since last update
    float fac_progress_ = (float)time_progress_ / (float)time_duration_;

    // If progress has reached 100%, set color to final target color
    if (fac_progress_ >= 1) {
        color_current = color_to_;
        //StopFade();
        return 0;
    }

    // Calculate color  how it should be according to progress
    color_current = fade();

    // Update time and finish
    //duration_ -= time_diff;
    //last_step_time_ = millis();
    return time_min_delta_;
};