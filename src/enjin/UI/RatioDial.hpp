#ifndef RATIODIAL_HPP
#define RATIODIAL_HPP
#include <memory>

#include "enjin/Object.hpp"
#include "enjin/Components/C_Tickmarks.hpp"

class RatioDial : public Object
{
public:
    RatioDial()
    {
        tickmarks = AddComponent<C_Tickmarks>(Vector2(64, 64), -82, 87, 6, 9, 61);
        tickmarks->SetDrawLayer(DrawLayer::UI);
        tickmarks->SetBlendMode(BlendMode::Normal);
    }

    void SetValue(float value)
    {
        tickmarks->SetValue(value);
    }
    float GetValue()
    {
        return tickmarks->GetValue();
    }

private:
    std::shared_ptr<C_Tickmarks> tickmarks;
};

#endif// RATIODIAL_HPP
