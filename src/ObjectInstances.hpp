#ifndef OBJECTINSTANCES_HPP
#define OBJECTINSTANCES_HPP

#include <memory>
#include "enjin/UI/Satellite.hpp"
#include "enjin/UI/TransmissionBeam.hpp"

class ObjectInstances
{
public:
    std::shared_ptr<Satellite> main_planet;
    std::vector<std::shared_ptr<Satellite>> satellites;
    std::shared_ptr<TransmissionBeam> transmission_beam;
};

#endif// OBJECTINSTANCES_HPP
