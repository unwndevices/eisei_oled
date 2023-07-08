#ifndef SPRITE_HPP
#define SPRITE_HPP
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include "utils/Types.hpp"
class Sprite
{
public:
    Sprite()
    {
        _texture = nullptr;
        _width = 0;
        _height = 0;
        _matte = 16U;
        _position.x = 0;
        _position.y = 0;
        _frame = 0;
        _mode = BlendMode::Normal;
    };

    Sprite(const uint8_t texture[], uint8_t width, uint8_t height, BlendMode mode = BlendMode::Normal)
    {
        _texture = texture;
        _width = width;
        _height = height;
        _matte = 16U;
        _position.x = 0;
        _position.y = 0;
        _frame = 0;
        _mode = mode;
    };

    void draw(GFXcanvas8 &canvas)
    {
        const uint8_t *texture = (const uint8_t *)(_texture + (_frame * _width * _height));
        canvas.drawGrayscaleBitmap(_position.x, _position.y, texture, _matte, _width, _height);
    };

    void Add(GFXcanvas8 &canvas)
    {
        const uint8_t *texture = (const uint8_t *)(_texture + (_frame * _width * _height));
        canvas.add(texture);
    }

    void Subtract(GFXcanvas8 &canvas)
    {
        const uint8_t *texture = (const uint8_t *)(_texture + (_frame * _width * _height));
        canvas.subtract(texture);
    }

    void setTexture(const uint8_t texture[], uint8_t width, uint8_t height)
    {
        _texture = texture;
        _width = width;
        _height = height;
    };

    void setTexture(const uint8_t texture[], uint8_t frameId)
    {
        _texture = texture;
        _frame = frameId;
    };

    void setTexture(uint8_t frameId)
    {
        _frame = frameId;
    };

    void setPosition(int16_t x, int16_t y)
    {
        _position.x = x;
        _position.y = y;
    };
    void setPosition(Vector2 pos)
    {
        _position.x = pos.x;
        _position.y = pos.y;
    };
    void setMatte(uint8_t matte)
    {
        _matte = matte;
    };

    const uint8_t *GetTexture()
    {
        return (const uint8_t *)(_texture + (_frame * _width * _height));
    };

    uint8_t _width, _height, _frame;
    Vector2 _position;
    uint8_t _matte;
    BlendMode _mode;

    uint8_t GetWidth() { return _width; };
    uint8_t GetHeight() { return _height; };

protected:
    const uint8_t *_texture;
};

#endif // !SPRITE_H