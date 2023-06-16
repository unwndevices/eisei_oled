#ifndef ObjectCollection_hpp
#define ObjectCollection_hpp

#include <memory>
#include <vector>

#include "enjin/Object.hpp"
#include "Components/S_Drawable.hpp"

class ObjectCollection
{
public:
    void Add(std::shared_ptr<Object> object);

    void Update(uint8_t deltaTime);
    void LateUpdate(uint8_t deltaTime);
    void Draw(GFXcanvas8 &canvas);

    void ProcessNewObjects();
    void ProcessRemovals();

private:
    std::vector<std::shared_ptr<Object>> objects;
    std::vector<std::shared_ptr<Object>> newObjects;
    S_Drawable drawables;
};

#endif /* ObjectCollection_hpp */