virtual RGBAnimation RGBTask::GetAnimation()
{
    printf("nothing to get");
    return 0;
};

void RGBAnimation RGBFlashTask::GetAnimation()
{
    printf("getting flash animation pointer");
    RGBAnimation* rgb_animation_ptr = new RGBFlashAnimation(color_from, color_to, time_on, time_off, num_repetitions, b_repeat);
    return rgb_animation_ptr;
};

void RGBAnimation RGBFadeTask::GetAnimation()
{
    printf("getting fade animation pointer");
    RGBAnimation* rgb_animation_ptr = new RGBFadeAnimation(color_from, color_to, time_duration, b_repeat);
    return rgb_animation_ptr;
};

virtual void RGBAnimation::Update()
{
    printf("nothing to update");
};

void RGBFlashAnimation::Update()
{
    printf("updating flash");
};

void RGBFadeAnimation::Update()
{
    printf("updating fade");
};