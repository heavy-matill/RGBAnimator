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
    printf("getting flash animation pointer");
    RGBAnimation* rgb_animation_ptr = new RGBFlashAnimation(color_from, color_to, time_on, time_off, num_repetitions, b_repeat);
    return rgb_animation_ptr;
};

RGBAnimation* RGBFadeTask::GetAnimation()
{
    printf("getting fade animation pointer");
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

void RGBFlashAnimation::Update()
{
    printf("updating flash");
};

void RGBFadeAnimation::Update()
{
    printf("updating fade");
};