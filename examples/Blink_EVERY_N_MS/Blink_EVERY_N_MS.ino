#include <Arduino.h>
#include <RGBAnimator.hpp>
#include <FastLED.h>

RGBAnimator animor;
uint32_t last, now;

void setup()
{
    animor = RGBAnimator();
    // add flash animation for red channel 500ms on and off 3 times and requeued after animation
    animor.add_flash(color_t(255, 0, 0), color_t(0, 0, 0), 200, 200, 5, true);
    // add flash animation for red channel 500ms on and off 3 times and requeued after animation
    animor.add_flash(color_t(255, 0, 0), color_t(0, 0, 0), 500, 500, 3, true);
    // add flash animation for green channel 200ms on and off 10 times and not requeued after animation
    // since we only display red channel this animation will not be visible
    animor.add_flash(color_t(0, 255, 0), color_t(0, 0, 0), 100, 100, 10, false);

    animor.start();
    last = millis();
}
void paint_col(color_t col)
{
    // output red channel inverse on builtin led
    digitalWrite(LED_BUILTIN, col.R < 127);
}

// the loop function runs over and over again forever
void loop()
{

    EVERY_N_MILLISECONDS(50)
    {
        animor.process(50);
        paint_col(animor.get_color_current());
    }
}