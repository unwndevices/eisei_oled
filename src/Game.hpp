#ifndef GAME_HPP
#define GAME_HPP

#include <Arduino.h>
#include "InterfaceManager.hpp"

#include "enjin/utils/Timer.hpp"

#include "scenes/SceneSplashScreen.hpp"
#include "scenes/SceneRatio.hpp"
#include "scenes/SceneMain.hpp"

class Game
{
public:
    Game(InterfaceManager &interface);
    void Init();
    void ProcessInput();
    void Update();
    void LateUpdate();
    void Draw();

private:
    Display &display;
    SceneStateMachine sceneStateMachine; // New
    InterfaceManager &interface;

    Timer timer;
};

Game::Game(InterfaceManager &interface) : interface(interface), display(interface.hw.getDisplay())
{
    std::shared_ptr<SceneSplashScreen> splashScreen = std::make_shared<SceneSplashScreen>(sceneStateMachine, interface);
    std::shared_ptr<SceneMain> mainPage = std::make_shared<SceneMain>(sceneStateMachine,interface);
    std::shared_ptr<SceneRatio> ratioPage = std::make_shared<SceneRatio>(sceneStateMachine, interface);
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

#endif// GAME_HPP
