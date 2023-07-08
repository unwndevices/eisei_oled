#ifndef CONSTANTLIST_HPP
#define CONSTANTLIST_HPP

#include "enjin/Object.hpp"
#include "enjin/Components/C_Canvas.hpp"
#include "enjin/Components/C_List.hpp"
#include "enjin/Components/C_PositionAnimator.hpp"
#include "Constants.hpp"

class ConstantList : public Object
{
public:
    ConstantList(const std::vector<ConstantPair> &constants)
    {
        position->SetPosition(64, 64);
        
        bg = AddComponent<C_Canvas>(64, 127);
        bg->SetDrawLayer(DrawLayer::UI);
        bg->SetAnchorPoint(Anchor::CENTER_LEFT);
        bg->_canvas.fillScreen(0);
        bg->_canvas.drawFastVLine(0, 0, 127, 10);

        list = AddComponent<C_List<ConstantPair>>(
            constants, [](const ConstantPair &constant)
            { return constant.first; },
            64, 127);
        list->SetDrawLayer(DrawLayer::UI);
        list->SetAnchorPoint(Anchor::CENTER_LEFT);
        list->AddOffset(Vector2(4, 0));


        transition = AddComponent<C_PositionAnimator>();
        PositionKeyframe kf1 = {0, Vector2(130, 64), Easing::Step};
        PositionKeyframe kf2 = {350, Vector2(64, 64), Easing::EaseOutQuad};
        transition->AddKeyframe(kf1);
        transition->AddKeyframe(kf2);
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

    void EnterTransition()
    {
        transition->StartAnimation();
    }

private:
    std::shared_ptr<C_Canvas> bg;
    std::shared_ptr<C_List<ConstantPair>> list;
    std::shared_ptr<C_PositionAnimator> transition;
};

#endif// CONSTANTLIST_HPP
