/*
 * RGBAnimation.cpp
 *
 *  Created on: Jun 13, 2016
 *      Author: mathias illian
 */

#include "RGBAnimation.h"

RGBAnimation::RGBAnimation(uint8_t pwm_w) {
  
  flasher_ = RGBFlasher();
  fader_ = RGBFader();
  curve_ = (curve_function)0;
  pwm_pin_w_ = pwm_w;
  state_ = 0;
}


void RGBAnimation::SetBrightness(uint8_t new_brightness)
{
	flasher_.brightness_ = new_brightness;
	fader_.brightness_ = new_brightness;
}

void RGBAnimation::ChangeBrightness(int8_t brightness_change)
{
	uint8_t new_brightness = flasher_.brightness_+brightness_change;
	if(new_brightness>100)
	new_brightness = 100;
	else if(new_brightness<0)
	new_brightness=0;
	SetBrightness(new_brightness);
}


void RGBAnimation::SetValue(int pwm_r, int pwm_g, int pwm_b) { 
	flasher_.SetValue(pwm_r, pwm_g, pwm_b);
}

uint8_t RGBAnimation::GetValueR() {
	if(state_==1)
		return flasher_.GetValueR();
	return fader_.GetValueR();
}
uint8_t RGBAnimation::GetValueG() {
	if(state_==1)
		return flasher_.GetValueG();
	return fader_.GetValueG();
}
uint8_t RGBAnimation::GetValueB() {
	if(state_==1)
		return flasher_.GetValueB();
	return fader_.GetValueB();
}
    
// Set curve to transform output
void RGBAnimation::SetCurve(curve_function c) {
 curve_ = c;
 flasher_.SetCurve(c);
 fader_.SetCurve(c);
}

// Get the current curve function pointer
RGBAnimation::curve_function RGBAnimation::GetCurve() {
 return curve_;
}

void RGBAnimation::Slower(int by) {
if(state_==1)
	flasher_.Slower(by);
	else
	{
	fader_.Slower(by);
	}
	if(!AnimationList_.isEmpty())
	{
		QueueList<Animation*>::link temp_node = (QueueList<Animation*>::link)AnimationList_.head;
		do
		{
		Animation* anim = (Animation*)temp_node->item;
		 if((anim->mode_)==1)
		 {
		   FlashAnimation* fanim = (FlashAnimation*) anim;
		   fanim->Slower(by);
		 }
		 else
		 {
		   FadeAnimation* fanim = (FadeAnimation*) anim;
		   fanim->Slower(by);
		 }
			temp_node=temp_node->next;
		}while(temp_node); 
	}
}

void RGBAnimation::Faster(int by) {
if(state_==1)
	flasher_.Faster(by);
	else
	{
	fader_.Faster(by);
	}
	if(!AnimationList_.isEmpty())
	{	
		QueueList<Animation*>::link temp_node = (QueueList<Animation*>::link)AnimationList_.head;
		do
		{
			Animation* anim = (Animation*)temp_node->item;
		 if((anim->mode_)==1)
		 {
		   FlashAnimation* fanim = (FlashAnimation*) anim;
		   fanim->Faster(by);
		 }
		 else
		 {
		   FadeAnimation* fanim = (FadeAnimation*) anim;
		   fanim->Faster(by);
		 }
			temp_node=temp_node->next;
		}while(temp_node); 
	}
}

void RGBAnimation::SpeedPercent(int8_t perc) {
if(state_==1)
	flasher_.SpeedPercent(perc);
	else
	{
	fader_.SpeedPercent(perc);
	}
	if(!AnimationList_.isEmpty())
	{	
		QueueList<Animation*>::link temp_node = (QueueList<Animation*>::link)AnimationList_.head;
		do
		{
			Animation* anim = (Animation*)temp_node->item;
		 if((anim->mode_)==1)
		 {
		   FlashAnimation* fanim = (FlashAnimation*) anim;
		   fanim->SpeedPercent(perc);
		 }
		 else
		 {
		   FadeAnimation* fanim = (FadeAnimation*) anim;
		   fanim->SpeedPercent(perc);
		 }
			temp_node=temp_node->next;
		}while(temp_node); 
	}
}

void RGBAnimation::Flash(uint8_t pwm_r, uint8_t pwm_g, uint8_t pwm_b , unsigned int new_on_time, unsigned int new_off_time, unsigned int new_repetitions) {
	Stop();  
	flasher_.Flash(pwm_r, pwm_g, pwm_b, new_on_time, new_off_time, new_repetitions);
	state_ = 1;
}

void RGBAnimation::Fade(uint8_t pwm_r, uint8_t pwm_g, uint8_t pwm_b , unsigned int time) {
	Stop();  
	fader_.Fade(pwm_r, pwm_g, pwm_b, time);
	state_ = 2;
}

bool RGBAnimation::IsFlashing() {
  return flasher_.IsFlashing();
}

bool RGBAnimation::IsFading() {
  return fader_.IsFading();
}

bool RGBAnimation::IsRunning() {
  return (flasher_.IsFlashing()||fader_.IsFading());
}

void RGBAnimation::Stop() {
  flasher_.StopFlash();
  fader_.StopFade();
  state_=0;
}

void RGBAnimation::ClearList() {
  while(!AnimationList_.isEmpty())
	{		
		AnimationList_.pop();
	}
}

uint8_t RGBAnimation::GetState() {
	return state_;
}

bool RGBAnimation::Update() {
	if(state_==1)
		return flasher_.Update();
	/*if(state==2){
		
	bool ret = fader_r.update();
	ret &= fader_g.update();
	ret &= fader_b.update();
	return ret;*/
	else if(state_==2)return fader_.Update();
}

FadeAnimation::FadeAnimation(uint8_t new_to_color_r, uint8_t new_to_color_g, uint8_t new_to_color_b, unsigned int new_time, bool new_repeat)
{
	to_color_r_ = new_to_color_r;
	to_color_g_ = new_to_color_g;
	to_color_b_ = new_to_color_b;
	time_ = new_time;
	repeat_=new_repeat;
	mode_=2;
}
FlashAnimation::FlashAnimation(uint8_t new_to_color_r, uint8_t new_to_color_g, uint8_t new_to_color_b, unsigned int new_on_time, unsigned int new_off_time, unsigned int new_repetitions, bool new_repeat)
{
	to_color_r_ = new_to_color_r;
	to_color_g_ = new_to_color_g;
	to_color_b_ = new_to_color_b;
	on_time_ = new_on_time;
	off_time_ = new_off_time;
	repetitions_ = new_repetitions;
	repeat_=new_repeat;
	mode_=1;
}

void RGBAnimation::AddFlash(uint8_t new_to_color_r, uint8_t new_to_color_g, uint8_t new_to_color_b, unsigned int new_on_time, unsigned int new_off_time, unsigned int new_repetitions, bool new_repeat)
{
	Serial.println(new_to_color_r);
	Serial.println(new_to_color_g);
	Serial.println(new_to_color_b);
	Serial.println(new_on_time);
	Serial.println(new_off_time);
	Serial.println(new_repetitions); //debug
	Animation *newAnimation = new FlashAnimation(new_to_color_r, new_to_color_g, new_to_color_b, new_on_time, new_off_time, new_repetitions, new_repeat);
	AnimationList_.push(newAnimation);
}

void RGBAnimation::AddFade(uint8_t new_to_color_r, uint8_t new_to_color_g, uint8_t new_to_color_b, unsigned int new_time, bool new_repeat)
{
	Animation *newAnimation = new FadeAnimation(new_to_color_r, new_to_color_g, new_to_color_b, new_time, new_repeat);
	AnimationList_.push(newAnimation);
}

void RGBAnimation::PopAnimation()
{
	if(AnimationList_.isEmpty())
		return;
	Animation* anim = AnimationList_.pop();
	if((anim->mode_)==1)
	{
		FlashAnimation* fanim = (FlashAnimation*) anim;
		Flash(fanim->to_color_r_, fanim->to_color_g_, fanim->to_color_b_, fanim->on_time_, fanim->off_time_, fanim->repetitions_);
	}
	else
	{
		FadeAnimation* fanim = (FadeAnimation*) anim;
		Fade(fanim->to_color_r_, fanim->to_color_g_, fanim->to_color_b_, fanim->time_);
	}
	if(!anim->repeat_)
	{
		delete [] anim;
	}
	else{
		AnimationList_.push(anim);
	}
}

bool RGBAnimation::ListIsEmpty()
{
	return AnimationList_.isEmpty();
}

void FlashAnimation::Slower(int by)
{
	off_time_+=by;
	on_time_+=by;
}
void FadeAnimation::Slower(int by)
{
	time_+=by;
}
void FlashAnimation::Faster(int by)
{
	if(off_time_>by && on_time_>by)
	{
		off_time_-=by;
		on_time_-=by;
	}
}
void FadeAnimation::Faster(int by)
{
	if(time_>by)
	time_-=by;
}
void FadeAnimation::SpeedPercent(int8_t perc)
{
	if(perc>-100)
	time_+= (time_*perc)/100;
}
void FlashAnimation::SpeedPercent(int8_t perc)
{
	if(perc>-100)
	{
		off_time_+=(off_time_*perc)/100;
		on_time_+= (on_time_*perc)/100;
	}
}
void RGBAnimation::DataAvailable(uint8_t datChar)
{
	if(!datCount_)
	{ //first received symbol
		switch(datChar)
        {
			case 0x01: //new jump to color
				data_[0] = 0x01;
				datCount_ = 1;
				break;			
			case 0x02: //new slide to color
				data_[0] = 0x02;
				datCount_ = 1;
				break;             
            case 0x03: //new flash in color 
				data_[0] = 0x03;
				datCount_ = 1;           
				break;             
            case 0x04: //faster
				Faster(100);
				Serial.println("faster");
				break;
            case 0x05: //slower
				Slower(100);
				break;
            case 0x06: //speedPercent
				datCount_ = 1;
				break;
            case 0x07: //clearList
				ClearList();
				Stop();				
				flasher_.SetValue(0,0,0);
				break;
            case 0x08: //pause
				pause_ = true;
				//millis_temp=millis();
				break;
            case 0x09: //resume
				//setMillis(millis_temp);
				pause_ = false;				
				break;             
            case 0x0A: //immediately setColor
				data_[0] = 0x0A;
				datCount_ = 1;
				break;
			case 0x0B: //immediately slideColor
				data_[0] = 0x0B;
				datCount_ = 1;
				break;	
			case 0x0C: //set Brightness uint8_t 0-100;
				data_[0] = 0x0C;
				datCount_ = 1;
				break;	
			case 0x0D: //change Brightness int8_t -100..+100;
				data_[0] = 0x0D;
				datCount_ = 1;
				break;		
			case 0x0E: //white brightness
				data_[0] = 0x0E;
				datCount_ = 1;
				break;
		}
	}
	else
    {         
        data_[datCount_] = datChar;
        datCount_++;
		if(datCount_==4&&data_[0]==0x0A)
        {
            Stop();
			ClearList();
			flasher_.SetValue(data_[1],data_[2],data_[3]);
			datCount_=0;
        }
        if(datCount_==4&&data_[0]==0x0B)
        {
			Stop();
			ClearList();
            Fade(data_[1],data_[2],data_[3],200);
            datCount_=0;
        }
        if(datCount_==6&&data_[0]==0x01)//jump to color
        {
            AddFlash(data_[1], data_[2], data_[3], (unsigned int)data_[4]*100, 0, 1, (bool)data_[5]);
            datCount_ = 0;
        }
        if(datCount_==6&&data_[0]==0x02)//fade to color
        {
            AddFade(data_[1], data_[2], data_[3], (unsigned int)data_[4]*100, (bool) data_[5]);
            datCount_ = 0;
        }
        if(datCount_==8&&data_[0]==0x03)//flash in color
        {
			Serial.println("New Flash");
            AddFlash(data_[1], data_[2], data_[3], (unsigned int)data_[4], (unsigned int)data_[5], (unsigned int)1*data_[6], (bool)data_[7]);
            datCount_ = 0;
        }
        if(datCount_==2&&data_[0]==0x06)
        {            
			SpeedPercent(data_[1]-100);
			datCount_=0;
        }
        if(datCount_==2&&data_[0]==0x0C)
        {            
			SetBrightness(data_[1]);
			datCount_=0;
        }
        if(datCount_==2&&data_[0]==0x0D)
        {            
			ChangeBrightness(data_[1]-100);
			datCount_=0;
        }
        if(datCount_==2&&data_[0]==0x0E)
        {            
			if(pwm_pin_w_)
				pwm_pin_w_ = data_[1];
			datCount_=0;
        }
		if(datCount_>8)
			datCount_=0;
    }
}

void RGBAnimation::ResetDatCount()
{
	datCount_=0;
}

/*void RGBAnimation::stringAvailable(String str)
{
	for (int i=0;i<str.length(); )
	{
		switch((uint8_t) str[i++])
        {
			case 0x01: //jump to color
				addFlash((uint8_t) str[i++], (uint8_t) str[i++],(uint8_t) str[i++], (unsigned int)str[i++]*100, 0, 1, (bool)str[i++]);
				break;			
			case 0x02: //new slide to color
				addFade((uint8_t) str[i++], (uint8_t) str[i++],(uint8_t) str[i++], (unsigned int)str[i++]*100, (bool)str[i++]);
				break;             
            case 0x03: //new flash in color 
				addFlash((uint8_t) str[i++], (uint8_t) str[i++],(uint8_t) str[i++], (unsigned int)str[i++]*10, (unsigned int)str[i++]*10, (unsigned int)str[i++], (bool)str[i++]);
				break;             
            case 0x04: //faster
				faster(100);
				break;
            case 0x05: //slower
				slower(100);
				break;
            case 0x06: //speedPercent
				speedPercent((uint8_t)str[i++]-100);
				break;
            case 0x07: //clearList
				clearList();
				stop();				
				flasher.set_value(0,0,0);
				break;
            case 0x08: //pause
				pause = true;
				//millis_temp=millis();
				break;
            case 0x09: //resume
				//setMillis(millis_temp);
				pause = false;				
				break;             
            case 0x0A: //immediately setColor
				stop();
				clearList();
				flasher.set_value((uint8_t)str[i++],(uint8_t)str[i++],(uint8_t)str[i++]);
				break;
			case 0x0B: //immediately slideColor
				stop();
				clearList();
				fade((uint8_t)str[i++],(uint8_t)str[i++],(uint8_t)str[i++],200);
				break;	
			case 0x0C: //set Brightness uint8_t 0-100;
				set_brightness((uint8_t)str[i++]);
				break;	
			case 0x0D: //change Brightness int8_t -100..+100;
				change_brightness((uint8_t)str[i++]-100);
				break;	
		}
	}
}*/