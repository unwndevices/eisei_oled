#ifndef FILLUPGAUGE_HPP
#define FILLUPGAUGE_HPP

#include "enjin/Object.hpp"
#include "enjin/Components/C_Position.hpp"
#include "enjin/Components/C_Sprite.hpp"
#include "enjin/Components/C_FillUpGauge.hpp"
#include "enjin/Components/C_Canvas.hpp"
#include "enjin/Components/C_Label.hpp"

#include "assets/resources.h"

class FillUpGauge : public Object
{
public:
    FillUpGauge()
    {
        // position = AddComponent<C_Position>(64, 64);
        position->SetPosition(64, 64);
        overlay_fill = AddComponent<C_Canvas>(128, 128);
        overlay_fill->SetDrawLayer(DrawLayer::Overlay);
        overlay_fill->SetBlendMode(BlendMode::Sub);
        overlay_fill->_canvas.fillScreen(6);

        overlay_graphic = AddComponent<C_Sprite>(128, 128);
        overlay_graphic->SetDrawLayer(DrawLayer::Overlay);
        overlay_graphic->SetBlendMode(BlendMode::Normal);
        overlay_graphic->SetAnchorPoint(Anchor::CENTER);
        overlay_graphic->Load((const uint8_t *)overlay_gradient, 128, 128);
        overlay_graphic->LoadFrame(4);

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

        label = AddComponent<C_Label>(55, 15, false, 12);
        label->SetDrawLayer(DrawLayer::Overlay);
        label->SetBlendMode(BlendMode::Normal);
        label->SetAnchorPoint(Anchor::CENTER);
        label->AddOffset(Vector2(0, -9));
        label->SetString("grav");

        value_label = AddComponent<C_Label>(55, 15, false, 12);
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
        overlay_fill->SetVisibility(visibility);
        overlay_graphic->SetVisibility(visibility);
        gauge->SetVisibility(visibility);
        value_bg->SetVisibility(visibility);
        label->SetVisibility(visibility);
        value_label->SetVisibility(visibility);
        plus->SetVisibility(visibility);
        minus->SetVisibility(visibility);
    }

private:
    std::shared_ptr<C_Canvas> overlay_fill;
    std::shared_ptr<C_Sprite> overlay_graphic;
    std::shared_ptr<C_FillUpGauge> gauge;
    std::shared_ptr<C_Canvas> value_bg;
    std::shared_ptr<C_Label> label;
    std::shared_ptr<C_Label> value_label;

    std::shared_ptr<C_Sprite> plus;
    std::shared_ptr<C_Sprite> minus;
};

#endif// FILLUPGAUGE_HPP
