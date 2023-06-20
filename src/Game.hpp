#ifndef GAME_HPP
#define GAME_HPP

#include <Arduino.h>
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

private:
    Display &display;
    SceneStateMachine sceneStateMachine; // New
    Hardware hw;
    Timer timer;
};

Game::Game(Hardware &hw) : hw(hw), display(hw.getDisplay())
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
    display.Show();
}

#endif // GAME_HPP
