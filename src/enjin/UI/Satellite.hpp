#ifndef SATELLITE_HPP
#define SATELLITE_HPP

#include <memory>

#include "enjin/Object.hpp"
#include "C_Satellite.hpp"

class Satellite : public Object
{

public:
    Satellite(uint8_t from_center, uint8_t radius = 5, uint8_t color = 15)
    {
        position = AddComponent<C_Position>();
        satellite = AddComponent<C_Satellite>(from_center, radius, color);
        satellite->SetDrawLayer(DrawLayer::Entities);
    };
    void drawBackground(GFXcanvas8 &canvas)
    {
        satellite->DrawBackground(canvas);
    };

    

    void SetPhase(float phase){satellite->SetPhase(phase);};
private:
    std::shared_ptr<C_Satellite> satellite;
};

#endif // !SATELLITE_HPP