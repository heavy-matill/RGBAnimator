/*
 * RGBFlasher.h
 *
 *  Created on: Jun 13, 2016
 *      Author: mathias illian
 */

#include "Arduino.h"

#ifndef RGBFlasher_H
#define RGBFlasher_H

class RGBFlasher {
public:
  // Who likes dealing with function pointers? (Ok, I do, but no one else does)
  typedef uint8_t (*curve_function)(uint8_t);
private:
  unsigned long last_switch_time_;
  unsigned int repetitions_;
  unsigned int current_repetition_;
  uint8_t value_r_;
  uint8_t value_g_;
  uint8_t value_b_;
  uint8_t target_r_;
  uint8_t target_g_;
  uint8_t target_b_;
  unsigned int on_time_;
  unsigned int off_time_;
  bool state_;
  
  curve_function curve_;

  public:
	uint8_t brightness_;

    // Create a new LED Fader for a pin
    RGBFlasher();

    // Set an LED to an absolute PWM value
    void SetValue(int new_value_r, int new_value_g, int new_value_b);

    // Get the current LED PWM value
    uint8_t GetValueR();
    uint8_t GetValueG();
    uint8_t GetValueB();	
    
    // Set curve to transform output
    void SetCurve(curve_function);
    
    // Get the current curve function pointer
    curve_function GetCurve();

    // Fade an LED to a PWM value over a duration of time (milliseconds)
    void Flash(uint8_t pwm_r, uint8_t pwm_g, uint8_t pwm_b , unsigned int on_time, unsigned int off_time, unsigned int repetitions);


    // Returns TRUE if there is an active fade process
    bool IsFlashing();

    // Stop the current fade where it's at
    void StopFlash();

    // Update the LEDs along the fade
    // Returns TRUE if a fade is still in process
    bool Update();

    // Decrease the current fading speed by a number of milliseconds
    void Slower(int by_seconds);

    // Increase the current fading speed by a number of milliseconds
    void Faster(int by_seconds);

    // Returns how much of the fade is complete in a percentage between 0 - 100
    bool GetState();
	
	
void SpeedPercent(int8_t perc);
};

#endif /* RGBFlasher_H */
