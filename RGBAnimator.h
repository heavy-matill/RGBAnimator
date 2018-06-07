#ifndef RGBANIMATOR_H_
#define RGBANIMATOR_H_

#include <list>

class color_t
{
    public:
    uint8_t R,G,B;
    color_t(R,G,B);
    color_t fade(color_t c1, color_t c2, float progress)
    {   
        int color_diff_r = c2.R - c1.R;
        int color_diff_g = c2.G - c1.G;
        int color_diff_b = c2.B - c1.B;;
        return color_t(c1.R+round(f*color_diff_r),c1.G+round(f*color_diff_g), c1.B+round(f*color_diff_b));
    }
}

class RGBAnimation
{
  public:
    color_t color_current;
    virtual bool Update() = false;
    virtual ~RGBAnimation();
};

class RGBFlashAnimation : public RGBAnimation//, public RGBFlashTask
{
  public:
    uint8_t time_on;
    uint8_t time_off;
    uint8_t num_repetitions;
    RGBFlashAnimation(color_t color_from_new, color_t color_to_new, uint8_t time_on_new, uint8_t time_off_new, uint8_t num_repetitions_new, bool b_repeat_new);// = false);
    ~RGBFlashAnimation();
    bool Update();
};

class RGBFadeAnimation : public RGBAnimation//, public RGBFadeTask
{
  public:
    uint8_t time_duration;
    RGBFadeAnimation(color_t color_from_new, color_t color_to_new, uint8_t time_duration_new, bool b_repeat_new);// = false);
    ~RGBFadeAnimation();
    bool Update();

};


class RGBTask
{
  public:
    color_t color_from;
    color_t color_to;
    bool b_repeat;
    virtual RGBAnimation* GetAnimation() = 0;
    //virtual RGBTask();
    virtual ~RGBTask();
};

class RGBFadeTask : public RGBTask
{
  public:
    uint8_t time_duration;
    RGBAnimation* GetAnimation();
    RGBFadeTask();
    ~RGBFadeTask();
};

class RGBFlashTask : public RGBTask
{
  public:
    uint8_t time_on;
    uint8_t time_off;
    uint8_t num_repetitions;
    RGBAnimation* GetAnimation();
    RGBFlashTask();
    ~RGBFlashTask();
};

class RGBAnimator
{
  public:
    RGBAnimation* rgb_animation;
    uint8_t brightness;
    uint8_t speed;

    //void SetValue(color_t color_set);
    //void ChangeBrightness(int8_t change_brightness = 10);
    //void ChangeSpeed(int8_t change_speed = 10);
    //void Stop();

    //void PopAnimation();
    //void ClearList();
    //bool ListIsEmpty();

    //void DataAvailable(uint8_t datChar);
    //void ResetDatCount();
    std::list<RGBTask*> RGBTaskList;
};



#endif // RGBANIMATOR_H_