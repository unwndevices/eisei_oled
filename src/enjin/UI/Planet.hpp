#ifndef PLANET_HPP
#define PLANET_HPP

#include <Memory>
#include "enjin/Object.hpp"
#include "enjin/Components/C_Planet.hpp"

class Planet : public Object
{
public:
    Planet(uint8_t radius = 30)
    {
        position->SetPosition(64, 64);
        planet = AddComponent<C_Planet>(radius);
        planet->SetDrawLayer(DrawLayer::Entities);
        planet->SetAnchorPoint(Anchor::CENTER);
    };

    void SetRadius(uint8_t radius)
    {
        planet->SetRadius(radius);
    }

    void GenerateTerrain()
    {
        planet->GenerateTerrain();
    }

    void SetPhase(float phase) { planet->SetPhase(phase); };

    void SetVisibility(bool visibility) { planet->SetVisibility(visibility); };

private:
    std::shared_ptr<C_Planet> planet;
};

#endif // PLANET_HPP
