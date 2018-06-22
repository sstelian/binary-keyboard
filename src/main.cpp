#include <mbed.h>
#include <USBKeyboard.h>
#include "PinDetect.h"

#define KEY_0 PTC11
#define KEY_1 PTC10

USBKeyboard keyboard;
PinDetect keys[2] {KEY_0, KEY_1};

volatile bool lock = 0;
volatile uint8_t counter = 0, buffer = 0, validChar = 0;

DigitalOut leds[3] {LED_RED, LED_GREEN, LED_BLUE};

void zeroPressed()
{
  if(!lock)
  {
    lock = 1;
    leds[0] = !leds[0];
    counter++;
    buffer <<= 1;
    if (counter >= 8)
    {
      counter = 0;
      validChar = 1;
      leds[2] = !leds[2];
    }
    lock = 0;
  }
}

void onePressed()
{
  if(!lock)
  {
    lock = 1;
    leds[1] = !leds[1];
    counter++;
    buffer <<= 1;
    buffer |= 1;
    if (counter >= 8)
    {
      counter = 0;
      validChar = 1;
      leds[2] = !leds[2];
    }
    lock = 0;
  }
}

int main()
{
  for (auto &key : keys)
  {
    key.mode(PullUp);
    key.setSampleFrequency();
  }

  keys[0].attach_deasserted(&zeroPressed);
  keys[1].attach_deasserted(&onePressed);

  while(1)
  {
    if (validChar)
    {
      validChar = 0;
      keyboard.printf("%c", buffer);
      buffer = 0;
    }
  }
}
