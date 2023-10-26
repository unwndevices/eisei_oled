#ifndef C_PROBE_RANGE_HPP
#define C_PROBE_RANGE_HPP

#include <iostream>
#include <memory>
#include "enjin/Object.hpp"
#include "enjin/Components/Component.hpp"
#include "enjin/Components/C_Drawable.hpp"
#include "enjin/Components/C_Position.hpp"
#include <Adafruit_GFX.h>
#include "enjin/utils/Polar.hpp"
using namespace Polar;

class C_ProbeRange : public C_Drawable
{
public:
    C_ProbeRange(Object *owner, uint8_t from_center, uint8_t radius, uint8_t color) : C_Drawable(127, 127), Component(owner),
                                                                                      phase(0.0f), from_center(from_center),
                                                                                      radius(radius),
                                                                                      color(color)
    {
        position = owner->GetComponent<C_Position>();
        if (!position)
        {
            std::cerr << "C_ProbeRange requires C_Position component.\n";
        }
    };

    void Awake() override
    {
        C_ProbeRange::amount++;
        identity = C_ProbeRange::amount;
        SetAnchorPoint(Anchor::CENTER);
        probe_position = RadialToCartesian(phase, from_center, abs_center);
    };

    void Update(uint8_t deltaTime) override
    {
        probe_position = RadialToCartesian(phase, from_center, abs_center);
    };

    void Draw(GFXcanvas8 &canvas) override
    {
        int8_t scaledColor = 0;
        if (GetBlendMode() == BlendMode::Normal)
        {
            scaledColor = color;
        }
        else if (GetBlendMode() == BlendMode::Opacity50)
        {
            scaledColor = color / 2;
        }
        else if (GetBlendMode() == BlendMode::Opacity25)
        {
            scaledColor = color / 4;
        }

        canvas.drawCircle(probe_position.x, probe_position.y, radius, scaledColor);
    };

    bool ContinueToDraw() const override
    {
        return !owner->IsQueuedForRemoval();
    };

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

    void SetLabel(String label)
    {
        this->label = label;
    }

private:
    float phase;
    uint8_t radius, from_center, identity;
    Vector2 _position, probe_position;

    static inline uint8_t amount = 0;
    static inline Vector2 abs_center = Vector2(64, 64);

    uint8_t color;

    String label;
};

#endif // C_PROBE_HPP
