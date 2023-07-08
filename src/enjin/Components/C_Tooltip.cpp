#include <iostream>

#include "C_Tooltip.hpp"
#include "enjin/Object.hpp"
#include <Fonts/VGATypewriter.h>

C_Tooltip::C_Tooltip(Object *owner, int8_t precision, uint8_t width, uint8_t height) : C_Drawable(width, height), Component(owner),
                                                                                       precision(precision),
                                                                                       value(220.0f),
                                                                                       width(31),
                                                                                       origin(0, 0),
                                                                                       canvas(width, height),
                                                                                       is_active(false)
{
    position = owner->GetComponent<C_Position>();

    if (!position)
    {
        std::cerr << "C_Tooltip requires C_Position component.\n";
    }
}

void C_Tooltip::Draw(GFXcanvas8 &canvas)
{
    if (is_active)
    {
        String value_string = String(value, precision);
        // draw value
        if (!precision)
        {
            value_string = String((int)value);
        }
        width = this->canvas.getTextWidth(value_string);
        this->canvas.fillScreen(16U);
        this->canvas.fillRoundRect(3, 0, width + 2 + 8, 15 + 2, 4, 0);
        this->canvas.fillRoundRect(4, 1, width + 8, 15, 3, 1);
        this->canvas.drawRoundRect(4, 1, width + 8, 15, 3, 15);
        this->canvas.setCursor(8, 12);
        this->canvas.println(value_string);
        // draw canvas
        canvas.drawGrayscaleBitmap(origin.x, origin.y, this->canvas.getBuffer(), (uint8_t)16U, this->canvas.width(), this->canvas.height());
    }
}

bool C_Tooltip::ContinueToDraw() const
{
    return !owner->IsQueuedForRemoval();
}

void C_Tooltip::Awake()
{
    position->SetPosition(origin);
    // Drawing the actual tooltip sprite on the canvas
    // setup font
    canvas.setTextColor(15);
    canvas.setFont(&VGATypewriter8pt7b);
}

void C_Tooltip::SetValue(float val)
{
    is_active = true;
    value = val;
}

void C_Tooltip::Update(uint8_t deltaTime)
{
}