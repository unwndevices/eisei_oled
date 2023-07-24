#ifndef LABEL_HPP
#define LABEL_HPP

#include "enjin/Object.hpp"
#include "enjin/Components/C_Label.hpp"
#include "enjin/Components/C_PositionAnimator.hpp"

class Label : public Object
{

public:
    Label(uint8_t width, uint8_t height, uint8_t labelColor = 14U, uint8_t bgColor = 0, uint8_t pointer = 0)
    {
        position->SetPosition(Vector2(64, 64));
        label = AddComponent<C_Label>(width, height, labelColor, bgColor, pointer);
        label->SetDrawLayer(DrawLayer::UI);
        label->SetBlendMode(BlendMode::Normal);
        label->SetAnchorPoint(Anchor::CENTER);

        transition = AddComponent<C_PositionAnimator>();
    };

    void SetValue(float value, const String &unitText = "")
    {
        String valueString;
        if (value < 1000.0f)
            valueString = String(value, 1);
        else
            valueString = String(value, 0);

        if (unitText != "")
        {
            label->SetString(valueString + unitText);
        }

        else
        {
            label->SetString(valueString);
        }
    }

    void SetVisibility(bool visibility)
    {
        label->SetVisibility(visibility);
    }

    void EnterTransition()
    {
        label->SetVisibility(true);
        transition->StartAnimation();
    }

    std::shared_ptr<C_PositionAnimator> transition;

private:
    std::shared_ptr<C_Label> label;
};
#endif // LABEL_HPP
