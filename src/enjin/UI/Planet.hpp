#ifndef PLANET_HPP
#define PLANET_HPP

#include <Memory>
#include "enjin/Object.hpp"
#include "enjin/Components/C_Satellite.hpp"
#include "enjin/Components/C_Planet.hpp"
#include "enjin/Components/C_Label.hpp"
#include "enjin/Components/C_ParameterAnimator.hpp"

class Planet : public Object
{
public:
    Planet(uint8_t radius = 30)
    {
        position->SetPosition(Vector2(64, 64));

        planet = AddComponent<C_Planet>(radius);
        planet->SetDrawLayer(DrawLayer::UI);
        planet->SetBlendMode(BlendMode::Normal);
        planet->SetAnchorPoint(Anchor::CENTER);

        SetFrequency(50.0f);
        SetVisibility(false);
    };

    void SetRadius(uint8_t radius)
    {
        planet->SetRadius(radius);
    };

    void GenerateTerrain()
    {
        planet->GenerateTerrain();
    };

    void SetPhase(float phase) { planet->SetPhase(phase); };

    void SetFrequency(float frequency)
    {
        planet->SetSpeed(frequency);
    };

    void SetVisibility(bool visibility)
    {
        planet->SetVisibility(visibility);
    };

    void SetActive()
    {
        planet->SetVisibility(true);
    };

    void EnterTransition()
    {
        transition->StartAnimation();
    };

    std::shared_ptr<C_ParameterAnimator<uint8_t>> transition;

private:
    std::shared_ptr<C_Planet> planet;
};

#endif// PLANET_HPP
