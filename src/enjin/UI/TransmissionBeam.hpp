#ifndef TRANSMISSIONBEAM_HPP
#define TRANSMISSIONBEAM_HPP

#include <memory>

#include "enjin/UI/Satellite.hpp"
#include "enjin/Object.hpp"
#include "enjin/Components/C_TransmissionBeam.hpp"

class TransmissionBeam : public Object
{
public:
    TransmissionBeam()
    {
        position = AddComponent<C_Position>();
        beam = AddComponent<C_TransmissionBeam>();
        beam->SetDrawLayer(DrawLayer::Background);
    }

    void Update()
    {
    }

    void SetWidth(float beamWidth)
    {
        beam->SetWidth(beamWidth);
    }
    void SetPhase(float phase)
    {
        beam->SetPhase(phase);
    }
    void SetMode(int mode)
    {
        beam->SetMode(mode);
    }

private:
    std::shared_ptr<C_TransmissionBeam> beam;
};

#endif // TRANSMISSIONBEAM_HPP
