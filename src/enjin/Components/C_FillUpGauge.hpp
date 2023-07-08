#ifndef C_FILLUPGAUGE_HPP
#define C_FILLUPGAUGE_HPP

#include "enjin/Components/Component.hpp"
#include "enjin/Components/C_Drawable.hpp"
#include "enjin/Components/C_Position.hpp"
#include <Adafruit_GFX.h>
#include <memory>
#include <iostream>

enum class GaugeMode
{
    Unidirectional,
    Bidirectional
};

class C_FillUpGauge : public C_Drawable
{
public:
    C_FillUpGauge(Object *owner, uint16_t width, uint16_t height, uint16_t color, GaugeMode mode);
    void Awake() override;
    void Draw(GFXcanvas8 &canvas) override;
    bool ContinueToDraw() const override;
    void SetValue(float value);
    float GetValue() const;
    void LateUpdate(uint8_t deltaTime) override;

private:
    uint16_t width;
    uint16_t height;
    uint16_t color;
    float currentValue;
    GFXcanvas8 internalCanvas;
    GFXcanvas1 mask;
    GaugeMode mode;
};

#endif// C_FILLUPGAUGE_HPP
