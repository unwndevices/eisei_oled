#ifndef SCENESTATEMACHINE_HPP
#define SCENESTATEMACHINE_HPP
#include <Arduino.h>

#include <memory>
#include <unordered_map>

#include "InterfaceManager.hpp"
#include "Scene.hpp"
class SceneStateMachine
{
public:
    SceneStateMachine() : scenes(0), curScene(0){};

    // ProcessInput, Update, LateUpdate, and Draw will simply be
    // pass through methods. They will call the correspondingly
    // named methods of the active scene.
    void ProcessInput();
    void Update(uint16_t delta_time);
    void LateUpdate(uint16_t delta_time);
    void Draw(Display &display);

    // Adds a scene to the state machine and returns the id of that scene.
    uint8_t Add(Scene *scene);

    // Transitions to scene with specified id.
    void SwitchTo(uint8_t id);

    // Removes scene from state machine.
    void Remove(uint8_t id);

private:
    // Stores all of the scenes associated with this state machine.
    std::unordered_map<uint8_t, Scene *> scenes;

    // Stores a reference to the current scene. Used when drawing/updating.
    Scene *curScene;
    // Stores our current scene id. This is incremented whenever
    // a scene is added.
    unsigned int insertedSceneID;
};

#endif // SCENESTATEMACHINE_HPP
