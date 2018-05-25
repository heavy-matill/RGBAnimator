/*
 * RGBAnimation.h
 *
 *  Created on: Jun 13, 2016
 *      Author: mathias illian
 */

#include "Arduino.h"
#include "RGBFlasher.h"
#include "RGBFader.h"
#include "Curve.h"
#include "QueueList.h"

#ifndef RGBAnimation_H
#define RGBAnimation_H

// The minimum time (milliseconds) the program will wait between LED adjustments
// adjust this to modify performance.
#define MIN_INTERVAL 20

typedef struct color
{
    uint8_t R;
    uint8_t G;
    uint8_t B;
} color_t;

class Animation
{
  public:
    uint8_t mode_;
    color_t color_from;
    color_t color_to;
    bool repeat_;

    void Slower(int decrease);
    void Faster(int increase);
    void SpeedPercent(int8_t percent);
};

class FadeAnimation : public Animation
{
  public:
    unsigned int duration_;
    FadeAnimation(color_t color_to_new, unsigned int duration_new, bool repeat_new = false);
};

class FlashAnimation : public Animation
{
  public:
    unsigned int time_on_;
    unsigned int time_off_;
    uint8_t repetitions_;
    FlashAnimation(color_t color_to_new, unsigned int time_on_new, unsigned int time_off_new, uint8_t repititions_new, bool repeat_new = false);
};

class RGBAnimator
{
  public:
    // Who likes dealing with function pointers? (Ok, I do, but no one else does)
    typedef uint8_t (*curve_function)(uint8_t);
    QueueList<Animation *> AnimationList_;

  private:
    RGBFlasher flasher_;
    RGBFader fader_;

    uint8_t state_;

    curve_function curve_;

    //dataAvailable addon
    uint8_t data_[8];
    uint8_t datCount_ = 0;
    unsigned long millis_temp_;
    bool pause_ = false;

    uint8_t pwm_pin_w_;

  public:
    // Create a new RGBAnimation for a pin
    RGBAnimation(uint8_t pwm_w = 0);

    // Set an LED to an absolute PWM value
    void SetValue(int pwm_r, int pwm_g, int pwm_b);

    // Get the current LED PWM value
    uint8_t GetValueR();
    uint8_t GetValueG();
    uint8_t GetValueB();

    // Set curve to transform output
    void SetCurve(curve_function);

    // Get the current curve function pointer
    curve_function GetCurve();

    // Fade an LED to a PWM value over a duration of time (milliseconds)
    void Flash(uint8_t pwm_r, uint8_t pwm_g, uint8_t pwm_b, unsigned int on_time, unsigned int off_time, unsigned int repetitions);

    void Fade(uint8_t pwm_r, uint8_t pwm_g, uint8_t pwm_b, unsigned int time);

    // Returns TRUE if there is an active fade process
    bool IsFlashing();
    bool IsFading();
    bool IsRunning();

    // Stop the current fade where it's at
    void Stop();

    // Update the LEDs along the fade
    // Returns TRUE if a fade is still in process
    bool Update();

    // Decrease the current fading speed by a number of milliseconds
    void Slower(int by_seconds);

    // Increase the current fading speed by a number of milliseconds
    void Faster(int by_seconds);

    // Returns how much of the fade is complete in a percentage between 0 - 100
    uint8_t GetState();

    void AddFlash(uint8_t new_to_color_r, uint8_t new_to_color_g, uint8_t new_to_color_b, unsigned int new_on_time, unsigned int new_off_time, unsigned int new_repetitions, bool new_repeat = false);

    void AddFade(uint8_t new_to_color_r, uint8_t new_to_color_g, uint8_t new_to_color_b, unsigned int time, bool new_repeat = false);

    void PopAnimation();
    void ClearList();

    bool ListIsEmpty();

    void DataAvailable(uint8_t datChar);
    void ResetDatCount();
    //void stringAvailable(String str);
    void SpeedPercent(int8_t perc);
    void SetBrightness(uint8_t new_brightness);

    void ChangeBrightness(int8_t brightness_change);
};

#endif /* RGBAnimation_H */
