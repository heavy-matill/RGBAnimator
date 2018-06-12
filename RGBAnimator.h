#ifndef RGBANIMATOR_H_
#define RGBANIMATOR_H_

#ifndef TIME_MIN_DELTA
    #define TIME_MIN_DELTA 20 //ms
#endif

#include <list>

class color_t
{
    public:
    uint8_t R,G,B;
    color_t(){
    R = 0;
    G = 0;
    B = 0;
    };
    color_t(uint8_t r, uint8_t g, uint8_t b)
    {
     R = r;
     G = g;
     B = b; 
    };
    
    bool operator==(const color_t other)
    {
      return (R==other.R&G==other.G&B==other.B);
    };
};

class RGBAnimation
{
  public:
    color_t color_current;
    unsigned int time_collective_delta_;
  //protected:
    //part of time uration thst was progressed already (necessary?)
    uint8_t time_progress_;
    uint8_t time_duration_;
    float fac_progress_;
    color_t color_from_;
    color_t color_to_;

    virtual bool Update(uint8_t time_delta) = 0;
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
    bool Update(uint8_t time_delta);
};

class RGBFadeAnimation : public RGBAnimation//, public RGBFadeTask
{
  public:
    uint8_t time_duration;
    RGBFadeAnimation(color_t color_from_new, color_t color_to_new, uint8_t time_duration_new, bool b_repeat_new);// = false);
    ~RGBFadeAnimation();
    bool Update(uint8_t time_delta);

    color_t fade()
    {   
        int color_diff_r = color_to_.R - color_from_.R;
        int color_diff_g = color_to_.G - color_from_.G;
        int color_diff_b = color_to_.B - color_from_.B;;
        return color_t(color_from_.R+(int)(fac_progress_*color_diff_r),color_from_.G+(int)(fac_progress_*color_diff_g), color_from_.B+(int)(fac_progress_*color_diff_b));
    };

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