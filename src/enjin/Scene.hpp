#ifndef SCENE_HPP
#define SCENE_HPP
#include <Arduino.h>
#include <Adafruit_SSD1327.h>
#include "HardwareManager.hpp"

class Scene
{
public:
    Scene() : buffer(128, 128){};
    // Called when scene initially created. Called once.
    virtual void OnCreate() = 0;

    // Called when scene destroyed. Called at most once (if a scene
    // is not removed from the game, this will never be called).
    virtual void OnDestroy() = 0;

    // Called whenever a scene is transitioned into. Can be
    // called many times in a typical game cycle.
    virtual void OnActivate(){};

    // Called whenever a transition out of a scene occurs.
    // Can be called many times in a typical game cycle.
    virtual void OnDeactivate(){};

    // The below functions can be overridden as necessary in our scenes.
    virtual void ProcessInput(){};
    virtual void Update(uint16_t deltaTime){};
    virtual void LateUpdate(uint16_t deltaTime){};
    virtual void Draw(Adafruit_SSD1327 &display){};
    static void setHardware(Hardware &hw) { Scene::hw = &hw; };

protected:
    GFXcanvas8 buffer;
    static Hardware *hw;
    float fps = 0.0f;
};


#endif // SCENE_HPP