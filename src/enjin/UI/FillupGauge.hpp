#ifndef FILLUPGAUGE_HPP
#define FILLUPGAUGE_HPP

#include "enjin/Object.hpp"
#include "enjin/Components/C_Position.hpp"
#include "enjin/Components/C_Sprite.hpp"
#include "enjin/Components/C_FillUpGauge.hpp"
#include "enjin/Components/C_Canvas.hpp"
#include "enjin/Components/C_Label.hpp"

#include "enjin/Components/C_PositionAnimator.hpp"

#include "assets/resources.h"

class FillUpGauge : public Object
{
public:
    FillUpGauge()
    {
        position->SetPosition(64, 64);

        gauge = AddComponent<C_FillUpGauge>(111, 111, 13, GaugeMode::Bidirectional);
        gauge->SetDrawLayer(DrawLayer::Overlay);
        gauge->SetBlendMode(BlendMode::Normal);
        gauge->SetAnchorPoint(Anchor::CENTER);

        value_bg = AddComponent<C_Canvas>(55, 35);
        value_bg->SetDrawLayer(DrawLayer::Overlay);
        value_bg->SetBlendMode(BlendMode::Normal);
        value_bg->SetAnchorPoint(Anchor::CENTER);

        value_bg->_canvas.fillScreen(16U);
        value_bg->_canvas.fillRoundRect(0, 0, 55, 35, 9, 1);
        value_bg->_canvas.drawRoundRect(0, 0, 55, 35, 9, 8);
        value_bg->_canvas.drawLine(3, 17, 51, 17, 7);

        label = AddComponent<C_Label>(55, 15, 12);
        label->SetDrawLayer(DrawLayer::Overlay);
        label->SetBlendMode(BlendMode::Normal);
        label->SetAnchorPoint(Anchor::CENTER);
        label->AddOffset(Vector2(0, -9));

        value_label = AddComponent<C_Label>(55, 15, 12);
        value_label->SetDrawLayer(DrawLayer::Overlay);
        value_label->SetBlendMode(BlendMode::Normal);
        value_label->SetAnchorPoint(Anchor::CENTER);
        value_label->AddOffset(Vector2(0, 9));
        value_label->SetString("0.0");

        plus = AddComponent<C_Sprite>(11, 11);
        plus->SetDrawLayer(DrawLayer::Overlay);
        plus->SetBlendMode(BlendMode::Normal);
        plus->SetAnchorPoint(Anchor::CENTER);
        plus->AddOffset(Vector2(0, -45));
        plus->Load((const uint8_t *)plus_11, 11, 11);

        minus = AddComponent<C_Sprite>(11, 11);
        minus->SetDrawLayer(DrawLayer::Overlay);
        minus->SetBlendMode(BlendMode::Normal);
        minus->SetAnchorPoint(Anchor::CENTER);
        minus->AddOffset(Vector2(0, 45));
        minus->Load((const uint8_t *)minus_11, 11, 11);

        pos_transition = AddComponent<C_PositionAnimator>();
        pos_transition->AddKeyframe({0, Vector2(200, 64), Easing::Step});
        pos_transition->AddKeyframe({350, Vector2(64, 64), Easing::EaseOutQuad});
    };

    void SetValue(float value)
    {
        gauge->SetValue(value);
        String value_string = String(value, 2);
        value_label->SetString(value_string);
    }

    void SetString(String string)
    {
        label->SetString(string);
    }

    void SetVisibility(bool visibility)
    {
        gauge->SetVisibility(visibility);
        value_bg->SetVisibility(visibility);
        label->SetVisibility(visibility);
        value_label->SetVisibility(visibility);
        plus->SetVisibility(visibility);
        minus->SetVisibility(visibility);
    }

    void EnterTransition()
    {
        SetVisibility(true);
        pos_transition->StartAnimation();
    }

    std::shared_ptr<C_PositionAnimator> pos_transition;

private:
    std::shared_ptr<C_FillUpGauge> gauge;
    std::shared_ptr<C_Canvas> value_bg;
    std::shared_ptr<C_Label> label;
    std::shared_ptr<C_Label> value_label;

    std::shared_ptr<C_Sprite> plus;
    std::shared_ptr<C_Sprite> minus;
};

#endif// FILLUPGAUGE_HPP
