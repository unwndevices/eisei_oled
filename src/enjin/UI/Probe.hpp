#ifndef PROBE_HPP
#define PROBE_HPP

#include <memory>

#include "enjin/Object.hpp"
#include "enjin/Components/C_Probe.hpp"

class Probe : public Object
{
public:
    Probe(uint8_t from_center, uint8_t radius = 10, uint8_t amount = 3, uint8_t color = 7) : from_center(from_center), radius(radius), color(color), amount(amount)
    {
        position = AddComponent<C_Position>();
        SetProbeAmount(amount);
    };
    void drawBackground(GFXcanvas8 &canvas){
        // satellite->DrawBackground(canvas);
    };

    void SetDistance(uint8_t distance)
    {
        // satellite->SetDistance(distance);
    }
    void SetRadius(uint8_t radius)
    {
        // satellite->SetRadius(radius);
    }

    void SetPhase(float phase)
    {
        int i = 0 ;
        for (auto &probe : probes)
        {
            probe->SetPhase((float)i / amount + phase);
            i++;
        }
    };

    void SetProbeAmount(uint8_t amount)
    {

        this->amount = amount;
        for (uint8_t i = 0; i < amount; i++)
        {
            probes.push_back(AddComponent<C_Probe>(from_center, radius, color));
            probes[i]->SetDrawLayer(DrawLayer::Background);
            probes[i]->SetPhase((float)i / amount);
        }
    }

private:
    std::vector<std::shared_ptr<C_Probe>> probes;

    uint8_t from_center, radius, color, amount;
};

#endif // PROBE_HPP
