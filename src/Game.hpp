#ifndef GAME_HPP
#define GAME_HPP

#include <Arduino.h>
#include "SharedContext.hpp"
#include "enjin/utils/Timer.hpp"

#include "scenes/SceneSplashScreen.hpp"
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

    SceneSplashScreen splashScreen;
    SceneSplashScreen splashScreen1;
    SceneSplashScreen splashScreen2;
    SceneBase basePage;
    SceneCvGravity cvPage;
    SceneMain mainPage;
    SceneGravity gravityPage;
    SceneRatio ratioPage;
};

Game::Game(SharedContext &context) : context(context), interface(context.interface), display(context.interface.hw.GetDisplay()),
                                     splashScreen(sceneStateMachine, context),
                                     splashScreen1(sceneStateMachine, context),
                                     splashScreen2(sceneStateMachine, context),
                                     basePage(sceneStateMachine, context),
                                     cvPage(sceneStateMachine, context),
                                     mainPage(sceneStateMachine, context),
                                     gravityPage(sceneStateMachine, context),
                                     ratioPage(sceneStateMachine, context)
{

    uint8_t splashScreenID = sceneStateMachine.Add(&splashScreen);
    uint8_t basePageID = sceneStateMachine.Add(&basePage);
    uint8_t cvPageID = sceneStateMachine.Add(&cvPage);
    //uint8_t mainPageID = sceneStateMachine.Add(&mainPage);
    uint8_t gravityPageID = sceneStateMachine.Add(&gravityPage);
    uint8_t ratioPageID = sceneStateMachine.Add(&ratioPage);
    uint8_t splashScreen1ID = sceneStateMachine.Add(&splashScreen1);
    uint8_t splashScreen2ID = sceneStateMachine.Add(&splashScreen2);
    sceneStateMachine.SwitchTo(basePageID);

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

#endif // GAME_HPP
