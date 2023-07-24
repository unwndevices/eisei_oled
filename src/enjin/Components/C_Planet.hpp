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
    void Update(uint8_t deltaTime) override
    {
        // speed is in hz (cycles 0-1 per second), use deltaTime to calculate the phase
        phase += speed * (deltaTime / 1000.0f);
        phase = fmod(phase, 1.0f);
    };
    void Draw(GFXcanvas8 &canvas) override
    {
        DrawPlanet();
        canvas.drawGrayscaleBitmap(GetOffsetPosition().x, GetOffsetPosition().y, internalCanvas.getBuffer(), 16U, internalCanvas.width(), internalCanvas.height());
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

    void GenerateSphericalMap(std::vector<Vector2> &map, uint8_t radius);

    void SetRadius(uint8_t radius)
    {
        this->radius = radius;
        DrawPlanet();
    }

    void SetSpeed(float speed)
    {
        this->speed = speed / 1000.0f;
    }

    void GenerateTerrain();
    void GenerateSky();

private:
    float phase, speed;
    uint8_t radius;
    GFXcanvas8 internalCanvas, textureCanvas, skyCanvas;
    std::vector<Vector2> sphericalMap;
    std::vector<Vector2> skyMap;

    void DrawPlanet();
};

#endif// C_PLANET_HPP
