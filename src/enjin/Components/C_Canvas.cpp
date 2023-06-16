#include "C_Canvas.hpp"
#include "enjin/Object.hpp"

C_Canvas::C_Canvas(Object *owner, uint8_t width, uint8_t height)
    : Component(owner), _canvas(width, height), _width(width), _height(height), _matte(0x8), _position{0, 0}
{
}

void C_Canvas::Draw(GFXcanvas8 &canvas)
{
    canvas.drawGrayscaleBitmap(_position.x, _position.y, _canvas.getBuffer(), _matte, _width, _height);
}

bool C_Canvas::ContinueToDraw() const
{
    return !owner->IsQueuedForRemoval();
}

void C_Canvas::LateUpdate(uint8_t deltaTime)
{
}