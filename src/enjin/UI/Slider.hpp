#ifndef SLIDER_HPP
#define SLIDER_HPP

#include "enjin/Object.hpp"
#include "enjin/Components/C_Slider.hpp"
#include "enjin/Components/C_Canvas.hpp"
#include "enjin/Components/C_Label.hpp"
#include "enjin/Components/C_PositionAnimator.hpp"

class Slider : public Object
{
public:
    Slider(String par_name)
    {
        position->SetPosition(64, 89);
        name = AddComponent<C_Label>(50, 20);
        name->SetDrawLayer(DrawLayer::Overlay);
        name->SetBlendMode(BlendMode::Normal);
        name->SetAnchorPoint(Anchor::CENTER_BOTTOM);
        name->AddOffset(Vector2(0, 13));
        value = AddComponent<C_Label>(50, 20);
        value->SetDrawLayer(DrawLayer::Overlay);
        value->SetBlendMode(BlendMode::Normal);
        value->SetAnchorPoint(Anchor::CENTER_TOP);
        value->AddOffset(Vector2(0, 8));
        slider = AddComponent<C_Slider>(83, 7);
        slider->SetDrawLayer(DrawLayer::Overlay);
        slider->SetBlendMode(BlendMode::Normal);
        slider->SetAnchorPoint(Anchor::CENTER_BOTTOM);
        slider->AddOffset(Vector2(0, -15));
        SetName(par_name);
        SetValue(0.0f);

        transition = AddComponent<C_PositionAnimator>();
        PositionKeyframe kf1 = {0, Vector2(64, 160), Easing::Step};
        PositionKeyframe kf2 = {350, Vector2(64, 89), Easing::EaseInOutCubic};
        transition->AddKeyframe(kf1);
        transition->AddKeyframe(kf2);
    };

    void SetName(String string)
    {
        name->SetString(string);
    }
    void SetValue(float val)
    {
        slider->SetValue(val);
        String value_string = String(val, 2);
        value->SetString(value_string);
    }
    void SetVisibility(bool visibility)
    {
        slider->SetVisibility(visibility);
        name->SetVisibility(visibility);
        value->SetVisibility(visibility);
    }

    void EnterTransition()
    {
        transition->StartAnimation();
    }

private:
    std::shared_ptr<C_Slider> slider;
    std::shared_ptr<C_Label> name;
    std::shared_ptr<C_Label> value;
    std::shared_ptr<C_PositionAnimator> transition;
};

#endif// SLIDER_HPP
