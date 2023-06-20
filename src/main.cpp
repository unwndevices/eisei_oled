

#include <Arduino.h>
#include "USB.h"

#include "HardwareManager.hpp"
#include "utils/utils.h"

#include "SharedData.hpp"
#include "Game.hpp"

Hardware hw;
Game eisei(hw);

void transmitI2C()
{
  byte *p = (byte *)&system_data;
  for (int i = 0; i < sizeof(SharedData); i++)
  {
    Wire.write(*p++);
  }
}

byte buffer[4 * sizeof(float)];
float *floatArray = (float *)buffer;

void receiveI2C(int byteCount)
{
  int i = 0;

  while (Wire.available())
  {
    buffer[i++] = Wire.read();
  }

  // reinterpret byte array as float array
  float *floatArray = (float *)buffer;
  // Print the received float values
}

void setup()
{
  // I2C
  Wire.begin(0x11, 43, 44, 400000);
  Wire.onRequest(transmitI2C);
  Wire.onReceive(receiveI2C);
  // SERIAL
  USBSerial.begin(115200);
  USBSerial.setDebugOutput(true);
  hw.Init();
  //  the Game Engine has to be initialized after the hardware
  eisei.Init();
  log_d("Game initialized");
}

void loop()
{
  for (int i = 0; i < 4; i++)
  {
    phase[i] = floatArray[i];
  }
  hw.Update();
  eisei.ProcessInput();
  eisei.Update();
  eisei.Draw();
  eisei.LateUpdate();
}
