#include "S_Drawable.hpp"

void S_Drawable::Add(std::vector<std::shared_ptr<Object>> &objects)
{
    for (auto o : objects)
    {
        Add(o);
    }

    Sort();
}

void S_Drawable::Add(std::shared_ptr<Object> object)
{
    std::shared_ptr<C_Drawable> draw = object->GetDrawable();

    if (draw)
    {
        DrawLayer layer = draw->GetDrawLayer();

        auto itr = drawables.find(layer);

        if (itr != drawables.end())
        {
            drawables[layer].push_back(object);
        }
        else
        {
            std::vector<std::shared_ptr<Object>> objs;
            objs.push_back(object);

            drawables.insert(std::make_pair(layer, objs));
        }
    }
}

void S_Drawable::Sort()
{
    for (auto &layer : drawables)
    {
        std::sort(layer.second.begin(), layer.second.end(),
                  [](std::shared_ptr<Object> a,
                     std::shared_ptr<Object> b) -> bool
                  {
                      return a->GetDrawable()->GetSortOrder() <
                             b->GetDrawable()->GetSortOrder();
                  });
    }
}

void S_Drawable::Draw(GFXcanvas8 &canvas)
{
    for (auto &layer : drawables)
    {
        for (auto &drawable : layer.second)
        {
            drawable->Draw(canvas);
        }
    }
}
void S_Drawable::ProcessRemovals()
{
    for (auto &layer : drawables)
    {
        auto objIterator = layer.second.begin();
        while (objIterator != layer.second.end())
        {
            auto obj = *objIterator;

            if (obj->IsQueuedForRemoval())
            {
                objIterator = layer.second.erase(objIterator);
            }
            else
            {
                ++objIterator;
            }
        }
    }
}