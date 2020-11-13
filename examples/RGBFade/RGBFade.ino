#include <Arduino.h>
#include <RGBAnimator.hpp>

// RGB led pins in NodeMCU
#define LED_RED 15
#define LED_GREEN 12
#define LED_BLUE 13

RGBAnimator animor;
uint32_t last, now;

void setup()
{
    animor = RGBAnimator();
    // add flash animation for red channel 200ms on and off 3 times and requeued after animation
    animor.add_flash(color_t(255, 0, 0), color_t(0, 255, 0), 200, 200, 5, false);
    // add fade animation only fading color for 1 second each and not holding final color (0)
    animor.add_fade(color_t(255, 0, 0), color_t(255, 255, 0), 1000, 0, 1, true);
    animor.add_fade(color_t(255, 255, 0), color_t(0, 255, 0), 1000, 0, 1, true);
    animor.add_fade(color_t(0, 255, 0), color_t(0, 255, 255), 1000, 0, 1, true);
    animor.add_fade(color_t(0, 255, 255), color_t(0, 0, 255), 1000, 0, 1, true);
    animor.add_fade(color_t(0, 0, 255), color_t(255, 0, 255), 1000, 0, 1, true);
    animor.add_fade(color_t(255, 0, 255), color_t(255, 0, 0), 1000, 0, 1, true);
    // add flash animation for red channel 500ms on and off 3 times and requeued after animation
    animor.add_flash(color_t(255, 255, 0), color_t(0, 0, 255), 1000, 500, 3, false);

    animor.start();
    last = millis();
}
void paint_col(color_t col)
{
    // analog write each color channel to the rgb led pins
    analogWrite(LED_RED, col.R);
    analogWrite(LED_GREEN, col.G);
    analogWrite(LED_BLUE, col.B);
}

// the loop function runs over and over again forever
void loop()
{

    // RGBAnimator
    now = millis();
    animor.process(now - last);
    last = now;
    paint_col(animor.get_color_current());
}