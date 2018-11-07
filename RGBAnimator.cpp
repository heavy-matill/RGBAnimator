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
    //printf("getting flash animation pointer\n");
    RGBAnimation* rgb_animation_ptr = new RGBFlashAnimation(this);
    return rgb_animation_ptr;
};

RGBAnimation* RGBFadeTask::GetAnimation()
{
    //printf("getting fade animation pointer\n");
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

RGBTask* RGBFlashAnimation::task_virt()
{
    return this->task;
}
RGBTask* RGBFadeAnimation::task_virt()
{
    return this->task;
}
uint8_t RGBFlashAnimation::update(uint8_t time_delta)
{
    //printf("updating flash\n");
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
    //printf("updating fade\n");

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
        //switch color
        task->color_to = task->color_from;
        task->color_from = color_current;
        if(num_rep_progress>0)
        {   
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

uint8_t RGBAnimator::animate(uint8_t time_delta)
{
    if(!b_running)
    {
        // return zero so that the user knows that there is nothing to wait for
        // but dont reset time_delta_next so you can continue after setting b_running to true
        return 0;
    }
    else
    {
        // if previous animation has not stopped
        if(time_delta_next == 0)
        {
            if(task_list.empty()&&!this->rgb_animation->task_virt()->b_repeat)
            {
                // queue is empty
                time_delta_next = 0;
                return 0;
            }
            else
            {
                // get new animation from queue
                this->get_animation();
            }
        }            
        // update current animation
        time_delta_next = rgb_animation->update(time_delta);
        return time_delta_next;
    }
}

void RGBAnimator::get_animation()
{
    // check if previous animation exists
    if(rgb_animation)
    {        
        // check if animation has to be repeated
        if(rgb_animation->task_virt()->b_repeat)
        {
            // queue task again
            this->queue_task(rgb_animation->task_virt());
            b_running=1;
        }
        else
        {
            delete rgb_animation->task_virt();
        }
        // delete old animation
        delete rgb_animation;
    }    
    // create new animation from task
    rgb_animation = this->pop_task_virt()->GetAnimation();
}

void RGBAnimator::queue_task(RGBTask* task)
{     
    this->task_list.push(task);  
}

RGBTask* RGBAnimator::pop_task_virt()
{
    RGBTask* rgb_task = this->task_list.pop();
    return  rgb_task;
}

color_t RGBAnimator::color_current()
{
    return rgb_animation->color_current;
}

void RGBAnimator::set_color(color_t color_to_new)
{
    rgb_animation->color_current = color_to_new;
}

void RGBAnimator::pause()
{
    b_running = false;
}
void RGBAnimator::stop()
{
    clear_list();
    //rgb_animation->time_progress_ = rgb_animation->task_virt()->time_duration;
    rgb_animation->task_virt()->b_repeat = false;
    //rgb_animation->num_rep_progress = 1.0;
    rgb_animation->color_current = color_t(0,0,0);
    time_delta_next = 0;
    pause();
}
void RGBAnimator::start()
{
    b_running = true;
    animate(0);
}

void RGBAnimator::clear_list()
{
    while(!task_list.empty())
    {
        task_list.pop();
    }    
}

void RGBAnimator::add_flash(color_t color_from_new, color_t color_to_new, uint16_t time_duration_new, uint8_t perc_on_new, uint8_t num_repetitions_new, bool b_repeat_new)
{
    RGBFlashTask* temp_task = new RGBFlashTask(color_from_new, color_to_new, time_duration_new, perc_on_new, num_repetitions_new, b_repeat_new);
    queue_task(temp_task); 
}

void RGBAnimator::add_fade(color_t color_from_new, color_t color_to_new, uint16_t time_duration_new, uint8_t num_repetitions_new, bool b_repeat_new)
{
    RGBFadeTask* temp_task = new RGBFadeTask(color_from_new, color_to_new, time_duration_new, num_repetitions_new, b_repeat_new);
    queue_task(temp_task); 
}

bool RGBAnimator::list_empty()
{
    return task_list.empty();
}

bool RGBAnimator::running()
{
    return b_running;
}

void RGBAnimator::process_data(uint8_t dat_char)
{
	if(!dat_count)
	{ //first received symbol
		switch(dat_char)
        {
			case 0x01: //new jump to color
				data[0] = 0x01;
				dat_count = 1;
				break;			
			case 0x02: //new slide to color
				data[0] = 0x02;
				dat_count = 1;
				break;             
            case 0x03: //new flash in color 
				data[0] = 0x03;
				dat_count = 1;           
				break;             
            case 0x04: //faster
				//Faster(100);
				//Serial.println("faster");
				break;
            case 0x05: //slower
				//Slower(100);
				break;
            case 0x06: //speedPercent
				dat_count = 1;
				break;
            case 0x07: //stop and clear
				stop();		
				clear_list();
				break;
            case 0x08: //pause
				pause();
				break;
            case 0x09: //resume
				start();			
				break;             
            case 0x0A: //immediately setColor
				data[0] = 0x0A;
				dat_count = 1;
				break;
			case 0x0B: //immediately slideColor
				data[0] = 0x0B;
				dat_count = 1;
				break;	
			case 0x0C: //set Brightness uint8_t 0-100;
				data[0] = 0x0C;
				dat_count = 1;
				break;	
			case 0x0D: //change Brightness int8_t -100..+100;
				data[0] = 0x0D;
				dat_count = 1;
				break;		
			case 0x0E: //white brightness
				data[0] = 0x0E;
				dat_count = 1;
				break;
		}
	}
	else
    {         
        data[dat_count] = dat_char;
        dat_count++;
		if(dat_count==4&&data[0]==0x0A) //set color
        {
            stop();
            set_color(color_t(data[1], data[2], data[3]));
			dat_count=0;
        }
        if(dat_count==4&&data[0]==0x0B) //fade now
        {
            color_t cur = color_current();
            stop();
            add_fade(cur, //color_from
                color_t(data[1], data[2], data[3]), //color_to
                2000, //time_duration
                1, //num_repetitions
                false); //b_repeat
            dat_count=0;
            start();
        }
        if(dat_count==4&&data[0]==0x01)//add jump to color
        {      
            add_flash(color_current(), //color_from
                color_t(data[1], data[2], data[3]), //color_to
                100, //time_duration
                0, //perc_on
                1, //num_repetitions
                false); //b_repeat
            dat_count = 0;
        }
        if(dat_count==11&&data[0]==0x02)//add fade to color
        {
            add_fade(color_t(data[1], data[2], data[3]), //color_from
                color_t(data[4], data[5], data[6]), //color_to
                (uint16_t)(data[7] << 8) + data[8], //time_duration
                (uint8_t)data[9], //num_repetitions
                (bool)data[10]); //b_repeat
            dat_count = 0;
        }
        if(dat_count==12&&data[0]==0x03)//add flash in color
        {
			//Serial.println("New Flash");
            add_flash(color_t(data[1], data[2], data[3]), //color_from
                color_t(data[4], data[5], data[6]), //color_to
                (uint16_t)(data[7] << 8) + data[8], //time_duration
                (uint8_t)data[9], //perc_on
                (uint8_t)data[10], //num_repetitions
                (bool)data[11]); //b_repeat
            dat_count = 0;
        }
        if(dat_count==2&&data[0]==0x06)
        {            
			//SpeedPercent(data[1]-100);
			dat_count=0;
        }
        if(dat_count==2&&data[0]==0x0C)
        {            
			//SetBrightness(data[1]);
			dat_count=0;
        }
        if(dat_count==2&&data[0]==0x0D)
        {            
			//ChangeBrightness(data[1]-100);
			dat_count=0;
        }
        if(dat_count==2&&data[0]==0x0E)
        {            
			//if(pwm_pin_w_)
			//	pwm_pin_w_ = data[1];
			dat_count=0;
        }
		if(dat_count>12)
			dat_count=0;
    }
}