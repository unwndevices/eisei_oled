#ifndef C_PROBE_HPP
#define C_PROBE_HPP

#include <memory>
#include "enjin/Object.hpp"
#include "enjin/Components/Component.hpp"
#include "enjin/Components/C_Drawable.hpp"
#include "enjin/Components/C_Position.hpp"
#include <Adafruit_GFX.h>
#include "enjin/utils/Polar.hpp"

using namespace Polar;

class C_Probe : public C_Drawable
{
public:
    C_Probe(Object *owner, uint8_t from_center, uint8_t radius, uint8_t color);
    void Awake() override;
    void Update(uint8_t deltaTime) override;
    void Draw(GFXcanvas8 &canvas) override;
    bool ContinueToDraw() const override;

    void DrawOrbit(GFXcanvas8 &canvas);
    void DrawBackground(GFXcanvas8 &canvas);
    void SetPhase(float amount) { phase = amount; };
    float GetPhase() { return phase; };
    static void SetAbsCenter(Vector2 position)
    {
        abs_center = position;
    }

    void SetDistance(uint8_t distance)
    {
        from_center = distance;
    }
    void SetRadius(uint8_t radius)
    {
        this->radius = radius;
    }

private:
    float phase;
    uint8_t radius, from_center, identity;
    Vector2 _position, probe_position;
    static uint8_t amount;
    static Vector2 abs_center;
    uint8_t color;

    void DrawProbe(GFXcanvas8 &canvas);
};

#endif// C_PROBE_HPP
