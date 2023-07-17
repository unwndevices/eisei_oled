#include "enjin/Components/C_Planet.hpp"
#include "enjin/utils/Noise.hpp"
C_Planet::C_Planet(Object *owner, uint8_t radius) : C_Drawable(radius * 2 + 1, radius * 2 + 1), Component(owner), radius(radius), internalCanvas(radius * 2 + 1, radius * 2 + 1), textureCanvas(127, radius * 2 + 1)
{
    position = owner->GetComponent<C_Position>();

    phase = 0.0f;
    InitSphericalMap(radius);
    GenerateTerrain();
}

void C_Planet::InitSphericalMap(uint8_t radius)
{
    uint8_t diameter = radius * 2 + 1;

    // coordinate table generation
    for (int y = 0; y < diameter; y++)
    {
        for (int x = 0; x < diameter; x++)
        {
            Vector2 _pos;

            float centeredX = (float)x - (radius);
            float centeredY = (float)y - (radius);

            float sineLatitude = centeredY / (radius);
            float latitude = asin(sineLatitude);

            float circleRadius = (radius)*cos(latitude);
            float sineLongitude = centeredX / circleRadius;

            // inside circle ?
            if (sineLongitude >= -1.0f && sineLongitude <= 1.0f)
            {
                float longitude = asin(sineLongitude) + M_PI / 2;

                _pos.x = (longitude * (radius)) / M_PI;
                _pos.y = y;
            }

            else
            {
                _pos.x = -1;
                _pos.y = -1;
            }
            sphericalMap.push_back(_pos);
        }
    }
}

void C_Planet::GenerateTerrain()
{
    randomSeed(millis());
    uint16_t offset = (rand() % 50 + 1) * 50;

    // texture generation
    for (int x = 0; x < 127; x++)
    {
        for (int y = 0; y < radius * 2 + 1; y++)
        {
            // uint8_t value = (uint8_t)((pnoise((float)(x + offset) * 0.1f, (float)(y + offset) * 0.1f, width / 10, 10) + 1.0f) * 127.5f);
            // ridged function
            float scale = 0.1f; // Adjust this value to change the noise resolution
            uint8_t value = (uint8_t)(abs(pnoise((float)(x + offset) * scale, (float)(y + offset) * scale, 5, 33)) * -1.0f * 255.0f);

            if (value < 110)
                value = 0;
            else if (value < 150)
                value = 3;
            else if (value < 180)
                value = 7;
            else if (value < 200)
                value = 15;
            else if (value < 220)
                value = 10;
            else
                value = 2;
            textureCanvas.drawPixel(x, y, value);
        }
    }
}

void C_Planet::DrawPlanet()
{
    uint8_t diameter = radius * 2 + 1;
    internalCanvas.fillScreen(16U);
    // draw the sphere
    for (int y = 0; y < diameter; ++y)
    {
        for (int x = 0; x < diameter; ++x)
        {
            Vector2 *_pos = &sphericalMap[y * diameter + x];
            int px = (_pos->x + (int)(phase * 127) % diameter);
            int py = _pos->y;
            uint8_t color = 0;
            if (_pos->x < 0)
            {
                color = 16;
            }
            else
            {
                color = textureCanvas.getBuffer()[py * textureCanvas.width() + px];
            }
            internalCanvas.drawPixel(x, y, color);
        }
    }
}