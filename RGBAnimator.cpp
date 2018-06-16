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
RGBFlashTask::RGBFlashTask(color_t color_from_new, color_t color_to_new, uint16_t time_duration_new, uint8_t perc_on_new, uint8_t num_repetitions_new, bool b_repeat_new)
{
    color_from = color_from_new;
    color_to = color_to_new;
    time_duration = time_duration_new;
    perc_on = MIN(100,perc_on_new);
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

/*virtual void RGBAnimation::update()
{
    printf("nothing to update");
};*/


RGBFlashAnimation::RGBFlashAnimation(RGBFlashTask* task_new)
{
    task = task_new;
    num_rep_progress = task->num_repetitions;
    time_progress_ = 0;
    fac_progress_ = 0;
};
RGBFlashAnimation::~RGBFlashAnimation()
{

};

uint8_t RGBFlashAnimation::update(uint8_t time_delta)
{
    printf("updating flash\n");
    // skip reptitions?
    if(time_delta>task->time_duration)
    {
        uint8_t delta_rep = time_delta/task->time_duration;
        num_rep_progress -= MIN(num_rep_progress, time_delta/task->time_duration);
        time_delta -= task->time_duration*delta_rep;
    }
    time_progress_ += time_delta;
    fac_progress_ = (float)time_progress_/(float)task->time_duration;

    if(fac_progress_>=1.0)
    {        
            num_rep_progress--;
        // Flash finished
        if(num_rep_progress > 0)
        {
            // reset progress for next repetition
            time_progress_ -= task->time_duration;
        }
        else
        {
            // no more flashes, stop
            return 0;
        }
    }
    if(fac_progress_*100 > task->perc_on) 
    {
        color_current = task->color_to;
        return LIM(task->time_duration-time_progress_);
        
    }
    else
    {
        color_current = task->color_from;
        return LIM((float)task->time_duration/100.0*task->perc_on-time_progress_);
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
    time_min_delta_ = LIM((uint8_t)((float)task->time_duration/(float)task->color_to.maxDiff(task->color_from)));
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
uint8_t RGBFadeAnimation::update(uint8_t time_delta)
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

RGBAnimator::animate(uint8_t time_delta)
{
    if(!b_running)
    {
        if(RGBTaskList.empty())
        {
            time_delta_next = 0;
        }
        else
        {
            // hier ein while time_delta_next == 0
            // fetch new
            if(time_delta_next > 0)
            {
                // already active animation going opened
                time_delta_next = rgb_animation->update(time_delta);
            }
            else
            {
                // get new animation from queue
                rgb_animation = RGBTaskList.begin()->GetAnimation();
                // similar code as above case...?
                time_delta_next = rgb_animation->update(time_delta);
            }

            if(time_delta_next==0)
            {
                // after current update still zero means animation has finished
                // dispose of current
                // fetch new!
            }
        }
    }
}