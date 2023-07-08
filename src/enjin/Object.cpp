#include "enjin/Object.hpp"

Object::Object() : queuedForRemoval(false)
{
    position = AddComponent<C_Position>();
}

void Object::Awake()
{
    for (const auto &component : components)
    {
        component->Awake();
    }
}

void Object::Start()
{
    for (const auto &component : components)
    {
        component->Start();
    }
}

void Object::Update(uint16_t deltaTime)
{
    for (const auto &component : components)
    {
        component->Update(deltaTime);
    }
}

void Object::LateUpdate(uint16_t deltaTime)
{
    for (const auto &component : components)
    {
        component->LateUpdate(deltaTime);
    }
}

// void Object::Draw(GFXcanvas8 &canvas)
// {
//     for (auto &drawable : drawables)
//     {
//         drawable->Draw(canvas);
//     }
// }

void Object::QueueForRemoval()
{
    queuedForRemoval = true;
}

bool Object::IsQueuedForRemoval()
{
    return queuedForRemoval;
}

std::vector<std::shared_ptr<C_Drawable>> Object::GetDrawables()
{
    return drawables;
}