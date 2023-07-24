#ifndef OVERLAYBG_HPP
#define OVERLAYBG_HPP
#include "enjin/Object.hpp"
#include "enjin/Components/C_Sprite.hpp"
#include "enjin/Components/C_Canvas.hpp"
#include "enjin/Components/C_Label.hpp"
#include "enjin/Components/C_ParameterAnimator.hpp"

#include "assets/resources.h"

class OverlayBg : public Object
{
public:
    OverlayBg(uint8_t gradient = 5)
    {
        overlay_fill = AddComponent<C_Canvas>(128, 128);
        overlay_fill->SetDrawLayer(DrawLayer::Overlay);
        overlay_fill->SetBlendMode(BlendMode::Sub);
        overlay_fill->_canvas.fillScreen(6);

        overlay_graphic = AddComponent<C_Sprite>(128, 128);
        overlay_graphic->SetDrawLayer(DrawLayer::Overlay);
        overlay_graphic->SetBlendMode(BlendMode::Normal);
        overlay_graphic->Load((const uint8_t *)overlay_gradient, 128, 128);
        overlay_graphic->LoadFrame(gradient);

        fill_transition = AddComponent<C_ParameterAnimator<uint8_t>>();
        fill_transition->SetParameterSetter(std::bind(&OverlayBg::SetOpacity, this, std::placeholders::_1));



        fill_transition->AddKeyframe({0, 0U, Easing::Step});
        fill_transition->AddKeyframe({250, 5U, Easing::Linear});

        graphic_transition = AddComponent<C_ParameterAnimator<uint8_t>>();
        graphic_transition->SetParameterSetter(std::bind(&OverlayBg::SetFrame, this, std::placeholders::_1));

        graphic_transition->AddKeyframe({0, 0U, Easing::Step});
        graphic_transition->AddKeyframe({250, gradient, Easing::Linear});
    };

    void SetVisibility(bool visibility)
    {
        overlay_fill->SetVisibility(visibility);
        overlay_graphic->SetVisibility(visibility);
    }

    void SetDrawLayer(DrawLayer layer)
    {
        overlay_fill->SetDrawLayer(layer);
        overlay_graphic->SetDrawLayer(layer);
    }

    void EnterTransition()
    {
        SetVisibility(true);
        graphic_transition->StartAnimation();
        fill_transition->StartAnimation();
    }

    // using float parameter to set frame to deal with float parameter animator atm
    void SetFrame(uint8_t frame)
    {
        overlay_graphic->LoadFrame(frame);
    }

    void SetOpacity(uint8_t opacity)
    {
        overlay_fill->_canvas.fillScreen(opacity);
    }

private:
    std::shared_ptr<C_Canvas> overlay_fill;
    std::shared_ptr<C_Sprite> overlay_graphic;
    std::shared_ptr<C_ParameterAnimator<uint8_t>> fill_transition;
    std::shared_ptr<C_ParameterAnimator<uint8_t>> graphic_transition;
};

#endif// OVERLAYBG_HPP
