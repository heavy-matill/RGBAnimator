/*
 * RGBFlasher.cpp
 *
 *  Created on: Jun 13, 2016
 *      Author: mathias illian
 */

#include "RGBFlasher.h"

RGBFlasher::RGBFlasher() {
	last_switch_time_ = 0;
  repetitions_ = 0;
  current_repetition_ = 0;
  value_r_=0;	//current output brightness of channel r
  value_g_=0;
  value_b_=0;
  target_r_ = 0;	//target brightness of channel r
  target_g_ = 0;
  target_b_ = 0;
  on_time_=0;
  off_time_=0;
  curve_ = (curve_function)0;
  state_ = true;
  brightness_ =100;
}

void RGBFlasher::SetValue(int new_value_r, int new_value_g, int new_value_b) {	
	if(brightness_!=100)
	{
		new_value_r*=brightness_/100;
		new_value_g*=brightness_/100;
		new_value_b*=brightness_/100;
	}
  if (curve_)
  {
   new_value_r = curve_(new_value_r);
   new_value_g = curve_(new_value_g);
   new_value_b = curve_(new_value_b);
   }
   value_r_ = new_value_r;
   value_g_ = new_value_g;
   value_b_ = new_value_b;
}

uint8_t RGBFlasher::GetValueR() {
  return value_r_;
}
uint8_t RGBFlasher::GetValueG() {
  return value_g_;
}
uint8_t RGBFlasher::GetValueB() {
  return value_b_;
}
    
// Set curve to transform output
void RGBFlasher::SetCurve(curve_function c) {
 curve_ = c;
}

// Get the current curve function pointer
RGBFlasher::curve_function RGBFlasher::GetCurve() {
 return curve_;
}

void RGBFlasher::Slower(int by) {
  on_time_ += by;
  off_time_ += by;
  Flash(value_r_, value_b_, value_g_, on_time_, off_time_, repetitions_);
}

void RGBFlasher::Faster(int by) {

  // Ends the fade
  if (on_time_ <= by||off_time_<=by) {
    StopFlash();
	if(on_time_>off_time_)
		  SetValue(value_r_, value_g_, value_b_);
	else
		SetValue(0,0,0);
		
  }  
  else 
  {
	on_time_ -= by; //??? mal verhältnis off on
	off_time_ -= by;
  }
}

void RGBFlasher::Flash(uint8_t pwm_r, uint8_t pwm_g, uint8_t pwm_b , unsigned int new_on_time, unsigned int new_off_time, unsigned int new_repetitions) {
  StopFlash();
  current_repetition_ = 1;
  if (new_repetitions == 0) {
    SetValue(pwm_r, pwm_g, pwm_b);
    return;
  }

  on_time_ = new_on_time;
  off_time_ = new_off_time;
  repetitions_ = new_repetitions;
  
  target_r_=pwm_r;
  target_g_=pwm_g;
  target_b_=pwm_b;
  
  // Figure out what the interval should be so that we're chaning the color by at least 1 each cycle
  // (minimum interval is MIN_INTERVAL)
  SetValue(pwm_r, pwm_g, pwm_b);
  last_switch_time_ = millis();
}

bool RGBFlasher::IsFlashing() {
  if (repetitions_ >= current_repetition_)
    return true;
  return false;
}

void RGBFlasher::StopFlash() {	
	//SetValue(0, 0, 0);
  current_repetition_ = repetitions_+1;
}

bool RGBFlasher::GetState() {
	return state_;
}

bool RGBFlasher::Update() {

  // No fade
  if (repetitions_ < current_repetition_) 
  {
    return false;
  }

  unsigned long now = millis();
  
  if(state_)
  {
	  if(now>(last_switch_time_+on_time_))
	  {
		  SetValue(0,0,0);
		  state_ = !state_;
		  last_switch_time_=now;
		  current_repetition_++; 
		  return true;
	  }	  
  }
  else
  {
	  if(now>(last_switch_time_+off_time_))
	  {		  
		  SetValue(target_r_, target_g_, target_b_);
		  state_ = !state_;
		  last_switch_time_=now;
		  return true;
	  }	  
  }
  return false;
}

void RGBFlasher::SpeedPercent(int8_t perc)
{
	if(perc>-100)
	{
		off_time_+=(off_time_*perc)/100;
		on_time_+= (on_time_*perc)/100;
	}
}
