#include "C_Sprite.hpp"
#include "../Object.hpp"

C_Sprite::C_Sprite(Object *owner) : Component(owner)
{
}

void C_Sprite::Load(const uint8_t texture[], uint8_t width, uint8_t height, uint8_t matte)
{
    sprite.setPosition(owner->position->GetPosition());
    sprite.setTexture(texture, width, height, matte);
}

void C_Sprite::Load(const bool texture[], uint8_t width, uint8_t height, uint8_t matte)
{
    sprite.setPosition(owner->position->GetPosition());
    sprite.setTexture(texture, width, height, matte);
}

void C_Sprite::LoadFrame(const uint8_t texture[], uint8_t frameId)
{
    sprite.setTexture(texture, frameId);
}
void C_Sprite::LoadFrame(const bool texture[], uint8_t frameId)
{
    sprite.setTexture(texture, frameId);
}

void C_Sprite::LoadFrame(uint8_t frameId)
{
    sprite.setTexture(frameId);
}

void C_Sprite::Draw(GFXcanvas8 &canvas)
{
    sprite.draw(canvas);
}

bool C_Sprite::ContinueToDraw() const
{
    return !owner->IsQueuedForRemoval();
}

void C_Sprite::LateUpdate(uint8_t deltaTime)
{
    sprite.setPosition(owner->position->GetPosition());
}