

#include <Arduino.h>
#include "USB.h"

#include "utils/utils.h"

#include "SharedContext.hpp"
#include "Game.hpp"

SharedContext context;

Game eisei(context);

void setup()
{
  // SERIAL
  USBSerial.begin(115200);
  USBSerial.setDebugOutput(true);
  // I2C
  context.interface.Init();
  //  the Game Engine has to be initialized after the hardware
  eisei.Init();
  log_d("Game initialized");
}

void loop()
{

  context.interface.Update();

  eisei.ProcessInput();
  eisei.Update();
  eisei.Draw();
  eisei.LateUpdate();

  if (USBSerial.available())
  {
    String command = USBSerial.readStringUntil('\n');
    command.trim(); // remove any trailing whitespace
    if (command == "screenshot")
    {
      eisei.ScreenShot();
    }
  }
}
