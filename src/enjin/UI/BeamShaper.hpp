#ifndef BEAMSHAPER_HPP
#define BEAMSHAPER_HPP

#include "enjin/Object.hpp"
#include "enjin/Components/C_Canvas.hpp"
#include "enjin/Components/C_SymmShape.hpp"
#include "enjin/Components/C_Tickmarks.hpp"
#include "enjin/Components/C_PositionAnimator.hpp"
#include "Constants.hpp"

class BeamShaper : public Object
{
public:
    BeamShaper()
    {
        position->SetPosition(64, 64);

        bg = AddComponent<C_Canvas>(127, 64);
        bg->SetDrawLayer(DrawLayer::UI);
        bg->SetAnchorPoint(Anchor::CENTER_BOTTOM);
        bg->_canvas.fillScreen(0);
        bg->_canvas.drawFastHLine(0, 63, 127, 10);

        shape = AddComponent<C_SymmShape>(127, 64, 83);
        shape->SetDrawLayer(DrawLayer::UI);
        shape->SetAnchorPoint(Anchor::CENTER_BOTTOM);
        shape->AddOffset(Vector2(0, 10));

        tickmarks = AddComponent<C_Tickmarks>(Vector2(63, 63), 195, 350, 6, 5, 60);
        tickmarks->SetDrawLayer(DrawLayer::UI);
        tickmarks->SetBlendMode(BlendMode::Normal);
        tickmarks->SetAnchorPoint(Anchor::CENTER);
        tickmarks->AddOffset(Vector2(1,1));

        transition = AddComponent<C_PositionAnimator>();
        PositionKeyframe kf1 = {0, Vector2(64, -67), Easing::Step};
        PositionKeyframe kf2 = {350, Vector2(64, 64), Easing::EaseOutQuad};
        transition->AddKeyframe(kf1);
        transition->AddKeyframe(kf2);
    }

    // void MoveUp() { list->MoveUp(); }
    // void MoveDown() { list->MoveDown(); }

    void EnterTransition()
    {
        transition->StartAnimation();
    }

    void SetShape(float value)
    {
        shape->SetAttack(1.0f - value);
        shape->SetDecay(value);
        tickmarks->SetValue(value * 10.0f);
    }

    void SetHold(float value)
    {
        shape->SetHold(value);
    }

    void SetVisibility(bool visibility)
    {
        shape->SetVisibility(visibility);
        tickmarks->SetVisibility(visibility);
    }

private:
    std::shared_ptr<C_Canvas> bg;
    std::shared_ptr<C_SymmShape> shape;
    std::shared_ptr<C_Tickmarks> tickmarks;
    std::shared_ptr<C_PositionAnimator> transition;
};

#endif// BEAMSHAPER_HPP
