#ifndef C_SLIDER_HPP
#define C_SLIDER_HPP
#include "enjin/Components/Component.hpp"
#include "enjin/Components/C_Drawable.hpp"
#include "enjin/Components/C_Position.hpp"
#include <Adafruit_GFX.h>
#include "enjin/Sprite.hpp"
#include "assets/slider_sprite.h"
#include "enjin/Object.hpp"
#include <memory>

enum Side
{
    LEFT = 0,
    RIGHT
};

class C_Slider : public Component, public C_Drawable
{
public:
    C_Slider(Object *owner, Side side);
    void Awake() override;
    void Update(uint8_t deltaTime) override;
    void Draw(GFXcanvas8 &canvas) override;
    bool ContinueToDraw() const override;
    void SetPosition(Vector2 pos) { position->SetPosition(pos); };
    void SetValue(float val);

private:
    Sprite sprite;
    Side side;
    float value, final_angle;

    uint8_t from_center, thickness;
    Vector2 _position, idle_position;

    std::shared_ptr<C_Position> position;

    bool is_active;
    uint16_t inactivity_count, inactivity_timer; // expressed in ms
};

//////////////////////////////////////////////////////////////////////////////////////////

class Slider : public Object
{
public:
    Slider(uint8_t from_center, uint8_t radius = 2)
    {
        position = AddComponent<C_Position>();
        left_slider = AddComponent<C_Slider>(Side::LEFT);
        left_slider->SetDrawLayer(DrawLayer::UI);
    };

    void ValueChanged(float value) { left_slider->SetValue(value); }

private:
    std::shared_ptr<C_Slider> left_slider;
};

#endif // !C_SLIDER_HPP
