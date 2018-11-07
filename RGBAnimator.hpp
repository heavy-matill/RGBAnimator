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

#define MIN( a, b ) ((a < b) ? a : b)
#define MAX( a, b ) ((a > b) ? a : b)
#define LIM( t ) (MAX(TIME_MIN_DELTA,MIN(TIME_MAX_DELTA, t )))
#define POSDIFF( a, b ) ((a > b) ? a - b : b - a)

#include <stdint.h>
#include <stddef.h>

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
    uint8_t num_repetitions;
    bool b_repeat;
    uint16_t time_duration;
    virtual RGBAnimation* GetAnimation() = 0;
    //virtual RGBTask_virt();
    virtual ~RGBTask();
};

class RGBFadeTask : public RGBTask
{
  public:
    RGBAnimation* GetAnimation();
    RGBFadeTask();
    RGBFadeTask(color_t color_from_new, color_t color_to_new, uint16_t time_duration_new, uint8_t num_repetitions_new, bool b_repeat_new);
    ~RGBFadeTask();
};

class RGBFlashTask : public RGBTask
{
  public:
    uint8_t perc_on;
    RGBAnimation* GetAnimation();
    RGBFlashTask();
    RGBFlashTask(color_t color_from_new, color_t color_to_new, uint16_t time_duration_new, uint8_t perc_on_new, uint8_t num_repetitions_new, bool b_repeat_new);
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
    uint8_t num_rep_progress;
    float fac_progress_;
    virtual RGBTask* task_virt() = 0;
    //uint8_t time_duration_; //
    //color_t color_from_; //
    //color_t color_to_;  //

    virtual uint8_t update(uint8_t time_delta) = 0;
    virtual ~RGBAnimation();
};

class RGBFlashAnimation : public RGBAnimation//, public RGBFlashTask
{
  public:
    RGBFlashAnimation(RGBFlashTask* task_new);
    ~RGBFlashAnimation();
    uint8_t num_repetitions;
    RGBFlashTask *task;
    RGBTask* task_virt();
    uint8_t update(uint8_t time_delta);
};

class RGBFadeAnimation : public RGBAnimation//, public RGBFadeTask
{
  public:
    uint8_t time_min_delta_;
    RGBFadeTask *task;
    RGBTask* task_virt();
    RGBFadeAnimation(RGBFadeTask* task_new);
    ~RGBFadeAnimation();
    uint8_t update(uint8_t time_delta);

    color_t fade();
};

struct RGBTaskNode
{
	RGBTask *task;
	RGBTaskNode *next;
  RGBTaskNode()
  {
    task = NULL;
    next = NULL;
  }
};	

class RGBTaskList
{
	private:
  
	public:
	RGBTaskNode *head, *tail;
	RGBTaskList()
	{
		head=NULL;
		tail=NULL;
	}
	void push(RGBTask *task)
	{
		RGBTaskNode *temp=new RGBTaskNode();
		temp->task=task;
    if(head==NULL)
    {
      head = temp;
      tail = temp;
    }
    else
    {
      tail->next = temp;
      tail = temp;
    }
	}
	RGBTask *pop()
	{
		RGBTaskNode *temp=new RGBTaskNode();
		temp=head;
		head=head->next;
		RGBTask *task = temp->task;
		delete temp;
		return task;
	}
  bool empty()
	{
		return head==NULL;
	}
  int size()
	{
    RGBTaskNode *temp=head;
    int count = 0;
    if(head!=NULL)
    {
      count++;
      while(temp!=tail)
      {
        count++;
        temp=temp->next;
      }
    }
    return count;
	}
};	

class RGBAnimator
{
  private:  
    bool b_running = true;
    uint8_t data[11];
    uint8_t dat_count;
  public:
    RGBAnimation* rgb_animation;
    uint8_t brightness;
    uint8_t speed;
    uint8_t animate(uint8_t time_delta);
    void get_animation();
    void queue_task(RGBTask *task);
    RGBTask* pop_task_virt();
    uint8_t time_delta_next;
    color_t color_current();

    void set_color(color_t color_to_new);
    //void ChangeBrightness(int8_t change_brightness = 10);
    //void ChangeSpeed(int8_t change_speed = 10);
    void stop();
    void pause();
    void start();

    void add_flash(color_t color_from_new, color_t color_to_new, uint16_t time_duration_new, uint8_t perc_on_new, uint8_t num_repetitions_new, bool b_repeat_new);
    void add_fade(color_t color_from_new, color_t color_to_new, uint16_t time_duration_new, uint8_t num_repetitions_new, bool b_repeat_new);
    //void PopAnimation();
    void clear_list();
    bool list_empty();
    bool running();

    void process_data(uint8_t datChar);

    //void DataAvailable(uint8_t datChar);
    //void ResetDatCount();
    RGBTaskList task_list;
};

#endif // RGBANIMATOR_H_