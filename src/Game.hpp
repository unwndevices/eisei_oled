#ifndef GAME_HPP
#define GAME_HPP

#include <Arduino.h>
#include <Adafruit_SSD1327.h>
//#include <Fonts/VGATypewriter.h>
#include "HardwareManager.hpp"

#include "enjin/utils/Timer.hpp"

#include "scenes/SceneSplashScreen.hpp"
#include "scenes/SceneRatio.hpp"
#include "scenes/SceneMain.hpp"

class Game
{
public:
    Game(Hardware &hw);
    void Init();
    void ProcessInput();
    void Update();
    void LateUpdate();
    void Draw();
    void CalculateDeltaTime();
    bool IsRunning() const;
    Timer timer;

private:
    Adafruit_SSD1327 display;
    SceneStateMachine sceneStateMachine; // New
    Hardware hw;
};

Game::Game(Hardware &hw) : display(128, 128, &SPI, OLED_DC, OLED_RESET, OLED_CS, 37000000), hw(hw)
{
    Scene::setHardware(hw);
    std::shared_ptr<SceneSplashScreen> splashScreen = std::make_shared<SceneSplashScreen>(sceneStateMachine);
    std::shared_ptr<SceneMain> mainPage = std::make_shared<SceneMain>(sceneStateMachine);
    std::shared_ptr<SceneRatio> ratioPage = std::make_shared<SceneRatio>(sceneStateMachine);
    uint8_t splashScreenID = sceneStateMachine.Add(splashScreen);
    uint8_t mainPageID = sceneStateMachine.Add(mainPage);
    uint8_t ratioPageID = sceneStateMachine.Add(ratioPage);
    sceneStateMachine.SwitchTo(mainPageID);
}

void Game::Init()
{
    USBSerial.println("Game initialization");
    // Initialize Screen
    SPI.begin(OLED_CLK, -1, OLED_MOSI, OLED_CS);
    display.begin();
    display.setContrast(127 / 2);
    // rotate 180deg
    display.setRotation(2);

    //display.setFont(&VGATypewriter8pt7b);
    display.setTextColor(15);
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
    display.display();
}

#endif // GAME_HPP
