#ifndef TOOLTIP_HPP
#define TOOLTIP_HPP
#include "enjin/Components/Component.hpp"
#include "enjin/Components/C_Drawable.hpp"
#include "enjin/Components/C_Position.hpp"
#include <Adafruit_GFX.h>
#include "enjin/Object.hpp"
#include <memory>

class C_Tooltip : public Component, public C_Drawable
{
public:
    C_Tooltip(Object *owner, int8_t precision);
    void Awake() override;
    void Update(uint8_t deltaTime) override;
    void Draw(GFXcanvas8 &canvas) override;
    bool ContinueToDraw() const override;
    void SetPosition(Vector2 pos) { position->SetPosition(pos); };
    void SetOrigin(Vector2 pos) { origin = pos; };
    void SetActive(bool val) { is_active = val; };
    void SetValue(float val);

private:
    float value;
    int8_t precision;
    uint8_t width;

    Vector2 origin; // position of the tooltip target

    std::shared_ptr<C_Position> position;

    GFXcanvas8 canvas;

    bool is_active;
};

//////////////////////////////////////////////////////////////////////////////////////////

class Tooltip : public Object
{
public:
    Tooltip(Vector2 origin, int8_t precision)
    {
        position = AddComponent<C_Position>();
        tooltip = AddComponent<C_Tooltip>(precision);
        tooltip->SetOrigin(origin);
        tooltip->SetDrawLayer(DrawLayer::UI);
    };

    void SetValue(float value) { tooltip->SetValue(value); }
    void SetActive(bool value) { tooltip->SetActive(value); }

private:
    std::shared_ptr<C_Tooltip> tooltip;
};

#endif // !TOOLTIP_HPP
