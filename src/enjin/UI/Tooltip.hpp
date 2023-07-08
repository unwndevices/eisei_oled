#ifndef TOOLTIP_HPP
#define TOOLTIP_HPP
#include "enjin/Object.hpp"
#include "enjin/Components/C_Tooltip.hpp"
#include <memory>

class Tooltip : public Object
{
public:
    Tooltip(Vector2 origin, int8_t precision)
    {
        tooltip = AddComponent<C_Tooltip>(precision, 44, 20);
        tooltip->SetOrigin(origin);
        tooltip->SetDrawLayer(DrawLayer::UI);
    };

    void SetValue(float value) { tooltip->SetValue(value); }
    void SetActive(bool value) { tooltip->SetActive(value); }

private:
    std::shared_ptr<C_Tooltip> tooltip;
};

#endif // !TOOLTIP_HPP
