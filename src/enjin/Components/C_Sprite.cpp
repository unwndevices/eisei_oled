#include <iostream>

#include "C_Sprite.hpp"
#include "../Object.hpp"

C_Sprite::C_Sprite(Object *owner, uint8_t width, uint8_t height) : C_Drawable(width, height), Component(owner)
{
    position = owner->GetComponent<C_Position>();
    if (!position)
    {
        std::cerr << "C_Satellite requires C_Position component.\n";
    }
}

void C_Sprite::Load(const uint8_t texture[], uint8_t width, uint8_t height)
{
    sprite.setPosition(position->GetPosition());
    sprite.setTexture(texture, width, height);
}

void C_Sprite::LoadFrame(const uint8_t texture[], uint8_t frameId)
{
    sprite.setTexture(texture, frameId);
}

void C_Sprite::LoadFrame(uint8_t frameId)
{
    sprite.setTexture(frameId);
}

void C_Sprite::Draw(GFXcanvas8 &canvas)
{
    switch (GetBlendMode())
    {
    case BlendMode::Normal:
        canvas.drawGrayscaleBitmap(GetOffsetPosition().x, GetOffsetPosition().y, sprite.GetTexture(), sprite._matte, sprite._width, sprite._height);
        break;
    case BlendMode::Add:
        canvas.add(sprite.GetTexture());
        break;
    case BlendMode::Sub:
        canvas.subtract(sprite.GetTexture());
        break;
    default:
        break;
    }
}

bool C_Sprite::ContinueToDraw() const
{
    return !owner->IsQueuedForRemoval();
}

void C_Sprite::LateUpdate(uint8_t deltaTime)
{
}
