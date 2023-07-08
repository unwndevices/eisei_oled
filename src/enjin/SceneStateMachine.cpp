#include "SceneStateMachine.hpp"

void SceneStateMachine::ProcessInput()
{

    if (curScene)
    {
        curScene->ProcessInput();
    }
}

void SceneStateMachine::Update(uint16_t delta_time)
{
    if (curScene)
    {
        curScene->AddToTimer(delta_time);
        curScene->Update(delta_time);
    }
}

void SceneStateMachine::LateUpdate(uint16_t delta_time)
{
    if (curScene)
    {
        curScene->LateUpdate(delta_time);
    }
}

void SceneStateMachine::Draw(Display &display)
{
    if (curScene)
    {
        curScene->Draw(display);
    }
}

uint8_t SceneStateMachine::Add(Scene *scene)
{
    auto inserted = scenes.insert(std::make_pair(insertedSceneID, scene));

    insertedSceneID++;

    inserted.first->second->OnCreate();

    return insertedSceneID - 1;
}

void SceneStateMachine::Remove(uint8_t id)
{
    auto it = scenes.find(id);
    if (it != scenes.end())
    {
        if (curScene == it->second)
        {
            // If the scene we are removing is the current scene,
            // we also want to set that to a null pointer so the scene
            // is no longer updated.
            curScene = nullptr;
        }

        // We make sure to call the OnDestroy method
        // of the scene we are removing.
        it->second->OnDestroy();

        scenes.erase(it);
    }
}

void SceneStateMachine::SwitchTo(uint8_t id)
{
    auto it = scenes.find(id);
    if (it != scenes.end())
    {
        if (curScene)
        {
            curScene->OnDeactivate();
        }

        curScene = it->second;

        curScene->OnActivate();
    }
}
