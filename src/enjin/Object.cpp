#include "enjin/Object.hpp"

Object::Object() : queuedForRemoval(false)
{
    position = AddComponent<C_Position>();
}

void Object::Awake()
{
    for (int i = components.size() - 1; i >= 0; i--)
    {
        components[i]->Awake();
    }
}

void Object::Start()
{
    for (int i = components.size() - 1; i >= 0; i--)
    {
        components[i]->Start();
    }
}

void Object::Update(uint16_t deltaTime)
{
    for (int i = components.size() - 1; i >= 0; i--)
    {
        components[i]->Update(deltaTime);
    }
}

void Object::LateUpdate(uint16_t deltaTime)
{
    for (int i = components.size() - 1; i >= 0; i--)
    {
        components[i]->LateUpdate(deltaTime);
    }
}

void Object::Draw(GFXcanvas8 &canvas)
{
    for (int i = components.size() - 1; i >= 0; i--)
    {
        drawable->Draw(canvas);
    }
}

void Object::QueueForRemoval()
{
    queuedForRemoval = true;
}

bool Object::IsQueuedForRemoval()
{
    return queuedForRemoval;
}

std::shared_ptr<C_Drawable> Object::GetDrawable()
{
    return drawable;
}