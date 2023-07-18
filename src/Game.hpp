#ifndef GAME_HPP
#define GAME_HPP

#include <Arduino.h>
#include "SharedContext.hpp"
#include "enjin/utils/Timer.hpp"

#include "scenes/SceneSplashScreen.hpp"
#include "scenes/SceneRatio.hpp"
#include "scenes/BaseScene.hpp"
#include "scenes/SceneGravity.hpp"
#include "scenes/SceneMass.hpp"
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
    void ScreenShot();

private:
    Display &display;
    SceneStateMachine sceneStateMachine;
    InterfaceManager &interface;
    SharedContext &context;
    Timer timer;
};

Game::Game(SharedContext &context) : context(context), interface(context.interface), display(context.interface.hw.GetDisplay())
{
}

void Game::ProcessButton(int id, Button::State state)
{
    if (state == Button::State::CLICKED)
        switch (id)
        {
        case SW_GRAVITY:
            sceneStateMachine.SwitchTo(3);
            break;
        // case SW_PHASE:
        //     break;
        // case SW_MASS:
        //     sceneStateMachine.SwitchTo(2);
        //     break;
        case SW_RATIO:
            sceneStateMachine.SwitchTo(2);
            break;

        default:
            break;
        }
}

void Game::Init()
{
    log_d("Game initialization");
    //  std::shared_ptr<SceneSplashScreen> splashScreen = std::make_shared<SceneSplashScreen>(sceneStateMachine, context);
    //  uint8_t splashScreenID = sceneStateMachine.Add(splashScreen);

    std::shared_ptr<BaseScene> basePage = std::make_shared<BaseScene>(sceneStateMachine, context);
    uint8_t basePageID = sceneStateMachine.Add(basePage);

    //std::shared_ptr<SceneGravity> gravityPage = std::make_shared<SceneGravity>(sceneStateMachine, context);
    //uint8_t gravityPageID = sceneStateMachine.Add(gravityPage);

    //std::shared_ptr<SceneMass> massPage = std::make_shared<SceneMass>(sceneStateMachine, context);
    //uint8_t massPageID = sceneStateMachine.Add(massPage);

    std::shared_ptr<SceneCvGravity> cvPage = std::make_shared<SceneCvGravity>(sceneStateMachine, context);
    uint8_t cvPageID = sceneStateMachine.Add(cvPage);

    std::shared_ptr<SceneRatio> ratioPage = std::make_shared<SceneRatio>(sceneStateMachine, context);
    uint8_t ratioPageID = sceneStateMachine.Add(ratioPage);
    std::shared_ptr<SceneMain> mainPage = std::make_shared<SceneMain>(sceneStateMachine, context);
    uint8_t mainPageID = sceneStateMachine.Add(mainPage);

    sceneStateMachine.SwitchTo(1);

    interface.hw.GetButton(Pages::Gravity).onStateChanged.Connect(std::bind(&Game::ProcessButton, this, std::placeholders::_1, std::placeholders::_2));
    interface.hw.GetButton(Pages::Phase).onStateChanged.Connect(std::bind(&Game::ProcessButton, this, std::placeholders::_1, std::placeholders::_2));
    interface.hw.GetButton(Pages::Mass).onStateChanged.Connect(std::bind(&Game::ProcessButton, this, std::placeholders::_1, std::placeholders::_2));
    interface.hw.GetButton(Pages::Ratio).onStateChanged.Connect(std::bind(&Game::ProcessButton, this, std::placeholders::_1, std::placeholders::_2));

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

void Game::ScreenShot()
{
    // Get the Adafruit GFX buffer
    uint8_t *buffer = display.GetBuffer();
    USBSerial.println(">img:");
    // Send the buffer over the serial connection
    for (int i = 0; i < 128 * 128 / 2; i++)
    {
        USBSerial.println(buffer[i]);
    }
    USBSerial.println("<");
}

#endif// GAME_HPP
