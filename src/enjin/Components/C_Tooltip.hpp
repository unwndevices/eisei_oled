#ifndef C_TOOLTIP_HPP
#define C_TOOLTIP_HPP
#include <memory>
#include "Component.hpp"
#include "C_Drawable.hpp"
#include "C_Position.hpp"

class C_Tooltip : public C_Drawable
{
public:
    C_Tooltip(Object *owner, int8_t precision, uint8_t width, uint8_t height);
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

    GFXcanvas8 canvas;

    bool is_active;
};

#endif // C_TOOLTIP_HPP
