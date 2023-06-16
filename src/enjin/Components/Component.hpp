#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include "../utils/Types.hpp"

class Object;

class Component
{
public:
    Component(Object *owner) : owner(owner) {}

    virtual void Awake(){};
    virtual void Start(){};

    virtual void Update(uint8_t deltaTime){};
    virtual void LateUpdate(uint8_t deltaTime){};

protected:
    Object *owner;
};

#endif /* COMPONENT_HPP */