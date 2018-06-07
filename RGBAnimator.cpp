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
RGBFadeAnimation::RGBFadeAnimation(color_t color_from_new, color_t color_to_new, uint8_t time_duration_new, bool b_repeat_new)
{

};
RGBFadeAnimation::~RGBFadeAnimation()
{

};

RGBFlashAnimation::RGBFlashAnimation(color_t color_from, color_t color_to, uint8_t time_on, uint8_t time_off, uint8_t num_repetitions, bool b_repeat)
{

};
RGBFlashAnimation::~RGBFlashAnimation()
{

};

bool RGBFlashAnimation::Update(unsigned int delta_time)
{
    printf("updating flash\n");

    // Animation finished already
    if(time_duration == 0)
        return false;

    // Animation needs more time for the next frame to be displayed
    if (delta_time < interval_)
        return true;

    // Get progress in percent since last update
    float percent = (float)time_diff / (float)duration_;
    percent_done_ += percent;

    // If progress has reached 100%, set color to final target color
    if (percent >= 1) {
        StopFade();
        SetValue(target_r_, target_g_, target_b_);
        return false;
    }

    // Calculate color  how it should be according to progress
    color_t cur_color = color_t.fade(start_color, end_color, progress);

    SetValue(value_r_ + increment_r, value_g_ + increment_g, value_b_ + increment_b);

    // Update time and finish
    duration_ -= time_diff;
    last_step_time_ = millis();
    return true;
};

bool RGBFadeAnimation::Update()
{
    printf("updating fade\n");
    return false;
};