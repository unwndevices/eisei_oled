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
    C_Label(Object *owner, uint8_t width, uint8_t height, uint8_t labelColor = 14U, uint8_t bgColor = 0, uint8_t pointer = 0); // Added parameters here
    void Awake() override;
    void Update(uint8_t deltaTime) override;
    void Draw(GFXcanvas8 &canvas) override;
    bool ContinueToDraw() const override;
    void SetString(String string);
    void SetMargins(int left_margin, int right_margin = 0){this->left_margin = left_margin; this->right_margin = right_margin;};


private:
    String string;
    uint8_t text_width, pointer;
    uint8_t labelColor, bgColor;

    int left_margin, right_margin = 0;

};

#endif// C_LABEL_HPP
