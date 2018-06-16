#include "RGBAnimator.hpp"

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
RGBFadeTask::RGBFadeTask(color_t color_from_new, color_t color_to_new, uint16_t time_duration_new, bool b_repeat_new)
{
    color_from = color_from_new;
    color_to = color_to_new;
    time_duration = time_duration_new;
    b_repeat = b_repeat_new;

};    
RGBFadeTask::~RGBFadeTask()
{

};

RGBFlashTask::RGBFlashTask()
{

};
RGBFlashTask::RGBFlashTask(color_t color_from_new, color_t color_to_new, uint8_t time_on_new, uint8_t time_off_new, uint8_t num_repetitions_new, bool b_repeat_new)
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
    RGBAnimation* rgb_animation_ptr =  new RGBFlashAnimation(this);
    return rgb_animation_ptr;
};

RGBAnimation* RGBFadeTask::GetAnimation()
{
    printf("getting fade animation pointer\n");
    RGBAnimation* rgb_animation_ptr = new RGBFadeAnimation(this);
    return rgb_animation_ptr;
};

/*virtual void RGBAnimation::Update()
{
    printf("nothing to update");
};*/


RGBFlashAnimation::RGBFlashAnimation(RGBFlashTask* task_new)
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
    task = task_new;
    // Color hasn't changed
    // overload operartor!
    if (task->color_from==task->color_to) {
        return;
    }

    // Fade duration is smaller than TIME_MIN_DELTA
    if (task->time_duration <= TIME_MIN_DELTA) {
        color_current = task->color_to;
        return;
    }
    time_collective_delta_ = 0;
    time_progress_ = 0;
    fac_progress_ = 0;
    time_min_delta_ = MAX(TIME_MIN_DELTA,(uint8_t)((float)task->time_duration/(float)task->color_to.maxDiff(task->color_from)));
    // MIN TIME_MAX_DELTA ?
};
RGBFadeAnimation::~RGBFadeAnimation()
{

};

color_t RGBFadeAnimation::fade()
{   
    int color_diff_r = task->color_to.R - task->color_from.R;
    int color_diff_g = task->color_to.G - task->color_from.G;
    int color_diff_b = task->color_to.B - task->color_from.B;;
    return color_t(task->color_from.R+(int)(fac_progress_*color_diff_r), task->color_from.G+(int)(fac_progress_*color_diff_g), task->color_from.B+(int)(fac_progress_*color_diff_b));
};

//return time until next. if opened with time_delta == 0, update to next step.
// this way it can be used with interrupts
uint8_t RGBFadeAnimation::Update(uint8_t time_delta)
{
    printf("updating fade\n");

    // Animation finished already
    if(task->time_duration == 0)
        return 0;

    // Animation needs more time for the next frame to be displayed
    /*time_collective_delta_ += time_delta;
    if (time_collective_delta_ < time_min_delta_) { 
        return time_min_delta_ - time_collective_delta_;
    } else {
        time_progress_ += time_collective_delta_;
        time_collective_delta_ = 0;
    }*/
    time_progress_ += time_delta;

    // Get progress in percent since last update
    fac_progress_ = (float)time_progress_/ (float)task->time_duration;

    // If progress has reached 100%, set color to final target color
    if (fac_progress_ >= 1) {
        color_current = task->color_to;
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