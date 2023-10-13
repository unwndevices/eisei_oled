#ifndef OBJECTINSTANCES_HPP
#define OBJECTINSTANCES_HPP

#include <memory>
#include "enjin/UI/Satellite.hpp"
#include "enjin/UI/TransmissionBeam.hpp"
#include "enjin/UI/OverlayBg.hpp"
#include "enjin/UI/FillUpGauge.hpp"

class ObjectInstances
{
public:
    std::shared_ptr<Satellite> main_planet;
    std::vector<std::shared_ptr<Satellite>> satellites;
    std::shared_ptr<TransmissionBeam> transmission_beam;
    std::shared_ptr<OverlayBg> bg;
    std::shared_ptr<FillUpGauge> gauge;
};

#endif// OBJECTINSTANCES_HPP
