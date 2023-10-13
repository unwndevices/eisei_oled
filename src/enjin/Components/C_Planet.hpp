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

        uint8_t diameter = radius * 2 + 1;
        uint8_t difference = (canvas.width() - diameter) / 2 + 1;
        // draw the sphere
        for (int y = 0; y < diameter; ++y)
        {
            for (int x = 0; x < diameter; ++x)
            {
                Vector2 *_pos = &sphericalMap[y * diameter + x];
                int px = (_pos->x + (int)(phase * 127));
                int py = _pos->y;
                uint8_t color = 0;
                if (_pos->x < 0)
                {
                    color = 16;
                }
                else
                {
                    color = textureCanvas.getBuffer()[py * textureCanvas.width() + px];
                    canvas.drawPixel(x + difference, y + difference, color);
                }
            }
        }

        canvas.drawCircle(canvas.width() / 2, canvas.height() / 2, radius, 14);
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
    GFXcanvas8 textureCanvas;
    std::vector<Vector2> sphericalMap;
    std::vector<Vector2> skyMap;

    void DrawPlanet();
};

#endif// C_PLANET_HPP
