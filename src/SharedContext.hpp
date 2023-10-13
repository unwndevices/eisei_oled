#ifndef SHAREDCONTEXT_HPP
#define SHAREDCONTEXT_HPP

#include "ObjectInstances.hpp"
#include "InterfaceManager.hpp"
#include "Data.hpp"
#include "enjin/ObjectCollection.hpp"
#include "enjin/utils/Timer.hpp"

enum Scenes
{
    Splashscreen = 0,
    Base,
    Gravity,
    Orbit,
    Scope,
    Ratio,
    Attenuverter,
    Mode
};

class SharedContext
{
public:
    InterfaceManager interface;
    Data data;
    ObjectCollection objects;
    ObjectInstances instances;
    Timer timer;
    Timer touch_timer;
};

#endif // SHAREDCONTEXT_HPP
