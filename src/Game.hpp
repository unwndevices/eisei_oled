#ifndef Game_hpp
#define Game_hpp

#include <Arduino.h>
#include <Adafruit_SSD1351.h>
#include <Fonts/VGATypewriter.h>
#include "drivers/pinout.h"

#include "utils/Timer.hpp"

#include "scenes/SceneSplashScreen.hpp"
#include "scenes/SceneRatio.hpp"
#include "scenes/SceneMain.hpp"

class Game
{
public:
    Game();
    void Init();
    void ProcessInput();
    void Update();
    void LateUpdate();
    void Draw();
    void CalculateDeltaTime();
    bool IsRunning() const;
    Timer timer;

private:
    Adafruit_SSD1351 display;
    SceneStateMachine sceneStateMachine; // New
};

Game::Game() : display(128, 128, &SPI, OLED_CS, OLED_DC, OLED_RESET)
{
    std::shared_ptr<SceneSplashScreen> splashScreen = std::make_shared<SceneSplashScreen>(sceneStateMachine);
    std::shared_ptr<SceneRatio> ratioPage = std::make_shared<SceneRatio>(sceneStateMachine);
    std::shared_ptr<SceneMain> mainPage = std::make_shared<SceneMain>(sceneStateMachine);
    uint8_t splashScreenID = sceneStateMachine.Add(splashScreen);
    uint8_t ratioPageID = sceneStateMachine.Add(ratioPage);
    uint8_t mainPageID = sceneStateMachine.Add(mainPage);
    sceneStateMachine.SwitchTo(mainPageID);
    int var = 4;
}

void Game::Init()
{
    // Initialize Screen
    USBSerial.println("Game initialization");
    SPI.begin(OLED_CLK, -1, OLED_MOSI, OLED_CS);

    display.begin(40000000);

    display.fillScreen(0xF800);
    delay (200);
    // rotate 180deg
    display.setRotation(1);
    display.setFont(&VGATypewriter8pt7b);
    display.setTextColor(0xFFFF);
    timer.Restart();
}

void Game::ProcessInput()
{
    sceneStateMachine.ProcessInput();
}

void Game::Update()
{
    sceneStateMachine.Update(timer.GetDeltaTime());
}

void Game::LateUpdate()
{
    sceneStateMachine.LateUpdate(timer.GetDeltaTime());
    timer.CalculateDeltaTime();
}

void Game::Draw()
{
    sceneStateMachine.Draw(display);
    //display.display();
}

#endif /* Game_hpp */