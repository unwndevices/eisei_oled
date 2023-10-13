#ifndef GAME_HPP
#define GAME_HPP

#include <Arduino.h>
#include "SharedContext.hpp"
#include "enjin/utils/Timer.hpp"

#include "scenes/SceneSplashScreen.hpp"
#include "scenes/BaseScene.hpp"
#include "scenes/SceneGravity.hpp"
#include "scenes/SceneOrbit.hpp"
#include "scenes/SceneScope.hpp"
#include "scenes/SceneRatio.hpp"
#include "scenes/SceneAttenuverter.hpp"
#include "scenes/SceneMode.hpp"

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

    void SetLed(Scenes scene);

private:
    Display &display;
    SceneStateMachine sceneStateMachine;
    InterfaceManager &interface;
    Hardware &hw;
    Data &data;
    SharedContext &context;

    ulong touch_timeout = 8000;

    std::shared_ptr<SceneAttenuverter> attenuverterPage;
    GFXcanvas8 screenShotCanvas = GFXcanvas8(128, 128);

    std::map<Scenes, int> sceneLed;
    Scenes currentScene = Scenes::Base;
};

Game::Game(SharedContext &context) : context(context), interface(context.interface), hw(interface.hw), data(context.data), display(context.interface.hw.GetDisplay())
{
}

void Game::ProcessButton(int id, Button::State state)
{
    log_d("Button: %d, state: %d", id, state);
    if (state == Button::State::CLICKED)
    {
        switch (id)
        {
        case Scenes::Gravity:
            currentScene = Scenes::Gravity;
            sceneStateMachine.SwitchTo((uint8_t)currentScene);
            break;
        case Scenes::Orbit:
            currentScene = Scenes::Orbit;
            sceneStateMachine.SwitchTo((uint8_t)currentScene);
            break;
        case Scenes::Scope:
            currentScene = Scenes::Scope;
            sceneStateMachine.SwitchTo((uint8_t)currentScene);
            break;
        case Scenes::Ratio:
            if (currentScene == Scenes::Ratio)
            {
                currentScene = Scenes::Mode;
            }
            else
                currentScene = Scenes::Ratio;
            sceneStateMachine.SwitchTo((uint8_t)currentScene);
            break;
        case SW_A:
        case SW_B:
        case SW_C:
        case SW_D:
            currentScene = Scenes::Attenuverter;
            attenuverterPage->SetData(id);
            sceneStateMachine.SwitchTo((uint8_t)currentScene);
            break;
        default:
            break;
        }
        hw.GetLeds().StopBlinkAll();
        hw.GetLeds().StopHoldAll();
        hw.GetLed(sceneLed[currentScene]).set((int16_t)512, (int16_t)-1);
    }

    else if (state == Button::State::LONG_PRESSED)
    {
        if (id < SW_A)
        {
            currentScene = Scenes::Attenuverter;
            attenuverterPage->SetData(id);
            sceneStateMachine.SwitchTo((uint8_t)currentScene);
            hw.GetLeds().SetSequential(0.0f);
            hw.GetLeds().StopBlinkAll();
            hw.GetLeds().SetBlink(sceneLed[(Scenes)id], 500, 200);
        }
    }

    else if (state == Button::State::RELEASED)
    {
        hw.GetLeds().SetSequential(0.0f);
    }
}

void Game::Init()
{
    log_d("Game initialization");
    int current_free_heap = ESP.getFreeHeap();
    int used_heap = 0;

    std::shared_ptr<SceneSplashScreen> splashScreen = std::make_shared<SceneSplashScreen>(sceneStateMachine, context);
    uint8_t splashScreenID = sceneStateMachine.Add(splashScreen);

    used_heap = current_free_heap - ESP.getFreeHeap();
    log_d("RAM USAGE: %d", used_heap);

    std::shared_ptr<BaseScene> basePage = std::make_shared<BaseScene>(sceneStateMachine, context);
    uint8_t basePageID = sceneStateMachine.Add(basePage);
    used_heap = current_free_heap - ESP.getFreeHeap();
    log_d("RAM USAGE: %d", used_heap);
    current_free_heap = ESP.getFreeHeap();

    std::shared_ptr<SceneGravity> gravityPage = std::make_shared<SceneGravity>(sceneStateMachine, context);
    uint8_t gravityPageID = sceneStateMachine.Add(gravityPage);

    used_heap = current_free_heap - ESP.getFreeHeap();
    log_d("RAM USAGE: %d", used_heap);
    current_free_heap = ESP.getFreeHeap();

    std::shared_ptr<SceneOrbit> orbitPage = std::make_shared<SceneOrbit>(sceneStateMachine, context);
    uint8_t orbitPageID = sceneStateMachine.Add(orbitPage);

    used_heap = current_free_heap - ESP.getFreeHeap();
    log_d("RAM USAGE: %d", used_heap);
    current_free_heap = ESP.getFreeHeap();

    std::shared_ptr<SceneScope> scopePage = std::make_shared<SceneScope>(sceneStateMachine, context);
    uint8_t scopePageID = sceneStateMachine.Add(scopePage);

    used_heap = current_free_heap - ESP.getFreeHeap();
    log_d("RAM USAGE: %d", used_heap);
    current_free_heap = ESP.getFreeHeap();

    std::shared_ptr<SceneRatio> ratioPage = std::make_shared<SceneRatio>(sceneStateMachine, context);
    uint8_t ratioPageID = sceneStateMachine.Add(ratioPage);

    used_heap = current_free_heap - ESP.getFreeHeap();
    log_d("RAM USAGE: %d", used_heap);
    current_free_heap = ESP.getFreeHeap();

    attenuverterPage = std::make_shared<SceneAttenuverter>(sceneStateMachine, context);
    uint8_t attenuverterPageID = sceneStateMachine.Add(attenuverterPage);

    used_heap = current_free_heap - ESP.getFreeHeap();
    log_d("RAM USAGE: %d", used_heap);
    current_free_heap = ESP.getFreeHeap();

    std::shared_ptr<SceneMode> modePage = std::make_shared<SceneMode>(sceneStateMachine, context);
    uint8_t scenePageID = sceneStateMachine.Add(modePage);

    used_heap = current_free_heap - ESP.getFreeHeap();
    log_d("RAM USAGE: %d", used_heap);
    log_d("free heap: %d", ESP.getFreeHeap());

    sceneStateMachine.SwitchTo(splashScreenID);

    context.touch_timer.Restart();
    context.timer.Restart();

    sceneLed[Scenes::Base] = -1;
    sceneLed[Scenes::Gravity] = 0;
    sceneLed[Scenes::Orbit] = 1;
    sceneLed[Scenes::Scope] = 2;
    sceneLed[Scenes::Ratio] = 3;
    sceneLed[Scenes::Attenuverter] = 4;
    sceneLed[Scenes::Splashscreen] = -1;

    hw.AddButton(Scenes::Gravity, Button(buttonPins[0]));
    hw.AddButton(Scenes::Orbit, Button(buttonPins[1]));
    hw.AddButton(Scenes::Scope, Button(buttonPins[2]));
    hw.AddButton(Scenes::Ratio, Button(buttonPins[3]));
    hw.AddButton(SW_A, Button(buttonPins[4]));
    hw.AddButton(SW_B, Button(buttonPins[5]));
    hw.AddButton(SW_C, Button(buttonPins[6]));
    hw.AddButton(SW_D, Button(buttonPins[7]));

    hw.GetButton(Scenes::Gravity).onStateChanged.Connect(std::bind(&Game::ProcessButton, this, std::placeholders::_1, std::placeholders::_2));
    hw.GetButton(Scenes::Orbit).onStateChanged.Connect(std::bind(&Game::ProcessButton, this, std::placeholders::_1, std::placeholders::_2));
    hw.GetButton(Scenes::Scope).onStateChanged.Connect(std::bind(&Game::ProcessButton, this, std::placeholders::_1, std::placeholders::_2));
    hw.GetButton(Scenes::Ratio).onStateChanged.Connect(std::bind(&Game::ProcessButton, this, std::placeholders::_1, std::placeholders::_2));
    hw.GetButton(SW_A).onStateChanged.Connect(std::bind(&Game::ProcessButton, this, std::placeholders::_1, std::placeholders::_2));
    hw.GetButton(SW_B).onStateChanged.Connect(std::bind(&Game::ProcessButton, this, std::placeholders::_1, std::placeholders::_2));
    hw.GetButton(SW_C).onStateChanged.Connect(std::bind(&Game::ProcessButton, this, std::placeholders::_1, std::placeholders::_2));
    hw.GetButton(SW_D).onStateChanged.Connect(std::bind(&Game::ProcessButton, this, std::placeholders::_1, std::placeholders::_2));
}

void Game::ProcessInput()
{

    sceneStateMachine.ProcessInput();
    if (context.touch_timer.IsElapsed(touch_timeout))
    {
        sceneStateMachine.SwitchTo(1);
        hw.GetLeds().SetLeds(0.0f);
        hw.GetLeds().StopBlinkAll();
        hw.GetLeds().StopHoldAll();
    }
    else
    {
        data.interfaceDataChanged = true;
        data.cvDataChanged = true;
    }

    for (int i = 2; i <= (int)Scenes::Ratio; i++)
    {
        if (hw.GetButton(i).GetState() == Button::State::PRESSED)
        {
            hw.GetLeds().SetSequential(hw.GetButton(i).GetHoldTimeNormalized());
        }
    }
}

void Game::Update()
{
    sceneStateMachine.Update(context.timer.GetDeltaTime());
}

void Game::LateUpdate()
{
    sceneStateMachine.LateUpdate(context.timer.GetDeltaTime());
    context.timer.CalculateDeltaTime();
    context.touch_timer.CalculateDeltaTime();
    for (int i = 0; i < 4; i++)
    {
        hw.GetLeds().sats[i].set((int16_t)(context.data.output_value[i] >> 6));
    }
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

#endif // GAME_HPP
