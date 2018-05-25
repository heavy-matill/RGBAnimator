typedef struct color
{
    uint8_t R;
    uint8_t G;
    uint8_t B;
} color_t;

class RGBAnimationTask
{
  public:
    color_t color_from;
    color_t color_to;
    bool b_repeat;
};

class RGBFadeTask : public RGBAnimationTask
{
  public:
    uint8_t time_duration;
};

class RGBFlashTask : public RGBAnimationTask
{
  public:
    uint8_t time_on;
    uint8_t time_off;
    uint8_t number_repetitions;
};

class RGBAnimator
{
    
};