#include <iostream>

#include "C_Satellite.hpp"
#include "enjin/Object.hpp"

uint8_t C_Satellite::amount = 0;
Vector2 C_Satellite::abs_center(64, 64);

C_Satellite::C_Satellite(Object *owner, uint8_t from_center, uint8_t radius, uint8_t color) : C_Drawable(127, 127), Component(owner),
                                                                                              phase(0.0f), from_center(from_center),
                                                                                              radius(radius),
                                                                                              color(color),
                                                                                              is_planet(false)
{
    this->position = owner->GetComponent<C_Position>();
    if (!this->position)
    {
        std::cerr << "C_Satellite requires C_Position component.\n";
    }
};

void C_Satellite::Awake()
{
    C_Satellite::amount++;
    identity = C_Satellite::amount;
    if (from_center == 0)
    {
        is_planet = true;
    }
    else
    {
        sat_position = RadialToCartesian(phase, from_center, abs_center);
    }
}

void C_Satellite::GenerateSatellite(GFXcanvas8 &canvas)
{
    if (!is_planet)
    {
        canvas.fillCircle(sat_position.x, sat_position.y, radius + 1, 0);
        canvas.fillCircle(sat_position.x, sat_position.y, radius, color);
    }
    else if (is_planet)
    {
        canvas.fillCircle(GetOffsetPosition().x, GetOffsetPosition().y, radius + 1, 0);
        canvas.fillCircle(GetOffsetPosition().x, GetOffsetPosition().y, radius, color);
        canvas.fillCircle(GetOffsetPosition().x, GetOffsetPosition().y, radius - 1, 0);
        canvas.fillCircle(GetOffsetPosition().x, GetOffsetPosition().y, radius - 3, 2);
        canvas.fillCircle(GetOffsetPosition().x, GetOffsetPosition().y, radius - 5, 4);
        canvas.fillCircle(GetOffsetPosition().x, GetOffsetPosition().y, radius - 8, 6);
    }
}

void C_Satellite::Update(uint8_t deltaTime)
{
    if (!is_planet)
    {
        sat_position = RadialToCartesian(phase, from_center, abs_center);
    }
};
void C_Satellite::Draw(GFXcanvas8 &canvas)
{
    DrawBackground(canvas);
    GenerateSatellite(canvas);
};

bool C_Satellite::ContinueToDraw() const
{
    return !owner->IsQueuedForRemoval();
}

void C_Satellite::DrawOrbit(GFXcanvas8 &canvas)
{
    canvas.drawCircle(64, 64, from_center - 1, 0);
    canvas.drawCircle(64, 64, from_center, 2);
    canvas.drawCircle(64, 64, from_center + 1, 0);
}

void C_Satellite::DrawBackground(GFXcanvas8 &canvas)
{
    if (!is_planet)
    {
        DrawOrbit(canvas);
    }
}
