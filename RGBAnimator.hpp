#ifndef RGBANIMATOR_H_
#define RGBANIMATOR_H_
//ms
#ifndef TIME_MIN_DELTA
    #define TIME_MIN_DELTA 20
#endif
//ms, maximum datatype of time_min_delta_ (uint8_t)
#ifndef TIME_MAX_DELTA
    #define TIME_MAX_DELTA 255
#endif


#include <list>


#define MIN( a, b ) (a < b) ? a : b
#define MAX( a, b ) (a > b) ? a : b
#define POSDIFF( a, b ) (a > b) ? a - b : b - a

class RGBAnimation;
class RGBTask;

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
    uint8_t maxDiff(color_t other)
    {
      return MAX(MAX(POSDIFF(R, other.R), POSDIFF(G, other.G)), POSDIFF(B, other.B));
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
    uint16_t time_duration;
    RGBAnimation* GetAnimation();
    RGBFadeTask();
    RGBFadeTask(color_t color_from_new, color_t color_to_new, uint16_t time_duration_new, bool b_repeat_new);
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
    RGBFlashTask(color_t color_from_new, color_t color_to_new, uint8_t time_on_new, uint8_t time_off_new, uint8_t num_repetitions_new, bool b_repeat_new);
    ~RGBFlashTask();
};


class RGBAnimation
{
  public:
    color_t color_current;
    unsigned int time_collective_delta_;
  //protected:
    //part of time uration thst was progressed already (necessary?)
    uint16_t time_progress_;
    uint8_t time_min_delta_;
    float fac_progress_;
    //uint8_t time_duration_; //
    //color_t color_from_; //
    //color_t color_to_;  //

    virtual uint8_t Update(uint8_t time_delta) = 0;
    virtual ~RGBAnimation();
};

class RGBFlashAnimation : public RGBAnimation//, public RGBFlashTask
{
  public:
    RGBFlashAnimation(RGBFlashTask* task_new);
    ~RGBFlashAnimation();
    uint8_t time_on;
    uint8_t time_off;
    uint8_t num_repetitions;
    RGBFlashTask *task;
    uint8_t Update(uint8_t time_delta);
};

class RGBFadeAnimation : public RGBAnimation//, public RGBFadeTask
{
  public:
    RGBFadeTask *task;
    RGBFadeAnimation(RGBFadeTask* task_new);
    ~RGBFadeAnimation();
    uint8_t Update(uint8_t time_delta);

    color_t fade();
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