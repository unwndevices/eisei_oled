#include <iostream>
#include "C_Canvas.hpp"
#include "enjin/Object.hpp"

C_Canvas::C_Canvas(Object *owner, uint8_t width, uint8_t height)
    : C_Drawable(width, height), Component(owner), _canvas(width, height)
{
    position = owner->GetComponent<C_Position>();
    if (!position)
    {
        std::cerr << "C_Satellite requires C_Position component.\n";
    }
}

void C_Canvas::Draw(GFXcanvas8 &canvas)
{
    switch (GetBlendMode())
    {
    case BlendMode::Normal:
        canvas.drawGrayscaleBitmap(GetOffsetPosition().x, GetOffsetPosition().y, _canvas.getBuffer(), 16U, _canvas.width(), _canvas.height());
        break;
    case BlendMode::Add:
        canvas.add(_canvas.getBuffer());
        break;
    case BlendMode::Sub:
        canvas.subtract(_canvas.getBuffer());
        break;
    default:
        break;
    }
}

bool C_Canvas::ContinueToDraw() const
{
    return !owner->IsQueuedForRemoval();
}

void C_Canvas::LateUpdate(uint8_t deltaTime)
{
}