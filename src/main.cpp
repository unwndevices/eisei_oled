

#include <Arduino.h>
#include "USB.h"

#include "utils/utils.h"

#include "SharedContext.hpp"
#include "Game.hpp"

SemaphoreHandle_t interfaceMutex;

SharedContext context;

Game eisei(context);

void interfaceUpdateTask(void *parameter);
void serialTask(void *parameter);

void setup()
{

  context.interface.Init();
  log_d("Hardware initialized");
  log_d("Heap: %d", ESP.getFreeHeap());

  //  the Game Engine has to be initialized after the hardware
  eisei.Init();
  log_d("Game initialized");
  log_d("Heap: %d", ESP.getFreeHeap());

  interfaceMutex = xSemaphoreCreateMutex();

  xTaskCreatePinnedToCore(
      interfaceUpdateTask,   /* Task function. */
      "UpdateInterfaceTask", /* name of task. */
      10000,                 /* Stack size of task */
      NULL,                  /* parameter of the task */
      5,                     /* priority of the task */
      NULL,                  /* Task handle to keep track of created task */
      0);                    /* Core where the task should run */

  xTaskCreatePinnedToCore(
      serialTask,   /* Task function. */
      "SerialTask", /* name of task. */
      10000,        /* Stack size of task */
      NULL,         /* parameter of the task */
      1,            /* priority of the task */
      NULL,         /* Task handle to keep track of created task */
      0);           /* Core where the task should run */
}

void loop()
{

  if (xSemaphoreTake(interfaceMutex, (TickType_t)10) == pdTRUE)
  {
    eisei.ProcessInput();
    xSemaphoreGive(interfaceMutex);
  }

  eisei.Update();
  eisei.Draw();
  eisei.LateUpdate();
}

void interfaceUpdateTask(void *parameter)
{
  // Interface
  for (;;)
  {
    if (xSemaphoreTake(interfaceMutex, (TickType_t)10) == pdTRUE)
    {
      context.interface.Update();
      xSemaphoreGive(interfaceMutex);
    }
    vTaskDelay(10); // Or any delay you need
  }
}

void serialTask(void *parameter)
{
  // SERIAL
  USBSerial.begin(115200);
  USBSerial.setDebugOutput(true);
  for (;;)
  {
    if (USBSerial.available())
    {
      String command = USBSerial.readStringUntil('\n');
      command.trim(); // remove any trailing whitespace
      if (command == "screenshot")
      {
        eisei.ScreenShot();
      }
    }
    vTaskDelay(50); // Or any delay you need
  }
}