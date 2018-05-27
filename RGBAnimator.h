#ifndef RGBANIMATOR_H_
#define RGBANIMATOR_H_

typedef struct color
{
    uint8_t R;
    uint8_t G;
    uint8_t B;
} color_t;

class RGBTask
{
  public:
    color_t color_from;
    color_t color_to;
    bool b_repeat;
    virtual RGBAnimation* GetAnimation();
};

class RGBFadeTask : public RGBTask
{
  public:
    uint8_t time_duration;
    RGBAnimation* GetAnimation();
};

class RGBFlashTask : public RGBTask
{
  public:
    uint8_t time_on;
    uint8_t time_off;
    uint8_t num_repetitions;
    RGBAnimation* GetAnimation();
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

  private:
    std::list<RGBAnimationTask *, std::pmr::polymorphic_allocator<T>> RGBTaskList_;
};

class RGBAnimation
{
  public:
    color_t color_current;
    virtual void Update();
};

class RGBFlasher : public RGBAnimation, public RGBFlashTask
{
  public:
    RGBFlasher(color_t color_from_new, color_t color_to_new, uint8_t time_on_new, uint8_t time_off_new, uint8_t num_repetitions_new, bool b_repeat_new = false)
    //void Update();
};

class RGBFader : public RGBAnimation, public RGBFadeTask
{
  public:
    RGBFader(color_t color_from_new, color_t color_to_new, uint8_t time_new, bool b_repeat_new = false)
    //void Update();
};

#endif // RGBANIMATOR_H_