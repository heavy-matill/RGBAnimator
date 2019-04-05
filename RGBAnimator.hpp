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
    bool b_fade;
    color_t color_1;
    color_t color_2;
    uint8_t num_repetitions;
    bool b_repeat;
    uint16_t time_1;
    uint16_t time_2;

    RGBAnimation* GetAnimation();
    RGBTask();
    RGBTask(bool b_fade_new, color_t color_1_new, color_t color_2_new, uint16_t time_1_new, uint16_t time_2_new, uint8_t num_repetitions_new, bool b_repeat_new);
    ~RGBTask();

    uint16_t time_sum();
};

class RGBAnimation
{
  public:
    color_t color_current;
    unsigned int time_collective_delta_;
    uint16_t time_left;   
    uint16_t time_1;   
    uint16_t time_2;   
    uint8_t num_rep_progress;
    uint8_t time_min_delta;
    float fac_progress;
    bool b_fade;

    RGBTask *task;

    RGBAnimation(RGBTask *task_new);
    ~RGBAnimation();

    uint8_t update(uint8_t time_delta);
    color_t fade(float fac_progress);
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
    bool b_running;
    uint8_t data[12];
    uint8_t dat_count;
    
    uint8_t brightness;
    uint8_t speed;

  public:
    RGBAnimation* rgb_animation = 0;
    uint8_t animate(uint8_t time_delta);
    void get_animation();
    void queue_task(RGBTask *task);
    RGBTask* pop_task_virt();
    uint8_t time_delta_next = 0;
    color_t get_color_current();

    void set_color(color_t color_2_new);
    //void ChangeBrightness(int8_t change_brightness = 10);
    //void ChangeSpeed(int8_t change_speed = 10);
    void stop();
    void pause();
    void start();
    
    void set_brightness(uint8_t brightness_new);
    void set_speed(uint8_t speed_new);

    void add_flash(color_t color_1_new, color_t color_2_new, uint16_t time_1_new, uint16_t time_2_new, uint8_t num_repetitions_new, bool b_repeat_new);
    void add_fade(color_t color_1_new, color_t color_2_new, uint16_t time_1_new, uint16_t time_2_new, uint8_t num_repetitions_new, bool b_repeat_new);
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