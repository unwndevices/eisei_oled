#ifndef ITEMLIST_HPP
#define ITEMLIST_HPP

#include "enjin/Object.hpp"
#include "enjin/Components/C_List.hpp"
#include "enjin/Components/C_PositionAnimator.hpp"

class ItemList : public Object
{
public:
    ItemList(const std::vector<std::string> &items)
    {
        position->SetPosition(64, 64);

        list = AddComponent<C_List<std::string>>(
            items, [](const std::string &item)
            { return item; },
            64, 127);
        list->SetDrawLayer(DrawLayer::UI);
        list->SetAnchorPoint(Anchor::CENTER);

        transition = AddComponent<C_PositionAnimator>();

        in_transition.AddKeyframe({0, Vector2(130, 64), Easing::Step});
        in_transition.AddKeyframe({350, Vector2(64, 64), Easing::EaseOutQuad});
        transition->SetAnimation(in_transition);
    }

    void UpdateList(const std::vector<std::string> &newItems)
    {
        list->UpdateItems(newItems);
    }

    void MoveUp() { list->MoveUp(); }
    void MoveDown() { list->MoveDown(); }
    std::string GetCurrentSelection() const { return list->GetCurrentSelection(); }

    void SetPosition(Vector2 pos) { list->SetPosition(pos); }

    void EnterTransition()
    {
        transition->StartAnimation();
    }

    void SetVisibility(bool visibility)
    {
        list->SetVisibility(visibility);
    }

private:
    std::shared_ptr<C_List<std::string>> list;
    std::shared_ptr<C_PositionAnimator> transition;
    PositionAnimation in_transition;
};

#endif// ITEMLIST_HPP
