#ifndef C_TICKMARKS_HPP
#define C_TICKMARKS_HPP

#include "enjin/Components/Component.hpp"
#include "enjin/Components/C_Drawable.hpp"
#include "enjin/Components/C_Position.hpp"
#include <Adafruit_GFX.h>
#include <memory>
#include <iostream>
class C_Tickmarks : public C_Drawable
{
public:
    C_Tickmarks(Object *owner, Vector2 center, int16_t startAngle, int16_t stopAngle, uint8_t spacing, uint8_t length, uint8_t radius);
     void Awake() override;
    void Draw(GFXcanvas8 &canvas) override;
    bool ContinueToDraw() const override;
    void SetValue(float value);
    float GetValue() const;

private:
    Vector2 center;
    int16_t startAngle;
    int16_t stopAngle;
    uint8_t spacing;
    uint8_t length, radius;
    float currentValue;
    GFXcanvas8 internalCanvas;
};

///////////////////////////////////////////////////////////////////

#endif// C_TICKMARKS_HPP
