#include "RGBAnimator.hpp"

RGBTask::~RGBTask()
{

};

RGBTask::RGBTask()
{

};
RGBTask::RGBTask(bool b_fade_new, color_t color_1_new, color_t color_2_new, uint16_t time_1_new, uint16_t time_2_new, uint8_t num_repetitions_new, bool b_repeat_new)
{
    b_fade = b_fade_new;
    color_1 = color_1_new;
    color_2 = color_2_new;
    time_1 = time_1_new;
    time_2 = time_2_new;
    num_repetitions = num_repetitions_new;
    b_repeat = b_repeat_new;
};  

RGBAnimation::~RGBAnimation()
{

};

RGBAnimation* RGBTask::GetAnimation()
{
    RGBAnimation* rgb_animation_ptr = new RGBAnimation(this);
    return rgb_animation_ptr;
};

uint16_t RGBTask::time_sum()
{
    return this->time_1 + this->time_2;
};

RGBAnimation::RGBAnimation(RGBTask *task_new)
{
    task = task_new;
    num_rep_progress = task->num_repetitions;
    time_left = task->time_1 + task->time_2;
    time_min_delta = LIM((uint8_t)((float)task->time_2 / (float)task->color_2.maxDiff(task->color_1)));
};

uint8_t RGBAnimation::update(uint8_t time_delta, uint8_t current_speed) {
    // adjust times to speed
    uint16_t time_delta_speeded = time_delta * current_speed / 16;
    uint8_t time_min_delta_speeded = LIM((uint16_t) time_min_delta * current_speed / 16);
    //  check whether time_delta finishes this animation
    while(time_left < time_delta_speeded) {
        // this repetition of animation is finished
        if(num_rep_progress > 0)
        {   
            num_rep_progress--;
            // switch task colors
            if (task->b_fade){
                color_current = task->color_2;
                task->color_2 = task->color_1;
                task->color_1 = color_current;
            }
            // reduce time_delta for next repetition
            time_delta_speeded -= time_left;
            // reset time_left for next repetition
            time_left = task->time_sum();
        } else {
            // switch task colors back if task is going to be repeated and has odd number of repetitions
            if (task->b_fade && task->b_repeat && task->num_repetitions%2) {
                color_current = task->color_2;
                task->color_2 = task->color_1;
                task->color_1 = color_current;
            }
            // no more repetitions to be done, stop
            return 0;
        }
    }

    // reduce time_left by time_delta to show the animation progress in this repetition
    time_left -= time_delta_speeded;

    // decide which color to display
    if (time_left > task->time_2) {
        // first part of animation, set color accordingly
        if (task->b_fade) {
            // Calculate color how it should be according to progress
            color_current = fade((float)(time_left - task->time_2) / (float) task->time_1);            
            return time_min_delta_speeded;   
        } else {
            // set color
            color_current = task->color_1;
            return LIM(time_left - task->time_2);
        }
    } else {
        // second part of animation, set color accordingly        
        color_current = task->color_2;
        return LIM(task->time_2 - time_left);
    }
};

color_t RGBAnimation::fade(float fac_progress)
{   
    uint8_t color_r_new = fac_progress * task->color_1.R + (1 - fac_progress) * task->color_2.R;
    uint8_t color_g_new = fac_progress * task->color_1.G + (1 - fac_progress) * task->color_2.G;
    uint8_t color_b_new = fac_progress * task->color_1.B + (1 - fac_progress) * task->color_2.B;
    return color_t(color_r_new, color_g_new, color_b_new);
};

uint8_t RGBAnimator::animate(uint8_t time_delta)
{
    if (!rgb_animation) {
        if(!task_list.empty()) {
            this->get_animation();
        }
    }
    if (b_running == 0) {
        // return zero so that the user knows that there is nothing to wait for
        // but dont reset time_delta_next so you can continue after setting b_running to true
        return 0;
    } else {
        // if previous animation has not stopped
        if (time_delta_next == 0) {
            if (task_list.empty()&&!this->rgb_animation->task->b_repeat) {
                // queue is empty
                time_delta_next = 0;
                return 0;
            } else {
                // get new animation from queue
                this->get_animation();
            }
        }           
        // update current animation
        time_delta_next = rgb_animation->update(time_delta, speed);
        return time_delta_next;
    }
}

void RGBAnimator::get_animation()
{
    // check if previous animation exists
    if(rgb_animation)
    {        
        // check if animation has to be repeated
        if(rgb_animation->task->b_repeat)
        {
            // queue task again
            this->queue_task(rgb_animation->task);
            b_running=1;
        } else {
            delete rgb_animation->task;
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

color_t RGBAnimator::get_color_current()
{
    if(rgb_animation) {
        if(brightness != 128) {
            // return reduced brightness color
            return color_t((uint8_t)((uint16_t) rgb_animation->color_current.R * brightness >> 7), \
                (uint8_t)((uint16_t) rgb_animation->color_current.G * brightness >> 7),\
                (uint8_t)((uint16_t) rgb_animation->color_current.B * brightness >> 7));
        } 
        // return full brightness color
        return rgb_animation->color_current;
    } else {
        // return black
        return color_t(0,0,0);
    }
}
color_t RGBAnimator::get_color_current_wo_brightness()
{
    if(rgb_animation){
        // return full brightness color
        return rgb_animation->color_current;
    } else {
        // return black
        return color_t(0,0,0);
    }
}

void RGBAnimator::set_color(color_t color_2_new)
{
    rgb_animation->color_current = color_2_new;
}

void RGBAnimator::pause()
{
    b_running = false;
}
void RGBAnimator::stop()
{
    clear_list();
    //rgb_animation->time_left = rgb_animation->task_virt()->time_duration;
    rgb_animation->task->b_repeat = false;
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

void RGBAnimator::set_brightness(uint8_t brightness_new)
{
    brightness = brightness_new;
}
void RGBAnimator::set_speed(uint8_t speed_new)
{
    speed = speed_new;    
}


void RGBAnimator::clear_list()
{
    while(!task_list.empty())
    {
        task_list.pop();
    }    
}
void RGBAnimator::add_flash(color_t color_1_new, color_t color_2_new, uint16_t time_1_new, uint16_t time_2_new, uint8_t num_repetitions_new, bool b_repeat_new)
{
    RGBTask* temp_task = new RGBTask(false, color_1_new, color_2_new, time_1_new, time_2_new, num_repetitions_new, b_repeat_new);
    queue_task(temp_task); 
}

void RGBAnimator::add_fade(color_t color_1_new, color_t color_2_new, uint16_t time_1_new, uint16_t time_2_new, uint8_t num_repetitions_new, bool b_repeat_new)
{
    RGBTask* temp_task = new RGBTask(true, color_1_new, color_2_new, time_1_new, time_2_new, num_repetitions_new, b_repeat_new);
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
    {
        if(dat_char<=0x07)
        {
            // valid startbyte for immediate commands
            switch(dat_char) {
                case 0x00: // start/resume
                    start();
                    break;			
                case 0x01: // stop
                    stop();
                    break;			
                case 0x02: // pause
                    pause();
                    break;             
                case 0x03: // faster
                    //faster();          
                    break;             
                case 0x04: // slower
                    //slower();
                    break;
                case 0x05: // brighter
                    //brighter();
                    break;
                case 0x06: // darker
                    //darker();
                    break;		
                case 0x07: // white
                    stop();
                    set_color(color_t(255, 255, 255)); // wharm white?
                    break;	
                /*case 0x08: // set speed
                    //stop();
                    //set_color(color_t(255, 255, 255)); // wharm white?
                    break;		
                case 0x09: // set brightness
                    //stop();
                    //set_color(color_t(255, 255, 255)); // wharm white?
                    break;*/	
            }		
        }
        else if (dat_char <= 0x15) {
            // valid startbyte for longer commands, set first element of data
            data[dat_count++] = dat_char;            
        } else {
            // unknown startbyte for commands, dat_count stays at zero
        }
    }
    else {// not first byte
        // further bytes of longer commands
        data[dat_count++] = dat_char;
        
        if (dat_count == 2) {
            switch(data[0]) {
                case 0x08: // set brightness
                    set_brightness(data[1]);
                    dat_count = 0;
                    break;
                case 0x09: // set speed
                    set_speed(data[1]);
                    dat_count = 0;
                    break;
            }
        }
        else if (dat_count == 4) {
            switch(data[0]) {
                case 0x10: // jump to color
                    stop();
                    set_color(color_t(data[1], data[2], data[3]));
                    dat_count = 0;
                    break;
                case 0x11: // fade to color;
                    stop();
                    add_fade(get_color_current_wo_brightness(), //color_1
                        color_t(data[1], data[2], data[3]), //color_2
                        2000, //time_duration
                        0,  //time_2
                        1, //num_repetitions
                        false); //b_repeat
                    start();
                    dat_count = 0;
                    break;
                case 0x12: // queue flash in color
                    stop();
                    add_flash(color_t(0,0,0), //color_1
                        color_t(data[1], data[2], data[3]), //color_2
                        100, //time_duration
                        50, //perc_on
                        2, //num_repetitions
                        true); //b_repeat
                    start();
                    dat_count = 0;
                    break;
                case 0x13: // queue pulse in color
                    stop();
                    add_fade(color_t(0,0,0), //color_1
                        color_t(data[1], data[2], data[3]), //color_2
                        1000, //time_duration
                        0, // time_2
                        2, //num_repetitions
                        true); //b_repeat
                    start();
                    dat_count = 0;
                    break;
            }		
        }
        else if (dat_count == 13) {
            switch(data[0]) {
                case 0x14: // queue jump between colors
                    add_flash(color_t(data[1], data[2], data[3]), //color_1
                        color_t(data[4], data[5], data[6]), //color_2
                        (uint16_t)(data[7] << 8) + data[8], //time_1
                        (uint16_t)(data[9] << 8) + data[10], //time_2
                        (uint8_t)data[11], //num_repetitions
                        (bool)data[12]); //b_repeat
                    dat_count = 0;
                    break;
                case 0x15: // queue fade color
                    add_fade(color_t(data[1], data[2], data[3]), //color_1
                        color_t(data[4], data[5], data[6]), //color_2
                        (uint16_t)(data[7] << 8) + data[8], //time_1
                        (uint16_t)(data[9] << 8) + data[10], //time_2
                        (uint8_t)data[11], //num_repetitions
                        (bool)data[12]); //b_repeat
                    dat_count = 0;
                    break;
            }	
            dat_count = 0;			
        }
    }
}