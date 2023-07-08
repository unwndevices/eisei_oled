#ifndef C_LABEL_HPP
#define C_LABEL_HPP
#include "Arduino.h"

#include <memory>
#include "Component.hpp"
#include "C_Drawable.hpp"
#include "C_Position.hpp"

class C_Label : public C_Drawable
{
public:
    C_Label(Object *owner, uint8_t width, uint8_t height, bool background = false, uint8_t labelColor = 14U); // Added parameters here
    void Awake() override;
    void Update(uint8_t deltaTime) override;
    void Draw(GFXcanvas8 &canvas) override;
    bool ContinueToDraw() const override;
    void SetString(String string);

private:
    String string;
    uint8_t text_width;
    bool background;
    uint8_t labelColor;

    GFXcanvas8 internalCanvas;
};

#endif // C_LABEL_HPP
