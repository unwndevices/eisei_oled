#ifndef CONSTANTLIST_HPP
#define CONSTANTLIST_HPP

#include "enjin/Object.hpp"
#include "enjin/Components/C_List.hpp"
#include "Constants.hpp"

class ConstantList : public Object
{
public:
    ConstantList(const std::vector<ConstantPair> &constants)
    {
        position = AddComponent<C_Position>();
        list = AddComponent<C_List<ConstantPair>>(constants, [](const ConstantPair &constant)
                                                  { return constant.first; });
        list->SetDrawLayer(DrawLayer::UI);
    }

    void MoveUp() { list->MoveUp(); }
    void MoveDown() { list->MoveDown(); }
    ConstantPair GetCurrentSelection() const { return list->GetCurrentSelection(); }
    void SetSelectedIndexByValue(float value) { list->SetSelectedIndexByValue(value); }
    float GetCurrentSelectionValue() const
    {
        return GetCurrentSelection().second;
    }

    void SetPosition(Vector2 pos) { list->SetPosition(pos); }

private:
    std::shared_ptr<C_List<ConstantPair>> list;
};

#endif // CONSTANTLIST_HPP
