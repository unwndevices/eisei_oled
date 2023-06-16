#include <iostream>

#include "Slider.hpp"
#include "enjin/Object.hpp"

float map(float value, float fromLow, float fromHigh, float toLow, float toHigh);
float map(float value, float fromLow, float fromHigh, float toLow, float toHigh)
{
    return (value - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow;
}

C_Slider::C_Slider(Object *owner, Side side) : Component(owner),
                                               side(side),
                                               sprite(slider_sprite, 56, 127),
                                               value(0.0f),
                                               final_angle(0.0f),
                                               from_center(60),
                                               thickness(4),
                                               is_active(false),
                                               inactivity_count(4000),
                                               inactivity_timer(0)
{
    position = owner->GetComponent<C_Position>();

    if (!position)
    {
        std::cerr << "C_Satellite requires C_Position component.\n";
    }
}

void C_Slider::Draw(GFXcanvas8 &canvas)
{
    //canvas.drawArc(65, 65, 61, 4, 97.0f, final_angle, 0xf);
    // canvas.drawCircleArc(64, 64, 59,97.0f, final_angle, 2, 0xf);
    //sprite.draw(canvas);
}

bool C_Slider::ContinueToDraw() const
{
    return !owner->IsQueuedForRemoval();
}

void C_Slider::Awake()
{
    _position = Vector2(1, 1);
    idle_position = Vector2(-30, 1);
    position->SetPosition(idle_position);
    sprite.setPosition(position->GetPosition());
    // TODO better initialization
}

void C_Slider::SetValue(float val)
{
    is_active = true;
    value += val;
}

void C_Slider::Update(uint8_t deltaTime)
{
    if (is_active)
    {
        position->SetPosition(_position);
        sprite.setPosition(position->GetPosition());
        final_angle = map(value, 0.0f, 2.0f, 97.0f, 262.0f);
    }
    else if (inactivity_timer < inactivity_count)
    {
        inactivity_timer += deltaTime;
    }
    else
    {
        position->SetPosition(idle_position);
        is_active = false;
        inactivity_timer = 0;
        sprite.setPosition(position->GetPosition());
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
