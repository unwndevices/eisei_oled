#include <iostream>

#include "C_Label.hpp"
#include "enjin/Object.hpp"
#include <Fonts/VGATypewriter.h>

C_Label::C_Label(Object *owner, uint8_t width, uint8_t height, bool background, uint8_t labelColor) : C_Drawable(width, height), Component(owner),
                                                                                                      string(String("hey")),
                                                                                                      text_width(0),
                                                                                                      background(background),
                                                                                                      labelColor(labelColor),
                                                                                                      internalCanvas(width, height)
{
    position = owner->GetComponent<C_Position>();

    if (!position)
    {
        std::cerr << "C_Tooltip requires C_Position component.\n";
    }
}

void C_Label::Draw(GFXcanvas8 &canvas)
{
    internalCanvas.fillScreen((uint8_t)16U);
    text_width = internalCanvas.getTextWidth(string);
    if (background) // Check if rounded rectangle should be drawn
    {
        internalCanvas.fillRoundRect(0, 0, width, height, 8, 8); // Use labelColor here
    }
    internalCanvas.setCursor((width - text_width) / 2, (height + 6) / 2); // Center the label
    internalCanvas.println(string);
    // draw canvas
    switch (GetBlendMode())
    {
    case BlendMode::Normal:
        canvas.drawGrayscaleBitmap(GetOffsetPosition().x, GetOffsetPosition().y, internalCanvas.getBuffer(), (uint8_t)16U, internalCanvas.width(), internalCanvas.height());
        break;
    case BlendMode::Add:
        canvas.add(internalCanvas.getBuffer());
        break;
    case BlendMode::Sub:
        canvas.subtract(internalCanvas.getBuffer());
        break;
    default:
        break;
    }
}

bool C_Label::ContinueToDraw() const
{
    return !owner->IsQueuedForRemoval();
}

void C_Label::Awake()
{
    internalCanvas.setTextColor(labelColor);
    internalCanvas.setFont(&VGATypewriter8pt7b);
}

void C_Label::SetString(String string)
{
    this->string = string;
}

void C_Label::Update(uint8_t deltaTime)
{
}