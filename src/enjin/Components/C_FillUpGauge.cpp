#include "C_FillUpGauge.hpp"
#include "enjin/Object.hpp"

uint8_t pattern[16] = {8U, 0, 8U, 0, // row 1: white, black
                       0, 0, 0, 8U,
                       8U, 0, 8U, 0,
                       0, 8U, 0, 0}; // row 2: black, white

C_FillUpGauge::C_FillUpGauge(Object *owner, uint16_t width, uint16_t height, uint16_t color, GaugeMode mode)
    : C_Drawable(width, height), Component(owner), width(width), height(height), color(color), currentValue(0.5f), internalCanvas(width, height), mask(width, height), mode(mode)
{
    position = owner->GetComponent<C_Position>();

    if (!position)
    {
        std::cerr << "C_Tooltip requires C_Position component.\n";
    }
};
void C_FillUpGauge::Awake()
{
    // Initialize the internal canvas
    mask.fillCircle(width / 2, height / 2, width / 2, true);
}

void C_FillUpGauge::Draw(GFXcanvas8 &canvas)
{
    if (mode == GaugeMode::Unidirectional)
    {
        // Calculate the height of the filled area
        uint16_t filledHeight = height * currentValue;
        // Clear the internal canvas
        internalCanvas.fillScreen(16U);
        // Draw the filled area on the internal canvas
        internalCanvas.fillRectWithPattern(0, height - filledHeight, width, filledHeight, pattern, 4, 4);
        internalCanvas.drawRect(0, 0, width, height, color);

        // Draw the internal canvas on the main canvas
        canvas.drawGrayscaleBitmap(GetOffsetPosition().x, GetOffsetPosition().y, internalCanvas.getBuffer(), (uint8_t)16U, width, height);
    }
    else if (mode == GaugeMode::Bidirectional)
    {
        // Calculate the height of the filled area
        uint16_t filledHeight = abs(height * currentValue / 2);
        int16_t y = height / 2;
        filledHeight = filledHeight;
        // Clear the internal canvas
        internalCanvas.fillScreen(16U);

        // Draw the filled area on the internal canvas
        if (currentValue >= 0)
        {
            internalCanvas.fillRectWithPattern(0, height / 2 - filledHeight, width, filledHeight, pattern, 4, 4);
            internalCanvas.drawLine(0, height / 2 - filledHeight, width, height / 2 - filledHeight, 8); // Line at the top
        }
        else
        {
            internalCanvas.fillRectWithPattern(0, height / 2, width, filledHeight, pattern, 4, 4);
            internalCanvas.drawLine(0, height / 2 + filledHeight, width, height / 2 + filledHeight, 8); // Line at the bottom
        }

        internalCanvas.drawLine(0, height / 2, width, height / 2, color);
        internalCanvas.drawCircle(width / 2, height / 2, width / 2, color);
        // Draw the internal canvas on the main canvas

        canvas.drawGrayscaleBitmap(GetOffsetPosition().x,GetOffsetPosition().y, internalCanvas.getBuffer(), mask.getBuffer(), width, height);
    }
}

void C_FillUpGauge::SetValue(float value)
{
    // Clamp the value between 0.0 and 1.0
    if (mode == GaugeMode::Bidirectional)
    {
        currentValue = max(-1.0f, min(1.0f, value));
    }
    else
    {
        currentValue = max(0.0f, min(1.0f, value));
    }
}

float C_FillUpGauge::GetValue() const
{
    return currentValue;
}

bool C_FillUpGauge::ContinueToDraw() const
{
    return !owner->IsQueuedForRemoval();
}

void C_FillUpGauge::LateUpdate(uint8_t deltaTime)
{
}