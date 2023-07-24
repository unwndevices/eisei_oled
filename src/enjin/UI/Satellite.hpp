#ifndef SATELLITE_HPP
#define SATELLITE_HPP

#include <memory>

#include "enjin/Object.hpp"
#include "enjin/Components/C_Satellite.hpp"
#include "enjin/Components/C_ParameterAnimator.hpp"

class Satellite : public Object
{

public:
    Satellite(uint8_t from_center, uint8_t radius = 5, uint8_t color = 15)
    {
        position->SetPosition(Vector2(64, 64));
        satellite = AddComponent<C_Satellite>(from_center, radius, color);
        satellite->SetDrawLayer(DrawLayer::Entities);
        satellite->SetBlendMode(BlendMode::Normal);

        radius_transition = AddComponent<C_ParameterAnimator<uint8_t>>();
        pos_transition = AddComponent<C_ParameterAnimator<uint8_t>>();

        radius_transition->SetParameterGetter(std::bind(&C_Satellite::GetRadius, satellite));
        pos_transition->SetParameterGetter(std::bind(&C_Satellite::GetDistance, satellite));
    };
    void drawBackground(GFXcanvas8 &canvas)
    {
        satellite->DrawBackground(canvas);
    };

    void SetRadius(uint8_t radius)
    {
        satellite->SetRadius(radius);
    }

    void SetPhase(float phase) { satellite->SetPhase(phase); };

    void SetDistance(uint8_t distance)
    {
        satellite->SetDistance(distance);
    }

    void EnterTransition()
    {
        pos_transition->StartAnimation();
        radius_transition->StartAnimation();
    }

    std::shared_ptr<C_ParameterAnimator<uint8_t>> pos_transition;
    std::shared_ptr<C_ParameterAnimator<uint8_t>> radius_transition;

private:
    std::shared_ptr<C_Satellite> satellite;
};

#endif // !SATELLITE_HPP