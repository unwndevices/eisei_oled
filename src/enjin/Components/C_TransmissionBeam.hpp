#ifndef C_TRANSMISSIONBEAM_HPP
#define C_TRANSMISSIONBEAM_HPP

#include <vector>
#include <memory>

#include "enjin/Components/Component.hpp"
#include "enjin/Components/C_Drawable.hpp"
#include "enjin/Components/C_Position.hpp"
#include <Adafruit_GFX.h>
#include "enjin/utils/Polar.hpp"

#include "enjin/Object.hpp"

class C_TransmissionBeam : public C_Drawable
{
public:
    enum
    {
        IDLE = 0,
        PHASE_EDIT,
        WIDTH_EDIT
    };

    C_TransmissionBeam(Object *owner, uint8_t width, uint8_t height) : C_Drawable(width, height), Component(owner),
                                                                       internalCanvas(width, height),
                                                                       aim_point{(64, 127), (64, 127), (64, 127)},
                                                                       width(0.0f),
                                                                       phase(0.5f),
                                                                       color(12),
                                                                       phase_angle(0),
                                                                       half_width(0),
                                                                       start_angle(0),
                                                                       end_angle(0),
                                                                       mode(IDLE)
    {
        position = owner->GetComponent<C_Position>();

        if (!position)
        {
        }
        full_color = color;
        dimmed_color = color / 2;
    };
    void Awake() override{};
    void Update(uint8_t deltaTime) override
    {
        if (mode == WIDTH_EDIT)
        {
            timer += deltaTime;
            if (timer >= 140)
            { // Change the flashing speed by adjusting this value
                timer = 0;
                is_dimmed = !is_dimmed; // Toggle the brightness state
                if (!is_dimmed)
                {
                    color = dimmed_color;
                }
                else
                {
                    color = full_color;
                }
            }
        }
        else if (mode == PHASE_EDIT)
        {
        }
        else
        {
            color = full_color;
        }
    }
    void Draw(GFXcanvas8 &canvas) override
    {
        // todo it's possible to render the internal canvas only when the parameters change.
        internalCanvas.fillScreen(16);
        internalCanvas.drawLine(64, 64, aim_point[0].x, aim_point[0].y, color);
        internalCanvas.drawLine(64, 64, aim_point[1].x, aim_point[1].y, color / 2);
        internalCanvas.drawLine(64, 64, aim_point[2].x, aim_point[2].y, color / 2);
        for (int i = 0; i < 8; i++)
        {
            drawDottedArc(64, 64, 17 + i * 7, start_angle, end_angle, color);
        }
        canvas.drawGrayscaleBitmap(0, 0, internalCanvas.getBuffer(), (uint8_t)16, internalCanvas.width(), internalCanvas.height());
    };
    bool ContinueToDraw() const override
    {
        return !owner->IsQueuedForRemoval();
    };

    void SetMode(int mode)
    {
        this->mode = mode;
    }

    void SetPhase(float phase)
    {
        this->phase = phase;
        start_angle = (uint16_t)((phase - width * 0.5f) * 360.0f);
        end_angle = (uint16_t)((phase + width * 0.5f) * 360.0f);
        aim_point[0] = RadialToCartesian(phase, 63);
        aim_point[1] = RadialToCartesian(phase - width * 0.5f, 63);
        aim_point[2] = RadialToCartesian(phase + width * 0.5f, 63);
    };
    void SetWidth(float width)
    {
        this->width = width;
        start_angle = (uint16_t)((phase - width * 0.5f) * 360.0f);
        end_angle = (uint16_t)((phase + width * 0.5f) * 360.0f);
        aim_point[0] = RadialToCartesian(phase, 63);
        aim_point[1] = RadialToCartesian(phase - width * 0.5f, 63);
        aim_point[2] = RadialToCartesian(phase + width * 0.5f, 63);
    };

    void DrawBackground(GFXcanvas8 &canvas){};

private:
    float width, phase;
    uint8_t color, full_color, dimmed_color;
    int16_t phase_angle, half_width;
    GFXcanvas8 internalCanvas;
    Vector2 aim_point[3];
    int16_t start_angle, end_angle;

    int mode;
    uint8_t timer = 0;
    bool is_dimmed = false;

    void drawDottedArc(int x0, int y0, int radius, int startAngle, int endAngle, uint16_t color)
    {
        // Convert start and end angles to radians
        float startRad = startAngle * PI / 180.0;
        float endRad = endAngle * PI / 180.0;
        float midRad = (startRad + endRad) / 2.0;

        // Calculate the number of segments to use for the arc
        int numSegments = abs(endAngle - startAngle) / 7; // Adjust the denominator to change the smoothness of the arc

        // Calculate the angle between each segment
        float angleStep = (endRad - startRad) / numSegments;

        // Draw each segment of the arc
        for (int i = 0; i <= numSegments; i++)
        {
            float angle = startRad + i * angleStep;
            int x = x0 + radius * -cos(angle);
            int y = y0 + radius * -sin(angle);

            // Calculate the distance of the current angle from the middle of the arc
            float distance = abs(angle - midRad);

            // Use the distance to calculate the color
            uint16_t currentColor = color * (1.0 - (distance / (endRad - startRad)));

            internalCanvas.drawPixel(x, y, currentColor);
        }
    }
};

#endif // C_TRANSMISSIONBEAM_HPP
