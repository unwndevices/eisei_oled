#ifndef GAME_HPP
#define GAME_HPP

#include <Arduino.h>
#include "SharedContext.hpp"
#include "enjin/utils/Timer.hpp"

// #include "scenes/SceneSplashScreen.hpp"
#include "scenes/SceneRatio.hpp"
#include "scenes/SceneBase.hpp"
#include "scenes/SceneMain.hpp"
#include "scenes/SceneGravity.hpp"
#include "scenes/SceneCvGravity.hpp"

class Game
{
public:
    Game(SharedContext &context);
    void Init();
    void ProcessInput();
    void Update();
    void LateUpdate();
    void Draw();
    void ProcessButton(int id, Button::State state);

private:
    Display &display;
    SceneStateMachine sceneStateMachine;
    InterfaceManager &interface;
    SharedContext &context;
    Timer timer;
};

Game::Game(SharedContext &context) : context(context), interface(context.interface), display(context.interface.hw.GetDisplay())
{
    // std::shared_ptr<SceneSplashScreen> splashScreen = std::make_shared<SceneSplashScreen>(sceneStateMachine, context);
    //std::shared_ptr<SceneBase> basePage = std::make_shared<SceneBase>(sceneStateMachine, context);
    std::shared_ptr<SceneCvGravity> cvPage = std::make_shared<SceneCvGravity>(sceneStateMachine, context);
    std::shared_ptr<SceneMain> mainPage = std::make_shared<SceneMain>(sceneStateMachine, context);
    std::shared_ptr<SceneGravity> gravityPage = std::make_shared<SceneGravity>(sceneStateMachine, context);
    std::shared_ptr<SceneRatio> ratioPage = std::make_shared<SceneRatio>(sceneStateMachine, context);
    // uint8_t splashScreenID = sceneStateMachine.Add(splashScreen);
    //uint8_t basePageID = sceneStateMachine.Add(basePage);
    uint8_t cvPageID = sceneStateMachine.Add(cvPage);
    uint8_t mainPageID = sceneStateMachine.Add(mainPage);
    uint8_t gravityPageID = sceneStateMachine.Add(gravityPage);
    uint8_t ratioPageID = sceneStateMachine.Add(ratioPage);
    sceneStateMachine.SwitchTo(cvPageID);

    interface.hw.GetButton(Pages::Gravity).onStateChanged.Connect(std::bind(&Game::ProcessButton, this, std::placeholders::_1, std::placeholders::_2));
    interface.hw.GetButton(Pages::Phase).onStateChanged.Connect(std::bind(&Game::ProcessButton, this, std::placeholders::_1, std::placeholders::_2));
    interface.hw.GetButton(Pages::Mass).onStateChanged.Connect(std::bind(&Game::ProcessButton, this, std::placeholders::_1, std::placeholders::_2));
    interface.hw.GetButton(Pages::Ratio).onStateChanged.Connect(std::bind(&Game::ProcessButton, this, std::placeholders::_1, std::placeholders::_2));
}

void Game::ProcessButton(int id, Button::State state)
{
    if (state == Button::State::CLICKED)
        switch (id)
        {
        case SW_MASS:
            sceneStateMachine.SwitchTo(0);
            break;
        case SW_GRAVITY:
            sceneStateMachine.SwitchTo(2);
            break;
        case SW_RATIO:
            sceneStateMachine.SwitchTo(3);
            break;

        default:
            break;
        }
}

void Game::Init()
{
    log_d("Game initialization");

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
