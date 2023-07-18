#ifndef GRAVITYFIELDS_HPP
#define GRAVITYFIELDS_HPP
#include <memory>

#include "enjin/UI/Satellite.hpp"
#include "enjin/Object.hpp"
#include "enjin/Components/C_GravityFields.hpp"

class GravityFields : public Object
{
public:
    GravityFields(const std::vector<std::shared_ptr<Satellite>> &satellites)
    {
        position = AddComponent<C_Position>();
        gravity_fields = AddComponent<C_GravityFields>(127, 127);
        gravity_fields->SetDrawLayer(DrawLayer::Background);
    }

    void Update()
    {
    }

private:
    std::shared_ptr<C_GravityFields> gravity_fields;
};

#endif// GRAVITYFIELDS_HPP
