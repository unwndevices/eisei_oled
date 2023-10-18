#ifndef PROBE_FIELDS_HPP
#define PROBE_FIELDS_HPP

#include <memory>

#include "enjin/UI/Scope.hpp"
#include "enjin/Components/C_Probe.hpp"

class ProbeFields : public Scope
{
public:
    ProbeFields(uint8_t from_center, uint8_t radius = 10, uint8_t amount = 4, uint8_t color = 7) : from_center(from_center), radius(radius), color(color), amount(amount)
    {
        position = AddComponent<C_Position>();
        SetProbeAmount(amount);
    };

    void SetShape(float shape) override
    {
        SetPosition(shape);
    };

    void SetAmount(float amount) override
    {
        int8_t radius = (uint8_t)(amount * 45.0f);
        for (auto &probe : probes)
        {
            probe->SetRadius(radius);
        }
    };

    void SetMode(uint8_t mode) override{};

    void Update() override{};

    void SetPosition(float pos)
    {
        int i = 0;
        for (auto &probe : probes)
        {
            probe->SetPhase(((float)i / (float)amount) * pos + 0.125f);
            i++;
        }
    };

    void SetProbeAmount(uint8_t amount)
    {
        this->amount = amount;
        probes.clear();
        for (uint8_t i = 0; i < amount; i++)
        {
            probes.push_back(AddComponent<C_Probe>(from_center, radius, color));
            probes[i]->SetDrawLayer(DrawLayer::Background);
            probes[i]->SetPhase((float)i / amount);
        }
    };

private:
    std::vector<std::shared_ptr<C_Probe>> probes;

    uint8_t from_center, radius, color, amount;
};

#endif // PROBE_HPP
