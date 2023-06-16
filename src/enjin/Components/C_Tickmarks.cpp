#include "C_Tickmarks.hpp"
#include "enjin/Object.hpp"

void C_Tickmarks::Awake()
{
    // Initialize the internal canvas
}
void C_Tickmarks::Draw(GFXcanvas8 &canvas)
{
    // Clear the internal canvas
    internalCanvas.fillScreen(7);

    for (int16_t i = startAngle; i <= stopAngle; i += spacing)
    {
        int angle = i - ((int)(currentValue * 100.0f) * spacing / 10) % spacing - 90;   // final angle for the tickmark
        float tick_value = floorf((currentValue * 10.0f) + (angle + 90.0f) / spacing); // get number value for each tickmark        //  Determine the length of this tickmark
        uint8_t tickLength = ((int)tick_value % 10 == 0) ? length : length / 2;    
        // Calculate the start and end points of the tickmark
        uint8_t startX = center.x + (radius - tickLength) * sin(radians(angle));
        uint8_t startY = center.y + (radius - tickLength) * cos(radians(angle));
        uint8_t endX = startX + tickLength * sin(radians(angle));
        uint8_t endY = startY + tickLength * cos(radians(angle));

        // Draw the tickmark
        internalCanvas.drawLine(startX, startY, endX, endY, 0xFFFF);
    }
    // Draw the internal canvas onto the main canvas
    canvas.drawGrayscaleBitmap(0, 0, internalCanvas.getBuffer(), 0x7, 127, 127);
}

void C_Tickmarks::SetValue(float value)
{
    currentValue = value;
}

float C_Tickmarks::GetValue() const
{
    return currentValue;
}

bool C_Tickmarks::ContinueToDraw() const
{
    return !owner->IsQueuedForRemoval();
};