

#include <Arduino.h>
#include "USB.h"

#include <WiFi.h>
#include <WiFiMulti.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include "utils/utils.h"

#include "SharedContext.hpp"
#include "Game.hpp"

WiFiMulti wifiMulti;
SemaphoreHandle_t interfaceMutex;

SharedContext context;

Game eisei(context);

void interfaceUpdateTask(void *parameter);
void serialTask(void *parameter);
void otaTask(void *parameter);

void otaInit();

void setup()
{
  context.interface.Init();

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

  log_d("Hardware initialized");
  log_d("Heap: %d", ESP.getFreeHeap());

  xTaskCreatePinnedToCore(
      otaTask,   /* Task function. */
      "OTATask", /* name of task. */
      10000,     /* Stack size of task */
      NULL,      /* parameter of the task */
      6,         /* priority of the task */
      NULL,      /* Task handle to keep track of created task */
      0);        /* Core where the task should run */

  //  the Game Engine has to be initialized after the hardware

  eisei.Init();

  log_d("Game initialized");
  log_d("Heap: %d", ESP.getFreeHeap());
}

void loop()
{

  if (context.data.current_state.state != RunState::RUNNING)
  {
    context.data.stateDataChanged = true;
  }

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
      if (command == "modes")
      {
        for (int i = 0; i < 4; i++)
        {
          log_d("Mode: %s, Type: %s", context.data.modes[i].name, context.data.modes[i].type);
        }
      }

      if (command == "state")
      {
        log_d("Current State: %d", context.data.current_state.state);
        log_d("Current Mode: %d", context.data.current_state.current_mode);
        for (uint8_t i = 0; i < 4; i++)
        {
          log_d("Output %d: %d", i, context.data.output_value[i]);
        }
      }
    }
    vTaskDelay(50); // Or any delay you need
  }
}

void otaTask(void *parameter)
{
  otaInit();
  // OTA
  for (;;)
  {
    ArduinoOTA.handle();

    vTaskDelay(10); // Or any delay you need
  }
}

void otaInit()
{
  // OTA
  WiFi.mode(WIFI_STA);
  wifiMulti.addAP("Uaifai", "tuttomaiuscolo");
  wifiMulti.addAP("TIM-26379166", "arturone");
  wifiMulti.addAP("Ciccio", "davidecane");

  // Connect to Wi-Fi using wifiMulti (connects to the SSID with strongest connection)
  Serial.println("Connecting Wifi...");
  if (wifiMulti.run() == WL_CONNECTED)
  {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }
  if (WiFi.status() != WL_CONNECTED)
  {
    WiFi.disconnect(true);
    wifiMulti.run();
  }

  ArduinoOTA.setHostname("eisei");
  // ArduinoOTA.setPort(55910);

  ArduinoOTA
      .onStart([]()
               {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type); })
      .onEnd([]()
             { Serial.println("\nEnd"); })
      .onProgress([](unsigned int progress, unsigned int total)
                  { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); })
      .onError([](ota_error_t error)
               {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed"); });

  ArduinoOTA.begin();

  log_d("OTA Initialized");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}