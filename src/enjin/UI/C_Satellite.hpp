#ifndef C_SATELLITE_HPP
#define C_SATELLITE_HPP

#include <memory>
#include "enjin/Object.hpp"
#include "enjin/Components/Component.hpp"
#include "enjin/Components/C_Drawable.hpp"
#include "enjin/Components/C_Position.hpp"
#include <Adafruit_GFX.h>
#include "enjin/utils/Polar.hpp"

using namespace Polar;

class C_Satellite : public Component, public C_Drawable
{
public:
    C_Satellite(Object *owner, uint8_t from_center, uint8_t radius, uint8_t color);
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

private:
    float phase;
    uint8_t radius, from_center, identity;
    Vector2 _position, canvas_position;
    GFXcanvas8 internalCanvas;
    static uint8_t amount;
    static Vector2 abs_center;
    std::shared_ptr<C_Position> position;
    uint8_t color;
    bool is_planet;
};

#endif // !C_SATELLITE_HPP
