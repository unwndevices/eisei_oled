#ifndef SCOPE_HPP
#define SCOPE_HPP

// base class for all scopes (like ProbeFields and TransmissionBeam)

#include <memory>

#include "enjin/Object.hpp"
#include "enjin/Components/C_Position.hpp"

class Scope : public Object
{
public:
    Scope(){};

    virtual void SetShape(float shape) = 0;
    virtual void SetAmount(float amount) = 0;
    virtual void Update() = 0;
    virtual void SetMode(uint8_t mode) = 0;
};

#endif // SCOPE_HPP
