/*
 * RGBFader.cpp
 *
 *  Created on: Jun 13, 2016
 *      Author: mathias illian
 */

#include "RGBFader.h"
//#define MIN( a, b ) (a < b) ? a : b
//#define MAX( a, b ) (a > b) ? a : b
RGBFader::RGBFader() {
  value_r_ = 0;		//current output brightness of channel r
  value_g_ = 0;
  value_b_ = 0;
  target_r_ = 0;	//target brightness of channel r
  target_g_ = 0;
  target_b_ = 0;
  last_step_time_ = 0;
  interval_ = 0;
  duration_ = 0;
  percent_done_ = 0;
  curve_ = (curve_function)0;
  brightness_ =100;  
}


void RGBFader::SetValue(int value_r, int value_g, int value_b) {
	if(brightness_!=100)
	{
		value_r*=brightness_/100;
		value_g*=brightness_/100;
		value_b*=brightness_/100;
	}
	value_r = (uint8_t)constrain(value_r, 0, 255);
	value_g = (uint8_t)constrain(value_g, 0, 255);
	value_b = (uint8_t)constrain(value_b, 0, 255);
	if (curve_)
	{
		value_r = curve_(value_r);
		value_g = curve_(value_g);
		value_b = curve_(value_b);
	}
	value_r_ = value_r;
	value_g_ = value_g;
	value_b_ = value_b;
}

uint8_t RGBFader::GetValueR() {
  return value_r_;
}
uint8_t RGBFader::GetValueG() {
  return value_g_;
}
uint8_t RGBFader::GetValueB() {
  return value_b_;
}

uint8_t RGBFader::GetTargetValueR() {
  return target_r_;
}
uint8_t RGBFader::GetTargetValueG() {
  return target_g_;
}
uint8_t RGBFader::GetTargetValueB() {
  return target_b_;
}
    
// Set curve to transform output
void RGBFader::SetCurve(curve_function c) {
 curve_ = c;
}

// Get the current curve function pointer
RGBFader::curve_function RGBFader::GetCurve() {
 return curve_;
}

void RGBFader::Slower(int by) {
  float cached_percent = percent_done_;
  duration_ += by;
  Fade(target_r_,target_g_,target_b_, duration_);
  percent_done_ = cached_percent;
}

void RGBFader::Faster(int by) {
  float cached_percent = percent_done_;

  // Ends the fade
  if (duration_ <= by) {
    StopFade();
    SetValue(target_r_, target_g_, target_b_);
  }
  else {
    duration_ -= by;
  Fade(target_r_,target_g_,target_b_, duration_);
  }
  percent_done_ = cached_percent;
}

void RGBFader::Fade(uint8_t value_r, uint8_t value_g, uint8_t value_b, unsigned int time) {
  StopFade();
  percent_done_ = 0;

  // Color hasn't changed
  if ((value_r == value_r_)&&(value_g==value_g_)&&(value_b==value_b_)) {
    return;
  }

  if (time <= MIN_INTERVAL) {
    SetValue(value_r, value_g, value_b);
    return;
  }

  duration_ = time;
  target_r_ = (uint8_t)constrain(value_r, 0, 255);
  target_g_ = (uint8_t)constrain(value_g, 0, 255);
  target_b_ = (uint8_t)constrain(value_b, 0, 255);

  // Figure out what the interval should be so that we're chaning the color by at least 1 each cycle
  // (minimum interval is MIN_INTERVAL)
  float color_diff_r = abs(value_r_ - target_r_);
  float color_diff_g = abs(value_g_ - target_g_);
  float color_diff_b = abs(value_b_ - target_b_);
  unsigned int interval_r =round((float)duration_ / color_diff_r);
  unsigned int interval_g = round((float)duration_ / color_diff_g);
  unsigned int interval_b = round((float)duration_ / color_diff_b);
  interval_ = (MAX(MAX(interval_r,interval_g),interval_b));
  if (interval_ < MIN_INTERVAL) {
    interval_ = MIN_INTERVAL;
  }

  last_step_time_ = millis();
}

bool RGBFader::IsFading() {
  if (duration_ > 0)
    return true;
  return false;
}

void RGBFader::StopFade() {
  percent_done_ = 100;
  duration_ = 0;
}

uint8_t RGBFader::GetProgress() {
  return round(percent_done_);
}

bool RGBFader::Update() {
  // No fade
  if (duration_ == 0) {
    return false;
  }

  unsigned long now = millis();
  unsigned int time_diff = now - last_step_time_;

  // Interval hasn't passed yet
  if (time_diff < interval_) {
    return false; //true
  }

  // How far along have we gone since last update
  float percent = (float)time_diff / (float)duration_;
  percent_done_ += percent;

  // We've hit 100%, set LED to the final color
  if (percent >= 1) {
    StopFade();
    SetValue(target_r_, target_g_, target_b_);
    return true; //false
  }

  // Move color to where it should be
  int color_diff_r = target_r_ - value_r_;
  int color_diff_g = target_g_ - value_g_;
  int color_diff_b = target_b_ - value_b_;
  int increment_r = round(color_diff_r * percent);
  int increment_g = round(color_diff_g * percent);
  int increment_b = round(color_diff_b * percent);

  SetValue(value_r_ + increment_r, value_g_ + increment_g, value_b_ + increment_b);

  // Update time and finish
  duration_ -= time_diff;
  last_step_time_ = millis();
  return true;
}

void RGBFader::SpeedPercent(int8_t perc)
{if(perc>-100)
	{
  float cached_percent = percent_done_;
	
	duration_+= (duration_*perc)/100;
	percent_done_ = cached_percent;}
}
