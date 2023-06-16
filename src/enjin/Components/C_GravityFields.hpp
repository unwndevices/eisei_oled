#ifndef C_GRAVITYFIELDS_HPP
#define C_GRAVITYFIELDS_HPP
#include <vector>
#include <memory>

#include "enjin/UI/Satellite.hpp"

#include "enjin/Components/Component.hpp"
#include "enjin/Components/C_Drawable.hpp"
#include "enjin/Components/C_Position.hpp"
#include <Adafruit_GFX.h>
#include "enjin/utils/Polar.hpp"

#include "enjin/Object.hpp"

class C_GravityFields : public Component, public C_Drawable
{
public:
    C_GravityFields(Object *owner, const std::vector<std::shared_ptr<Satellite>> &satellites) : Component(owner), canvas(127, 127), radius(33), color(5)
    {
        position = owner->GetComponent<C_Position>();

        if (!position)
        {
        }
        for (auto &satellite : satellites)
        {
            sat_positions.push_back(satellite->GetComponent<C_Position>());
        }
    };
    void Awake() override{};
    void Update(uint8_t deltaTime) override{};
    void Draw(GFXcanvas8 &canvas) override
    {
        this->canvas.fillScreen(7);
        for (auto &sat : sat_positions)
        {
            this->canvas.drawCircle(sat->GetPosition().x, sat->GetPosition().y, radius, color);
        }
        canvas.drawGrayscaleBitmap(0, 0, this->canvas.getBuffer(), (uint8_t)7, this->canvas.width(), this->canvas.height());
    };
    bool ContinueToDraw() const override
    {
        return !owner->IsQueuedForRemoval();
    };

    void DrawBackground(GFXcanvas8 &canvas){};

private:
    uint8_t radius;
    uint8_t color;

    GFXcanvas8 canvas;
    std::shared_ptr<C_Position> position;
    std::vector<std::shared_ptr<C_Position>> sat_positions;
};

#endif // C_GRAVITYFIELDS_HPP
