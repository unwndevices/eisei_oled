#ifndef TRANSMISSIONBEAM_HPP
#define TRANSMISSIONBEAM_HPP

#include <memory>

#include "enjin/UI/Satellite.hpp"
#include "enjin/UI/Scope.hpp"
#include "enjin/Components/C_TransmissionBeam.hpp"

class TransmissionBeam : public Scope
{
public:
    TransmissionBeam()
    {
        position = AddComponent<C_Position>();
        beam = AddComponent<C_TransmissionBeam>(127, 127);
        beam->SetDrawLayer(DrawLayer::Background);
    }

    void Update()
    {
    }

    void SetShape(float beamWidth) override
    {
        beam->SetWidth(beamWidth);
    }
    void SetAmount(float amount) override
    {
        beam->SetPhase(amount);
    }
    void SetMode(uint8_t mode) override
    {
        beam->SetMode(mode);
    }

private:
    std::shared_ptr<C_TransmissionBeam> beam;
};

#endif// TRANSMISSIONBEAM_HPP
