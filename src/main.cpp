

#include <Arduino.h>
#include "USB.h"

#include "utils/utils.h"

#include "I2CShare.hpp"
#include "Game.hpp"

Hardware hw;
InterfaceManager interface;

Game eisei(interface);

void setup()
{
  // SERIAL
  USBSerial.begin(115200);
  USBSerial.setDebugOutput(true);
  // I2C
  interface.Init();
  //  the Game Engine has to be initialized after the hardware
  eisei.Init();
  log_d("Game initialized");
}

void loop()
{
  interface.Update();
  eisei.ProcessInput();
  eisei.Update();
  eisei.Draw();
  eisei.LateUpdate();
}
