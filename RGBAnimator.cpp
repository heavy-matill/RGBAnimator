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
RGBFadeTask::RGBFadeTask(color_t color_from_new, color_t color_to_new, uint16_t time_duration_new, uint8_t num_repetitions_new, bool b_repeat_new)
{
    color_from = color_from_new;
    color_to = color_to_new;
    time_duration = time_duration_new;
    num_repetitions = num_repetitions_new;
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
    color_from = color_from_new;
    color_to = color_to_new;
    time_on = time_on_new;
    time_off = time_off_new;
    num_repetitions = num_repetitions_new;
    b_repeat = b_repeat_new;
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
    task = task_new;
    num_rep_progress = task->num_repetitions;
    b_state_on = true;
    time_progress_ = 0;
};
RGBFlashAnimation::~RGBFlashAnimation()
{

};

uint8_t RGBFlashAnimation::Update(uint8_t time_delta)
{
    printf("updating flash\n");
    // what if multiple are skipped?
    //if (time_delta > (time_on+time_off)
    // skip reptitions?

    time_progress_ += time_delta;
    if (time_progress_ > (time_on+time_off))
    {
        //skip state?
    }
    if(b_state_on)
    {
        if(time_progress_>=time_on)
        {
            time_progress_ -= time_on;
            color_current = task->color_to;
            b_state_on = false;
            return time_off-time_progress_;
        }
        else
        {
            color_current = task->color_from;
            return time_on-time_progress_;
        }
    }
    else
    {
        if(time_progress_>=time_off)
        {
            num_rep_progress--;
            // finish flashing
            if(num_rep_progress==0)
            {
                return 0;
            }
                
            time_progress_ -= time_off;
            color_current = task->color_from;
            b_state_on = true;
            return time_on-time_progress_;
        }
        else
        {
            color_current = task->color_to;
            return time_on-time_progress_;
        }
    }
};

RGBFadeAnimation::RGBFadeAnimation(RGBFadeTask* task_new)
{
    task = task_new;
    num_rep_progress = task->num_repetitions;
    // Check if color hasn't changed
    /*if (task->color_from==task->color_to) {
        return 0;
    }*/

    // Fade duration is smaller than TIME_MIN_DELTA
    /*if (task->time_duration <= TIME_MIN_DELTA) {
        color_current = task->color_to;
        return;
    }*/
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
    if(num_rep_progress==0||task->time_duration == 0)
        return 0;
    time_progress_ += time_delta;

    // Get progress in percent since last update
    fac_progress_ = (float)time_progress_/ (float)task->time_duration;

    // If progress has reached 100%, set color to final target color
    if (fac_progress_ >= 1) {
        color_current = task->color_to;
        //reduce repetitions
        num_rep_progress--;
        if(num_rep_progress>0)
        {   
            //switch color
            task->color_to = task->color_from;
            task->color_from = color_current;
            //reset progress
            if(time_progress_>task->time_duration)
            {
                time_progress_ -= task->time_duration;
            }
            else
            {
                time_progress_ = 0;
            }
        }
        else
        {
            return 0;
        }
    }
    else
    {
        // Calculate color  how it should be according to progress
        color_current = fade();        
    }
    return time_min_delta_;
};