#ifndef SHAREDCONTEXT_HPP
#define SHAREDCONTEXT_HPP

#include "ObjectInstances.hpp"
#include "InterfaceManager.hpp"
#include "Data.hpp"
#include "enjin/ObjectCollection.hpp"
class SharedContext
{
public:
    InterfaceManager interface;
    Data data;
    ObjectCollection objects;
    ObjectInstances instances;
};

#endif // SHAREDCONTEXT_HPP
