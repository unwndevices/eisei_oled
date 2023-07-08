#ifndef C_SPRITE_HPP
#define C_SPRITE_HPP
#include <memory>
#include "Component.hpp"
#include "C_Drawable.hpp"
#include "../Sprite.hpp"
#include "C_Position.hpp"


class C_Sprite : public C_Drawable
{
public:
    C_Sprite(Object *owner, uint8_t width, uint8_t height);
    void Load(const uint8_t texture[], uint8_t width, uint8_t height);
    void LoadFrame(const uint8_t texture[], uint8_t frameId);
    void LoadFrame(uint8_t frameId);
    // We override the draw method so we can draw our sprite.
    void Draw(GFXcanvas8 &canvas) override;
    bool ContinueToDraw() const override;
    void LateUpdate(uint8_t deltaTime) override;

    void setMatte(uint8_t matte)
    {
        sprite.setMatte(matte);
    };

 

private:
    Sprite sprite;
    Anchor anchor;
};

#endif// C_SPRITE_HPP
