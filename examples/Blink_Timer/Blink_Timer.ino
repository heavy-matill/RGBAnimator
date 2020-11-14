#include <Arduino.h>
#include <RGBAnimator.hpp>
#include <arduino-timer.h>

Timer<10> timer; // 10 concurrent tasks, using millis as resolution

// RGB led pins in NodeMCU
#define LED_RED 15
#define LED_GREEN 12
#define LED_BLUE 13

RGBAnimator animor;
uint8_t delta_time = 1;
uint32_t last, now;


bool repeatedly_update(void *) {
  delta_time = animor.process(delta_time);
  paint_col(animor.get_color_current());
  Serial.print("Wait: ");
  Serial.print(delta_time);
  Serial.print(" ms; with value: ");
  if (delta_time)    
    timer.in(delta_time, repeatedly_update);
  else
    Serial.println("process() returned 0. Animations have stoppped with nothing left in queue.");
  return 0;
}
void paint_col(color_t col)
{  
    // output red channel inverse on builtin led
    analogWrite(LED_RED, col.R);
    Serial.println(col.R);
}

void setup() {
  // Serial
  Serial.begin(115200);

  animor = RGBAnimator();
  // add flash animation for red channel 500ms on and off 3 times and requeued after animation
  animor.add_flash(color_t(255,0,0), color_t(0,0,0),200,200,5,true);
  // add flash animation for red channel 500ms on and off 3 times and requeued after animation
  animor.add_flash(color_t(255,0,0), color_t(0,0,0),500,500,3,true);
  // add flash animation for green channel 200ms on and off 10 times and not requeued after animation
  // since we only display red channel this animation will not be visible
  animor.add_flash(color_t(0,255,0), color_t(0,0,0),100,100,10,false);
    
  animor.start();
  timer.in(1, repeatedly_update);
}

// the loop function runs over and over again forever
void loop() {  
  timer.tick(); // tick the timer
}