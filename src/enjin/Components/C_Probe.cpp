#include <iostream>

#include "C_Probe.hpp"
#include "enjin/Object.hpp"

uint8_t C_Probe::amount = 0;
Vector2 C_Probe::abs_center(64, 64);

C_Probe::C_Probe(Object *owner, uint8_t from_center, uint8_t radius, uint8_t color) : C_Drawable(127, 127), Component(owner),
                                                                                      phase(0.0f), from_center(from_center),
                                                                                      radius(radius),
                                                                                      color(color)
{
    position = owner->GetComponent<C_Position>();
    if (!position)
    {
        std::cerr << "C_Probe requires C_Position component.\n";
    }
};

void C_Probe::Awake()
{
    C_Probe::amount++;
    identity = C_Probe::amount;
    SetAnchorPoint(Anchor::CENTER);
    probe_position = RadialToCartesian(phase, from_center, abs_center);
}

void C_Probe::DrawProbe(GFXcanvas8 &canvas)
{
    int8_t scaledColor = color - 4;
    canvas.fillCircle(probe_position.x, probe_position.y, radius, 0);
    for (int i = 1; i < 6; i++)
    {
        uint8_t interpolatedColor = scaledColor / 6 * i;
        int interpolatedRadius = radius - radius / 6 * i;
        canvas.fillCircle(probe_position.x, probe_position.y, interpolatedRadius, interpolatedColor);
    }
    canvas.drawCircle(probe_position.x, probe_position.y, radius, color);
    canvas.fillCircle(probe_position.x, probe_position.y, 2 + 1, 0);
    canvas.fillCircle(probe_position.x, probe_position.y, 2, color - 4);
}

void C_Probe::Update(uint8_t deltaTime)
{
    probe_position = RadialToCartesian(phase, from_center, abs_center);
};
void C_Probe::Draw(GFXcanvas8 &canvas)
{
    DrawProbe(canvas);
};

bool C_Probe::ContinueToDraw() const
{
    return !owner->IsQueuedForRemoval();
}

void C_Probe::DrawBackground(GFXcanvas8 &canvas)
{
}
