#include <Arduino.h>
#include <RGBAnimator.hpp>
#include <arduino-timer.h>
#include <FastLED.h>

Timer<10> timer; // 10 concurrent tasks, using millis as resolution

// RGB led pins in NodeMCU
#define LED_RED 15
#define LED_GREEN 12
#define LED_BLUE 13

RGBAnimator animorR;
RGBAnimator animorG;
RGBAnimator animorB;
uint8_t delta_time = 0;
uint32_t last, now;


bool repeatedly_update(void *) {
  delta_time = animorR.process(delta_time);
  paint_red(animorR.get_color_current());
  Serial.print("Wait: ");
  if (delta_time)    
    timer.in(delta_time, repeatedly_update);
  else
    Serial.println("process() returned 0. Animations have stoppped with nothing left in queue.");
  return 0;
}
void paint_red(color_t col)
{  
    analogWrite(LED_RED, col.R);
    Serial.print("R: ");
    Serial.println(col.R);
}
void paint_green(color_t col)
{  
    analogWrite(LED_GREEN, col.G);
    Serial.print("G: ");
    Serial.println(col.G);
}
void paint_blue(color_t col)
{  
    analogWrite(LED_BLUE, col.B);
    //Serial.print("B: ");
    //Serial.println(col.B);
}

void setup() {
  // Serial
  Serial.begin(115200);

  animorR = RGBAnimator(); // processed via timer
  animorG = RGBAnimator(); // processed with EVERY_N_MILISECONDS(50);
  animorB = RGBAnimator(); // processed with last and now from millis();
  animorR.add_flash(color_t(255,0,0), color_t(0,0,0),200,200,5,true);
  animorG.add_flash(color_t(0,255,0), color_t(0,0,0),200,200,5,true);
  animorB.add_flash(color_t(0,0,255), color_t(0,0,0),200,200,5,true);
    
  animorR.start();
  animorG.start();
  animorB.start();
  last = millis();
  timer.in(0, repeatedly_update);
}

// the loop function runs over and over again forever
void loop() {  
    timer.tick(); // tick the timer
  
    EVERY_N_MILLISECONDS(50)
    {
        animorG.process(50);
        paint_green(animorG.get_color_current());
    }
    
    now = millis();
    animorB.process(now - last);
    last = now;
    paint_blue(animorB.get_color_current());
}