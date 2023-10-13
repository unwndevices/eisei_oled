#ifndef LABEL_HPP
#define LABEL_HPP

#include "enjin/Object.hpp"
#include "enjin/Components/C_Label.hpp"
#include "enjin/Components/C_PositionAnimator.hpp"

class Label : public Object
{

public:
    Label(uint8_t width, uint8_t height, int precision = 0, uint8_t labelColor = 14U, uint8_t bgColor = 0, uint8_t pointer = 0)
    {
        this->precision = precision;
        position->SetPosition(Vector2(64, 64));
        label = AddComponent<C_Label>(width, height, labelColor, bgColor, pointer);
        label->SetDrawLayer(DrawLayer::UI);
        label->SetBlendMode(BlendMode::Normal);
        label->SetAnchorPoint(Anchor::CENTER);

        transition = AddComponent<C_PositionAnimator>();
    };

    void SetValue(float value, const String &unitText = "", const String &prefix = "")
    {
        String valueString;
        if (value < 1000.0f)
            valueString = String(value, precision);
        else
            valueString = String(value, 1);

        if (unitText != "")
        {
            valueString.concat(unitText);
        }

        if (prefix != "")
        {
            valueString = prefix + valueString;
        }

        label->SetString(valueString);
    }

    void SetVisibility(bool visibility)
    {
        label->SetVisibility(visibility);
    }

    void EnterTransition(bool reset = false)
    {
        label->SetVisibility(true);
        transition->StartAnimation(reset);
    }

    std::shared_ptr<C_PositionAnimator> transition;
    PositionAnimation in_transition;

private:
    std::shared_ptr<C_Label> label;
    int precision = 0;
};
#endif // LABEL_HPP
