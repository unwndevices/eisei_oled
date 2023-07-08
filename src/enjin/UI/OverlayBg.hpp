#ifndef OVERLAYBG_HPP
#define OVERLAYBG_HPP
#include "enjin/Object.hpp"
#include "enjin/Components/C_Sprite.hpp"
#include "enjin/Components/C_Canvas.hpp"
#include "enjin/Components/C_Label.hpp"

#include "assets/resources.h"

class OverlayBg : public Object
{
public:
    OverlayBg(uint8_t gradient = 3)
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
    };

    void SetVisibility(bool visibility)
    {
        overlay_fill->SetVisibility(visibility);
        overlay_graphic->SetVisibility(visibility);
    }

private:
    std::shared_ptr<C_Canvas> overlay_fill;
    std::shared_ptr<C_Sprite> overlay_graphic;
};

#endif// OVERLAYBG_HPP
