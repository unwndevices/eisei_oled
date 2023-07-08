#ifndef C_CANVAS_HPP
#define C_CANVAS_HPP
#include <memory>

#include <Adafruit_GFX.h>
#include "Component.hpp"
#include "C_Drawable.hpp"
#include "C_Position.hpp"

class C_Canvas : public C_Drawable
{
public:
    C_Canvas(Object *owner, uint8_t width, uint8_t height);
    void Draw(GFXcanvas8 &canvas);
    bool ContinueToDraw() const override;
    void LateUpdate(uint8_t deltaTime) override;

    GFXcanvas8 _canvas;

    };

#endif// C_CANVAS_HPP
