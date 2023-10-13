#include <iostream>

#include "C_Label.hpp"
#include "enjin/Object.hpp"
#include <Fonts/VGATypewriter.h>

C_Label::C_Label(Object *owner, uint8_t width, uint8_t height, uint8_t labelColor, uint8_t bgColor, uint8_t pointer) : C_Drawable(width, height + pointer), Component(owner),
                                                                                                                       string(String("hey")),
                                                                                                                       text_width(0),
                                                                                                                       pointer(pointer),
                                                                                                                       labelColor(labelColor),
                                                                                                                       bgColor(bgColor)
{
    position = owner->GetComponent<C_Position>();

    if (!position)
    {
        std::cerr << "C_Tooltip requires C_Position component.\n";
    }
}

void C_Label::Draw(GFXcanvas8 &canvas)
{
    canvas.setTextColor(labelColor);

    text_width = canvas.getTextWidth(string);

    if (bgColor) // Check if rounded rectangle should be drawn
    {
        canvas.fillRoundRect(GetOffsetPosition().x, GetOffsetPosition().y, width, height - pointer, 8, bgColor);
        canvas.drawRoundRect(GetOffsetPosition().x, GetOffsetPosition().y, width, height - pointer, 8, labelColor); // Use labelColor here
    }
    if (pointer) // Check if pointer should be drawn
    {
        canvas.fillTriangle(GetOffsetPosition().x + width / 2 - 3, GetOffsetPosition().y + height - pointer - 1, GetOffsetPosition().x + width / 2 + 3, GetOffsetPosition().y + height - pointer - 1, GetOffsetPosition().x + width / 2, GetOffsetPosition().y + height, bgColor);
        canvas.drawLine(GetOffsetPosition().x + (width) / 2 - 3, GetOffsetPosition().y + height - pointer - 1, GetOffsetPosition().x + width / 2, GetOffsetPosition().y + height, labelColor);
        canvas.drawLine(GetOffsetPosition().x + (width) / 2 + 3, GetOffsetPosition().y + height - pointer - 1, GetOffsetPosition().x + width / 2, GetOffsetPosition().y + height, labelColor);
    }
    canvas.setCursor(GetOffsetPosition().x + (width - text_width) / 2, GetOffsetPosition().y + (height - pointer + 7) / 2); // Center the label
    canvas.println(string);
}

bool C_Label::ContinueToDraw() const
{
    return !owner->IsQueuedForRemoval();
}

void C_Label::Awake()
{
}

void C_Label::SetString(String string)
{
    this->string = string;
}

void C_Label::Update(uint8_t deltaTime)
{
}