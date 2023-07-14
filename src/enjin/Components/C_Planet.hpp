#ifndef C_PLANET_HPP
#define C_PLANET_HPP

#include <memory>
#include <vector>
#include "enjin/Object.hpp"
#include "enjin/Components/Component.hpp"
#include "enjin/Components/C_Drawable.hpp"
#include "enjin/Components/C_Position.hpp"
#include <Adafruit_GFX.h>
#include "enjin/utils/Polar.hpp"

using namespace Polar;

class C_Planet : public C_Drawable
{
public:
    C_Planet(Object *owner, uint8_t radius);
    void Awake() override{};
    void Update(uint8_t deltaTime) override{};
    void Draw(GFXcanvas8 &canvas) override
    {
        DrawPlanet();
        canvas.drawGrayscaleBitmap(GetOffsetPosition().x, GetOffsetPosition().x, internalCanvas.getBuffer(), 16U, internalCanvas.width(), internalCanvas.height());
    };

    bool ContinueToDraw() const override
    {
        return !owner->IsQueuedForRemoval();
    };

    void SetPhase(float amount)
    {
        phase = amount;
        DrawPlanet();
    };

    void InitSphericalMap(uint8_t radius);

    void SetRadius(uint8_t radius)
    {
        this->radius = radius;
        DrawPlanet();
    }
    
    void GenerateTerrain();

private:
    float phase;
    uint8_t radius;
    GFXcanvas8 internalCanvas, textureCanvas;
    std::vector<Vector2> sphericalMap;

    void DrawPlanet();
};

#endif // C_PLANET_HPP
