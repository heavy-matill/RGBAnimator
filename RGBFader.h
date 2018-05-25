/*
 * RGBFader.h
 *
 *  Created on: Sep 24, 2013
 *      Author: jeremy
 */

#include "Arduino.h"

#ifndef RGBFader_H
#define RGBFader_H

// The minimum time (milliseconds) the program will wait between LED adjustments
// adjust this to modify performance.
#define MIN_INTERVAL 20

class RGBFader {
public:
  // Who likes dealing with function pointers? (Ok, I do, but no one else does)
  typedef uint8_t (*curve_function)(uint8_t);
private:
  unsigned long last_step_time_;
  unsigned int interval_;
  uint8_t value_r_;
  uint8_t value_g_;
  uint8_t value_b_;
  uint8_t target_r_;
  uint8_t target_g_;
  uint8_t target_b_;
  unsigned int duration_;
  float percent_done_;
  curve_function curve_;

  public:
	uint8_t brightness_;
    // Create a new LED Fader for a pin
    RGBFader();


    // Set an LED to an absolute PWM value
    void SetValue(int value_r, int value_g, int value_b);

    // Get the current LED PWM value
    uint8_t GetValueR();
    uint8_t GetValueG();
    uint8_t GetValueB();

    // Get the PWM value we're fading to
    uint8_t GetTargetValueR();
    uint8_t GetTargetValueG();
    uint8_t GetTargetValueB();
    
    // Set curve to transform output
    void SetCurve(curve_function);
    
    // Get the current curve function pointer
    curve_function GetCurve();

    // Fade an LED to a PWM value over a duration of time (milliseconds)
    void Fade(uint8_t pwm_r, uint8_t pwm_g, uint8_t pwm_b, unsigned int time);


    // Returns TRUE if there is an active fade process
    bool IsFading();

    // Stop the current fade where it's at
    void StopFade();

    // Update the LEDs along the fade
    // Returns TRUE if a fade is still in process
    bool Update();

    // Decrease the current fading speed by a number of milliseconds
    void Slower(int by_seconds);

    // Increase the current fading speed by a number of milliseconds
    void Faster(int by_seconds);

    // Returns how much of the fade is complete in a percentage between 0 - 100
    uint8_t GetProgress();
	
	
void SpeedPercent(int8_t perc);
};

#endif /* RGBFader_H */
