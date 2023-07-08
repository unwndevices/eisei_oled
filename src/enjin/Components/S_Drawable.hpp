#ifndef S_Drawable_hpp
#define S_Drawable_hpp

#include <map>
#include <vector>

#include "C_Drawable.hpp"
#include "enjin/Object.hpp"

class S_Drawable
{
public:
    void Add(std::vector<std::shared_ptr<Object>> &object);
    void Add(std::shared_ptr<Object> object);

    void ProcessRemovals();

    void Draw(GFXcanvas8 &canvas);
    void Sort();

private:
    std::map<DrawLayer, std::vector<std::shared_ptr<C_Drawable>>> drawables;
};

#endif /* S_Drawable_hpp */