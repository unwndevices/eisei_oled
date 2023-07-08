#include <iostream>

#include "C_Satellite.hpp"
#include "enjin/Object.hpp"

uint8_t C_Satellite::amount = 0;
Vector2 C_Satellite::abs_center(64, 64);

C_Satellite::C_Satellite(Object *owner, uint8_t from_center, uint8_t radius, uint8_t color) : C_Drawable(radius * 2 + 3, radius * 2 + 3), Component(owner),
                                                                                              phase(0.0f),
                                                                                              internalCanvas(radius * 2 + 3, radius * 2 + 3),
                                                                                              from_center(from_center),
                                                                                              radius(radius),
                                                                                              color(color),
                                                                                              is_planet(false)
{
    position = owner->GetComponent<C_Position>();
    if (!position)
    {
        std::cerr << "C_Satellite requires C_Position component.\n";
    }
};

void C_Satellite::Awake()
{
    C_Satellite::amount++;
    identity = C_Satellite::amount;
    if (!from_center)
    {
        is_planet = true;
        _position = (Vector2)(abs_center, abs_center);
        position->SetPosition(_position);
        canvas_position.x = position->GetPosition().x - (internalCanvas.width() / 2 + 1);
        canvas_position.y = position->GetPosition().y - (internalCanvas.height() / 2 + 1);
    }
    else
    {
        _position = RadialToCartesian(phase, from_center, abs_center);
        position->SetPosition(_position);
        canvas_position.x = position->GetPosition().x - (internalCanvas.width() / 2 + 1);
        canvas_position.y = position->GetPosition().y - (internalCanvas.height() / 2 + 1);
    }
    // matte mask
    GenerateSatellite();
}

void C_Satellite::GenerateSatellite()
{
    internalCanvas.fillScreen(16U);
    internalCanvas.fillCircle(internalCanvas.width() / 2 + 1, internalCanvas.height() / 2 + 1, radius + 1, 0);
    internalCanvas.fillCircle(internalCanvas.width() / 2 + 1, internalCanvas.height() / 2 + 1, radius, color);

    if (is_planet)
    {
        internalCanvas.fillCircle(internalCanvas.width() / 2 + 1, internalCanvas.height() / 2 + 1, radius - 1, 0);
        internalCanvas.fillCircle(internalCanvas.width() / 2 + 1, internalCanvas.height() / 2 + 1, radius - 3, 2);
        internalCanvas.fillCircle(internalCanvas.width() / 2 + 1, internalCanvas.height() / 2 + 1, radius - 5, 4);
        internalCanvas.fillCircle(internalCanvas.width() / 2 + 1, internalCanvas.height() / 2 + 1, radius - 8, 6);
    }
}

void C_Satellite::Update(uint8_t deltaTime)
{
    if (!is_planet)
    {
        _position = RadialToCartesian(phase, from_center, abs_center);
        position->SetPosition(_position);
        canvas_position.x = position->GetPosition().x - (internalCanvas.width() / 2 + 1);
        canvas_position.y = position->GetPosition().y - (internalCanvas.height() / 2 + 1);
    }
};
void C_Satellite::Draw(GFXcanvas8 &canvas)
{
    canvas.drawGrayscaleBitmap(canvas_position.x, canvas_position.y, internalCanvas.getBuffer(), 16U, internalCanvas.width(), internalCanvas.height());
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
