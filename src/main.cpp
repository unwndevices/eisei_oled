

#include <Arduino.h>
#include "USB.h"

#include "drivers/pinout.h"
#include "drivers/led.h"
#include "drivers/touchwheel.h"
#include "utils/utils.h"

#include "synthesis/SharedData.hpp"
#include "Game.hpp"

//-------------------------------
Leds leds;
//-------------------------------
Game eisei;
TouchWheel *touch = nullptr;

void setup()
{
  // SERIAL
  USBSerial.begin(115200);
  USBSerial.setDebugOutput(true);

  // LED
  leds.init();
  USBSerial.println("Leds initialized");
  // TOUCH
  touch = TouchWheel::GetInstance();
  touch->Init();
  USBSerial.println("Touch sensor initialized");

  // SWITCHES
  pinMode(SW_A, INPUT_PULLUP);
  pinMode(SW_B, INPUT_PULLUP);

  // Game Engine
  eisei.Init();
  USBSerial.println("Game initialized");

  // I2C
  Wire.begin(43, 44, 400000);
}
float value = 0.0f;
void loop()
{
  touch->Update();
  eisei.Update();
  eisei.Draw();
  eisei.LateUpdate();
  SharedData::base_mult = SharedData::base_mult + (touch->GetSpeed());
  value = SharedData::base_mult;
  byte *p = (byte *)&SharedData::base_mult;
  Wire.beginTransmission(0x11); // transmit to device #11
  for (int i = 0; i < sizeof(float); i++)
  {
    Wire.write(*p++);
  }
  Wire.endTransmission();
}